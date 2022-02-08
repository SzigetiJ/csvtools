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
#include "CsvPipe.h"
#include "ColTypes.h"
#include "DefaultCommandLine.h"
#include "log.h"

using namespace std;

// Projection specific options.
const Option proj_option_a[]={
{"c","columns",1,APPEND,"Define columns."}
};
const int proj_option_n = sizeof(proj_option_a)/sizeof(Option);

/// Extension to DefaultCommandLine: ProjectionCommandLine can derive column intervals from columns option value(s).
class ProjectionCommandLine : public DefaultCommandLine {

public:
 ProjectionCommandLine(const string &desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(proj_option_a,proj_option_a+proj_option_n)){};
 int process(){
  return 0;
 };
 ColIvalV get_intervals() const {
  ColIvalV retv(false);
  vector<vector<const char*> > arg_v=get_values_for_longname("columns");
  for (vector<const char*> arg : arg_v){
   ColIvalV tmpproj(arg[0]);
   retv.insert(retv.end(), tmpproj.begin(), tmpproj.end());
  }
  return retv;
 };
};

const string DESCRIPTION="Filters columns of the csv file (projection). Takes stdin as input and puts result to stdout.\n";
const string USAGE="-c <expr> [-c <expr> ...]\n"
" expr ::= <ival>[,<expr>]\t\t(column expression)\n"
" ival ::= num|-num|num-|num-num\t\t(column interval)\n"
"where num is integer value, for the first column num=0.";

int main(int argc, const char *argv[]){
 ProjectionCommandLine cmdline=ProjectionCommandLine(DESCRIPTION,USAGE);
 CommandLineExecuteResponse resp=cmdline.execute(argc, argv);
 if (resp!=CMDLINE_OK) {
  return resp;
 }
 CsvPipe()
 .set_projection(cmdline.get_intervals())
 .process(cin,cout);
 return 0;
}
