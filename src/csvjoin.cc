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
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "CsvPipe.h"
#include "CsvRow.h"
#include "RowOrderKey.h"
#include "DefaultCommandLine.h"
#include "CsvIStream.h"
#include "MultiSetJoinAlgorithm.h"
#include "log.h"
#include "stl_out.h"

using namespace std;

const string DESCRIPTION="Joins two CSV file. Left CSV file is taken from stdin, the filename of the right CSV data is given as argument and the result is put to stdout.\n";
const string USAGE="-{jf} filename {-jt {natural|cross} | -jt {inner|outer} -jc <jexpr> [-jc <jexpr> ...]}\n"
" jexpr ::= <expr>:<expr>\t\t(join column expression - left table columns:right table columns)\n"
" expr ::= <ival>[,<expr>]\t\t(column expression)\n"
" ival ::= num|-num|num-|num-num\t\t(column interval)\n"
"where num is integer and column numbering begins with 0.\n";

// Selection specific options.
const Option join_option_a[]={
{"jf","join-file",1,OVERRIDE,"Right CSV file."},
{"jt","join-type",1,OVERRIDE,"Type of join: natural|cross|inner|outer. Default: inner."},
{"jc","join-columns",1,APPEND,"Column expression pair for join."}
};
const int join_option_n = sizeof(join_option_a)/sizeof(Option);

/// Extension to DefaultCommandLine: JoinCommandLine can derive join parameters from options.
class JoinCommandLine : public DefaultCommandLine {
 pair<ColIvalV,ColIvalV> join_columns=make_pair(ColIvalV("",0u),ColIvalV("",0u));
 const char *right_fname = NULL;
 JoinType join_type=JOIN_INNER;
public:
 JoinCommandLine(const string &desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(join_option_a,join_option_a+join_option_n)){};

 int process() {
// join type
  if (is_set_flag("jt")) {
   join_type<<get_arg_for_flag("jt");
  }
// right table
  if (is_set_flag("jf")) {
   right_fname=get_arg_for_flag("jf");
  } else {
   ERROR(get_log_config(),"Right table is not defined.");
   return 2;
  }
// join columns
  if (join_type != JOIN_NATURAL && join_type != JOIN_CROSS && set_join_columns()) {
   ERROR(get_log_config(), "Chosen join type requires join columns definition.");
   return 3;
  }
  return 0;
 };

 int set_join_columns() {
  if (!is_set_flag("jc")){
   return 1;
  }
  vector<vector<const char*> > arg_v=get_values_for_flag("jc");
  for (vector<const char*> arg : arg_v){
   const char *sep=find(arg[0], arg[0] + strlen(arg[0]), ':');
   ColIvalV tmpleft(arg[0], sep);
   ColIvalV tmpright(sep + 1);
   join_columns.first.insert(join_columns.first.end(),tmpleft.begin(),tmpleft.end());
   join_columns.second.insert(join_columns.second.end(),tmpright.begin(),tmpright.end());
  }
  return 0;
 };

 // getters
 const char *get_right_fname() const {return right_fname;};
 JoinType get_join_type() const {return join_type;};
 pair<ColIvalV,ColIvalV> get_join_columns() const {return join_columns;};
};


int main(int argc, const char *argv[]){
 JoinCommandLine cmdline=JoinCommandLine(DESCRIPTION,USAGE);
 CommandLineExecuteResponse resp=cmdline.execute(argc, argv);
 if (resp!=CMDLINE_OK) {
  return resp;
 }

 Delimiters delims;
 EscapeStrategy strat=ESC_PRESERVE;

 ifstream right_fstream(cmdline.get_right_fname());
 if (right_fstream.rdstate()) {
  ERROR(cmdline.get_log_config(),"Cannot read input file.");
  return 2;
 }
 CsvIStream left_table(cin,delims);
 CsvIStream right_table(right_fstream,delims);
 JoinAlgorithm *join_alg=new MultiSetJoinAlgorithm(
  &left_table,
  &right_table,
  cmdline.get_join_type(),
  cmdline.get_join_columns());

 join_alg->get_head().print(cout,delims,strat);cout<<delims.get(ORS);
 while (join_alg->next()){
  join_alg->get_current_line().print(cout,delims,strat);cout<<delims.get(ORS);
 }

 delete join_alg;
 right_fstream.close();
 return 0;
}
