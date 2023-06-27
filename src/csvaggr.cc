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
#include <memory>
#include "ColFunc.h"
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
 + string(colivalv_expr_help_str) +
" fun ::= sum|min|max|count|concat|first|last\t\t(aggregation function, sum, min and max aggregates numeric values, count counts not empty cells, concat concatenates cells without separator)\n"
"where str is string, num is integer\n";

typedef tuple<const char*, AggrFunPtr, ColIvalV, FieldV> AggrCol;

// Aggregation specific options.
const Option aggr_option_a[]={
{"a","aggregation",3,APPEND,"Defines aggregation on column(s). arg1: new column name, arg2: aggr.function (sum|min|max|count|concat|first|last), arg3: column(s)"}
};
const int aggr_option_n = sizeof(aggr_option_a)/sizeof(Option);

/// Extension to DefaultCommandLine: AggregationCommandLine can derive from option value(s) which columns are aggregated and which columns are grouped.
class AggregationCommandLine : public DefaultCommandLine {
 vector<AggrCol> aggr_v;
 FieldV remain_v;	///< Set of fields that are not part of any aggregation.
public:
 AggregationCommandLine(const string &desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(aggr_option_a,aggr_option_a+aggr_option_n)){};

 int process() {
  for (vector<const char*> arg : get_values_for_flag("a")) {
   const char *name=arg[0];
   const char *fun=arg[1];
   const char *expr=arg[2];

   ColIvalV col_v(false);
   if (col_v.parse(expr)) {
    ERROR(get_log_config(), "Illegal column expression [" << expr << "].");
    return -1;
   }
   AggrFunMap::const_iterator fmi=std_colfun_m.find(fun);
   if (fmi==std_colfun_m.end()) {
    ERROR(logger, "Unrecognized function ["<<fun<<"]. Aborting.");
    return -2;
   }
   AggrFunPtr af=fmi->second.get();
   aggr_v.push_back(make_tuple(name,af,col_v,FieldV()));
   INFO(global_logger, "Aggregation parsed.");
  }
  return 0;
 };

 void set_colnum(int size){
  INFO(global_logger,"columns: "<<size);
  auto all=ColIvalV(true).extract_ival(size);
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
 const vector<string> &refill_v=vector<string>()
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
}


int main(int argc, const char *argv[]){
 const Delimiters delims;
 const EscapeStrategy strat = ESC_PRESERVE;

 AggregationCommandLine cmdline = AggregationCommandLine(DESCRIPTION,USAGE);
 CommandLineExecuteResponse resp = cmdline.execute(argc, argv);
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
  to_aggr.reserve(acols.size());
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

 return 0;
}
