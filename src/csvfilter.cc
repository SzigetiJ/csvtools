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
#include <map>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "CsvPipe.h"
#include "RowFunc.h"
#include "DefaultCommandLine.h"
#include "log.h"

using namespace std;

const string DESCRIPTION="Filters selected rows of the CSV file. Takes stdin as input and puts result to stdout.\n";
const string USAGE="-r <cond> [-r <cond> ...]\n"
" cond ::= <op>:<expr>:str\t\t(filtering condition)\n"
" expr ::= <ival>[,<expr>]\t\t(column expression)\n"
" ival ::= num|-num|num-|num-num\t\t(column interval)\n"
" op ::= =|!=|<|<=|>|>=\t\t(comparison operator, = and != compare strings, each other compares numbers)\n"
"where str is string, num is integer\n";

typedef pair<string,OpFun> OpFunKey;
typedef map<string,OpFun> OpFunMap;

/// Avaliable row filter functions mapped by operator sign / function name.
const OpFunMap colop_m={
 {"=",[](const string &a, const string &b)->bool{return a==b;}},
 {"!=",[](const string &a, const string &b)->bool{return a!=b;}},
 {"<=",[](const string &a, const string &b)->bool{return atof(a.c_str())<=atof(b.c_str());}},
 {"<",[](const string &a, const string &b)->bool{return atof(a.c_str())<atof(b.c_str());}},
 {">=",[](const string &a, const string &b)->bool{return atof(a.c_str())>=atof(b.c_str());}},
 {">",[](const string &a, const string &b)->bool{return atof(a.c_str())>atof(b.c_str());}}
};

// Selection specific options.
const Option sel_option_a[]={
{"r","row",1,APPEND,"Defines row filtering condition."}
};
const int sel_option_n = sizeof(sel_option_a)/sizeof(Option);

/// Extension to DefaultCommandLine: SelectionCommandLine can derive row filtering conditions from row option value(s).
class SelectionCommandLine : public DefaultCommandLine {
 RowFilterV filter_v;
public:
 SelectionCommandLine(const string desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(sel_option_a,sel_option_a+sel_option_n)){};
 RowFilterV get_filters() const {
  return filter_v;
 };
 int process() {
  filter_v.clear();

  vector<vector<char*> > arg_v=get_values_for_longname("row");
  for (vector<char*> arg : arg_v){
   char *a=arg[0];
   char *ae=a+strlen(a);
   char *xptr=find(a,ae,':');
   *xptr=0;
   if (xptr==ae) {
    ERROR(logger, "Cannot parse condition.");
    return -1;
   }
   char *xptr1=find(xptr+1,ae,':');
   *xptr1=0;
   if (xptr1==ae) {
    ERROR(logger, "Cannot parse condition.");
    return -1;
   }

   string op=a;
   ColIvalV col_v=parse_projparam(xptr+1);
   string str=xptr1+1;
   OpFunMap::const_iterator comi=colop_m.find(op);
   if (comi==colop_m.end()) {
    ERROR(logger, "Unrecognized operator ["<<a<<"]. Aborting.");
    return -2;
   }
   OpFun co=comi->second;
   filter_v.push_back(make_pair(co,make_pair(col_v,str)));
   INFO(logger, "Parameter parsed.");
  }
  return 0;
 };
};


int main(int argc, char **argv){
 SelectionCommandLine cmdline=SelectionCommandLine(DESCRIPTION,USAGE);
 if (cmdline.parse(argc, argv) || cmdline.process()) {
  cmdline.print_help();
  return -1;
 }
 if (cmdline.print_if_needed())
  return 0;
 CsvPipe()
 .set_projection(ColIvalV(1,ColIval(COLID_UNDEF,COLID_UNDEF)))
 .set_filter(cmdline.get_filters())
 .process(cin,cout);
 return 0;
}
