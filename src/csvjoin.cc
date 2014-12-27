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
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "CsvPipe.h"
#include "CsvRow.h"
#include "RowOrderKey.h"
#include "DefaultCommandLine.h"
#include "log.h"
#include "stl_out.h"

using namespace std;

typedef enum {JOIN_NATURAL, JOIN_CROSS, JOIN_INNER, JOIN_OUTER, JOIN_UNDEF} JoinType;

JoinType &operator<<(JoinType &a, const string &b){
 a=
  "natural"==b?JOIN_NATURAL:
  "cross"==b?JOIN_CROSS:
  "inner"==b?JOIN_INNER:
  "outer"==b?JOIN_OUTER:
  JOIN_UNDEF;
 return a;
};

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
 pair<FieldV,FieldV> join_fields;
 pair<ColIvalV,ColIvalV> join_columns=make_pair("","");
 CsvRow right_head;
 list<CsvRow> right_body;
 JoinType join_type=JOIN_INNER;
public:
 JoinCommandLine(const string desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(join_option_a,join_option_a+join_option_n)){};
 int process() {
// general
  if (DefaultCommandLine::process()) {
   return 1;
  }
// join type
  if (is_set_flag("jt")) {
   join_type<<get_values_for_flag("jt")[0][0];
  }
// right table
  if (set_right_table()) {
   ERROR(get_log_config(), "Cannot process right table.");
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
  vector<vector<char*> > arg_v=get_values_for_flag("jc");
  for (vector<char*> arg : arg_v){
   char *sep=find(arg[0],arg[0]+strlen(arg[0]),':');
   *sep=0;
   ColIvalV tmpleft(arg[0]);
   ColIvalV tmpright(sep+1);
   join_columns.first.insert(join_columns.first.end(),tmpleft.begin(),tmpleft.end());  
   join_columns.second.insert(join_columns.second.end(),tmpright.begin(),tmpright.end());
  }
  return 0;
 };
 int set_right_table(){
  if (!is_set_flag("jf")){
   ERROR(get_log_config(),"Right table is not defined.");
   return 1;
  }
   ifstream fin(get_values_for_flag("jf")[0][0]);
   if (fin.rdstate()) {
    ERROR(get_log_config(),"Cannot read input file.");
    return 2;
   }
   if (!right_head.parse(fin,Delimiters())) {
    WARN(get_log_config(),"Right file empty.");
    return 0;
   }
   CsvRow row;
   while (row.parse(fin,Delimiters())) {
    right_body.push_back(row);
    row.clear();
   }
   fin.close();
  return 0;
 }

 void update_join_fields(const CsvRow &left_head) {
  if (join_type == JOIN_CROSS) {
   join_columns=make_pair("-","-");
  }
  if (join_type == JOIN_NATURAL) {
   for (unsigned int i=0; i<right_head.size(); ++i) {
    auto it=find(left_head.begin(),left_head.end(),right_head.at(i));
    if (it!=left_head.end()){
     join_fields.first.push_back(i);
     int j=it-left_head.begin();
     join_fields.second.push_back(j);
    }
   }
  } else {
   join_fields=make_pair(
    join_columns.first.extract_ival(left_head.size()),
    join_columns.second.extract_ival(right_head.size()));
  }
// sort right table by join columns
 list<RowOrderKey> right_order_keys;
 transform(join_fields.second.begin(),join_fields.second.end(),back_inserter(right_order_keys),[](const ColID &a)->RowOrderKey {return RowOrderKey(a,false,false,false);});
 right_body.sort([compare=right_order_keys](const CsvRow &a, const CsvRow &b)->bool {
  for (auto key : compare) {
   int diff=key.compare(a,b);
   if (diff!=0) {
    return diff<0;
   }
  }
  return false;
 });
 }
 list<CsvRow> get_right_body() const {
  return right_body;
 }
};


int main(int argc, char **argv){
 JoinCommandLine cmdline=JoinCommandLine(DESCRIPTION,USAGE);
 if (cmdline.parse(argc, argv) || cmdline.process()) {
  cmdline.print_help();
  return -1;
 }
 if (cmdline.print_if_needed())
  return 0;

// header
 CsvRow left_head;
 if (!left_head.parse(cin, Delimiters())){
  ERROR(cmdline.get_log_config(),"Cannot parse CSV header");
  return -2;
 }
 cmdline.update_join_fields(left_head);
 
// TODO match rows

 return 0;
}
