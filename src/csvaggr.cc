/*
 *  Copyright (C) 2014 SZIGETI János <szigeti at pilar dot hu>
 *
 *  This file is part of CsvTools.
 *
 *  CsvTools is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  CsvTools is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 * along with CsvTools. If not, see http://www.gnu.org/licenses/.
 */
#include <iostream>
#include <sstream>
#include <algorithm>
#include "CsvPipe.h"
#include "CsvRow.h"
#include "Delimiters.h"
#include "DefaultCommandLine.h"
#include "Numeric.h"
#include "log.h"
#include "stl_out.h"

using namespace std;

const string DESCRIPTION="Aggregates columns of CSV file. Input file must be sorted.\n";
const string USAGE="-a <name> <fun> <expr> [-a <name> <fun> <expr> [...]]\n"
" name ::= str\t\t(name of the aggregated output column)\n"
" expr ::= <ival>[,<expr>]\t\t(column expression)\n"
" ival ::= num|-num|num-|num-num\t\t(column interval)\n"
" fun ::= sum|min|max|count|concat\t\t(aggregation function, sum, min and max aggregates numeric values, count counts not empty cells, concat concatenates cells without separator)\n"
"where str is string, num is integer\n";

/// General interface for aggregation (column) functions.
class AggrFunIface {
public:
 virtual void *init_value(const string&) const = 0;
 virtual void *accumulate(void*, const string&) const = 0;
 virtual string to_string(void*) const = 0;
 virtual void delete_value(void*) const = 0;
 virtual ~AggrFunIface() = default;
};

/// Base implementation of aggregation interface.
/// It can be used with any type that supports T->string conversion.
/// Accumulator function MUST be given,
/// initializer function MAY be given (defaults to T(string&) constructor).
template<typename T>
class AggrFunBase : public AggrFunIface {
 function<T & (T&, const string&)> accumulator;
 function<void * (const string&)> initializer;
public:
 AggrFunBase(const function<T & (T&, const string&)> &a, const function<void * (const string&)> &b=[](const string &a)->void*{return new T(a);}):accumulator(a),initializer(b){}
 void *init_value(const string &a) const {return initializer(a);}
 void *accumulate(void *a, const string &b) const {
  T &item=*(reinterpret_cast<T*>(a));
  accumulator(item,b);
  return &item;
 }
 virtual string to_string(void *a) const { return *(reinterpret_cast<string*>(a));}
 void delete_value(void *a) const {delete reinterpret_cast<T*>(a);}
};

/// Aggregation function dealing with numeric values.
class AggrFunNumeric : public AggrFunBase<Numeric> {
public:
 AggrFunNumeric(const function<Numeric& (Numeric&, const string&)> &a):AggrFunBase<Numeric>(a){}
 string to_string(void *a) const {
  return reinterpret_cast<Numeric*>(a)->to_string();
 }
};

/// Aggregation for counting. Use with type T=int|unsigned|long...
template<typename T>
class AggrFunCnt : public AggrFunBase<T> {
public:
 AggrFunCnt():AggrFunBase<T>([](T &a, const string &b)->T&{return a+=(b.empty()?0:1);},[](const string &a)->void*{return new T(a.empty()?0:1);}){}
 string to_string(void *a) const {
  return std::to_string(*reinterpret_cast<T*>(a));
 }
};

/// Represents an aggregated value.
/// Internally the aggregated value can be of ANY type.
/// There are 3 requirements against the aggregated value:
/// 1.) it must accept a string as initial value;
/// 2.) it must accept a string value for accumulation;
/// 3.) it must present a string value as result.
class AggrValue {
 const AggrFunIface *fun; 
 bool has_dat;
 void *dat;
public:
 AggrValue(const AggrFunIface *f):fun(f),has_dat(false){};
 bool init(const string &a){if (!has_dat) {dat=fun->init_value(a); has_dat=true; return true;} return false;};
 void accumulate(const string &a){fun->accumulate(dat,a);};
 string get_dat(){return fun->to_string(dat);};
 bool cleanup(){if (has_dat) {fun->delete_value(dat);has_dat=false; return true;} return false;};
 bool valid_dat() const {return has_dat;};
};

typedef pair<string,AggrFunIface*> AggrFunKey;
typedef map<string,AggrFunIface*> AggrFunMap;

typedef tuple<char*, AggrFunIface*, ColIvalV, FieldV> AggrCol;

// Available column functions mapped by name.
const AggrFunMap colfun_m={
 {"sum",new AggrFunNumeric([](Numeric &a, const string &b)->Numeric&{return a+=b;})},
 {"min",new AggrFunNumeric([](Numeric &a, const string &b)->Numeric&{
    Numeric bnum(b);
    if (bnum<a) {
     a=bnum;
    }
    return a;
   })},
 {"max",new AggrFunNumeric([](Numeric &a, const string &b)->Numeric&{
    Numeric bnum(b);
    if (a<bnum) {
     a=bnum;
    }
    return a;
   })},
 {"count",new AggrFunCnt<int>()},
 {"concat",new AggrFunBase<string>([](string &a, const string &b)->string&{
    return a+=b;
   })}
};

// Aggregation specific options.
const Option aggr_option_a[]={
{"a","aggregation",3,APPEND,"Defines aggregation on column(s). arg1: new column name, arg2: aggr.function (sum|min|max|count|concat), arg3: column(s)"}
};
const int aggr_option_n = sizeof(aggr_option_a)/sizeof(Option);

/// Extension to DefaultCommandLine: AggregationCommandLine can derive from option value(s) which columns are aggregated and which columns are grouped.
class AggregationCommandLine : public DefaultCommandLine {
 vector<AggrCol> aggr_v;
 FieldV remain_v;	///< Set of fields that are not part of any aggregation.
public:
 AggregationCommandLine(const string desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(aggr_option_a,aggr_option_a+aggr_option_n)){};

 int process() {
  for (vector<char*> arg : get_values_for_flag("a")) {
   char *name=arg[0];
   char *fun=arg[1];
   char *expr=arg[2];

   ColIvalV col_v(expr);
   AggrFunMap::const_iterator fmi=colfun_m.find(fun);
   if (fmi==colfun_m.end()) {
    ERROR(logger, "Unrecognized function ["<<fun<<"]. Aborting.");
    return -2;
   }
   AggrFunIface *af=fmi->second;
   aggr_v.push_back(make_tuple(name,af,col_v,FieldV()));
   INFO(global_logger, "Aggregation parsed.");
  }
  return 0;
 };

 void set_colnum(int size){
  INFO(global_logger,"columns: "<<size);
  auto all=ColIvalV("-").extract_ival(size);
  set<ColID> remain_s(all.begin(),all.end());
  for (AggrCol &aggr : aggr_v) {
   get<3>(aggr) = get<2>(aggr).extract_ival(size);
   for (auto field : get<3>(aggr)) {
    remain_s.erase(field);
   }
  }
  remain_v.clear();
  remain_v.insert(remain_v.end(),remain_s.begin(),remain_s.end());
  INFO(global_logger,"columns: "<<remain_s);
 }
 FieldV get_remain() const {return remain_v;};
 CsvRow get_aggr_head() const {
  CsvRow retv;
  for_each(aggr_v.begin(),aggr_v.end(),
   [&retv](const AggrCol &a){retv.push_back(CsvCell(get<0>(a),true));});
  return retv;
 };
 vector<AggrCol> get_aggrcols() const {
  return aggr_v;
 };
};


void flush_aggr_line(
 const CsvRow &ref,
 ostream &os,
 const Delimiters &delims,
 const EscapeStrategy &strat,
 vector<AggrValue> &avals,
 bool refill=false,
 vector<string> refill_v=vector<string>()
){
  ref.print(os,delims,strat);
   if (!ref.empty() && !avals.empty()){
    os<<delims.get(OFS);
   }
   for (unsigned int i=0; i < avals.size(); ++i) {
    CsvCell acell(avals[i].get_dat(),false);
    avals[i].cleanup();
    acell.print(os,delims,strat);
    if (i!=avals.size()-1){
     os<<delims.get(OFS);
    }
    if (refill)
     avals[i].init(refill_v[i]);
   }
   os<<delims.get(ORS);
};


int main(int argc, char **argv){
 const Delimiters delims;
 const EscapeStrategy strat=ESC_PRESERVE;

 AggregationCommandLine cmdline=AggregationCommandLine(DESCRIPTION,USAGE);
 CommandLineExecuteResponse resp=cmdline.execute(argc, argv);
 if (resp!=CMDLINE_OK) {
  return resp;
 }
// header
 CsvRow head;
 if (!head.parse(cin, delims)){
  ERROR(cmdline.get_log_config(),"Cannot parse CSV header");
  return -2;
 }
 cmdline.set_colnum(head.size());
 (head.get_fields(cmdline.get_remain())+=cmdline.get_aggr_head())
  .print(cout,delims,strat);
 cout<<delims.get(ORS);

// body
 auto acols=cmdline.get_aggrcols();
 vector<AggrValue> avalues;
 for (auto acol : acols) {
  avalues.push_back(AggrValue(get<1>(acol)));
 }
 
 bool first=true;
 CsvRow ref;
 CsvRow rx;
 while (rx.parse(cin, delims)){
  if (first) {
   ref=rx.get_fields(cmdline.get_remain());
   first=false;
  }
  CsvRow group=rx.get_fields(cmdline.get_remain());
  vector<string> to_aggr;
  for (auto acol : acols) {
   stringstream ss;
   rx.get_fields(get<3>(acol)).print(ss,Delimiters(),ESC_REMOVE);
   to_aggr.push_back(ss.str());
  }
  if (group==ref) {
   INFO(global_logger,"continuing aggregation.");
// accumulate
   for (unsigned int i=0; i < acols.size(); ++i) {
    if (avalues[i].valid_dat()) {
     avalues[i].accumulate(to_aggr[i]);
    } else {
     avalues[i].init(to_aggr[i]);
    }
   }
  } else {
   INFO(global_logger,"flushing aggregation.");
// flush
   flush_aggr_line(ref,cout,delims,strat,avalues,true,to_aggr);
// set new ref
   ref=group;
  }
  rx.clear();
 }
 flush_aggr_line(ref,cout,delims,strat,avalues);

 for (auto item : colfun_m){
  delete (item.second);
 }
 return 0;
}

