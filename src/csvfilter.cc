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
#include "log.h"

using namespace std;

LogConfig logger;

string HELP="Projects selected columns of the CSV file. The program works with <stdin> and <stdout>\n"
"Usage:\n"
"csvproj [OPTIONS]\n"
"Options:\n"
"\t<op>:<exp>[,<exp>]:<str>\tfield list\n"
"\t<exp>:\tnum|-num|num-|num-num\n"
"\t<op>:\t=|!=|<|<=|>|>=\t(= and != compare strings, each other compares numbers)\n"
"\t<str>:\tstring\n";


bool op_streq(const string &a, const string &b){
 return a==b;
}

bool op_strne(const string &a, const string &b){
 return a!=b;
}

bool op_numle(const string &a, const string &b){
 return atof(a.c_str())<=atof(b.c_str());
}

bool op_numlt(const string &a, const string &b){
 return atof(a.c_str())<atof(b.c_str());
}

bool op_numge(const string &a, const string &b){
 return atof(a.c_str())>=atof(b.c_str());
}

bool op_numgt(const string &a, const string &b){
 return atof(a.c_str())>atof(b.c_str());
}

typedef pair<string,OpFun> OpFunKey;
typedef map<string,OpFun> OpFunMap;


const OpFunMap colop_m={
 OpFunKey("=",op_streq),
 OpFunKey("!=",op_strne),
 OpFunKey("<=",op_numle),
 OpFunKey("<",op_numlt),
 OpFunKey(">=",op_numge),
 OpFunKey(">",op_numgt)
};

int proc_param(char *a, RowFilterV &b){
 INFO(logger,"Parsing parameter ["<<a<<"]");
 char *ae=a+strlen(a);
 char *xptr=find(a,ae,':');
 *xptr=0;
 if (xptr==ae)
  return -1;
 char *xptr1=find(xptr+1,ae,':');
 *xptr1=0;
 if (xptr1==ae)
  return -1;

 string op=a;
 ColIvalV col_v=parse_projparam(xptr+1);
 string str=xptr1+1;
 OpFunMap::const_iterator comi=colop_m.find(op);
 if (comi==colop_m.end()) {
  WARN(logger, "Unrecognized operator ["<<a<<"]. Filter skipped.");
  return -2;
 }
 OpFun co=comi->second;
 b.push_back(make_pair(co,make_pair(col_v,str)));
 INFO(logger, "Parameter parsed.");
 return 0;
};

int main(int argc, char **argv){
 RowFilterV filter_v;

 if (argc<2){
  cerr<<HELP<<endl;
  return -1;
 }
 for (int i=1;i<argc;++i){
  proc_param(argv[i], filter_v);
 }

 CsvPipe()
 .set_projection(ColIvalV(1,ColIval(COLID_UNDEF,COLID_UNDEF)))
 .set_filter(filter_v)
 .process(cin,cout);
 return 0;
}
