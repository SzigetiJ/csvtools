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
#include <algorithm>
#include "CommandLine.h"
#include "stl_out.h"

#define DASH_CHR '-'
#define EQ_CHR '='
using namespace std;

/// strict weak ordering by flag (lexicographic).
bool Option::operator<(const _Option &b) const {
 return std::strcmp(flag, b.flag) < 0;
};

ostream &operator<<(ostream &a, const Option &b){
 if (b.flag)
  a<<DASH_CHR<<b.flag;
 if (b.longname) {
  a<<"\t"<<DASH_CHR<<DASH_CHR<<b.longname;
  if (b.arg_num){
   for (int i=0; i<b.arg_num; ++i){
    a << (i==0?EQ_CHR:' ') << "<arg" << (i+1) << ">";
   }
  }
 }
 if (b.help) {
  a<<"\t"<<b.help;
 }
 return a;
};

CommandLine::CommandLine(const set<Option> &a):logger(LogConfig()),option_s(a),parsed(false){
 for (auto option : a){
  flag_m.insert(make_pair(option.flag,option));
  longname_m.insert(make_pair(option.longname,option));
 }
};

int CommandLine::parse(int argc, char **argv){
 progname=argv[0];
 int retv=0;
 int i=1;
 while (i<argc && retv==0) {
  char *xcmd=argv[i];
  Option xoption;
  int arg_chr_shift=0;
	// option begins with dash
  if (xcmd[0]==DASH_CHR) {
	// double dash denotes longname option
   if (xcmd[1]==DASH_CHR) {
	// longname option may be followed by equals sign
    char *xcmd_e=xcmd+strlen(xcmd);
    char *eq_pos=find(xcmd,xcmd_e,EQ_CHR);
    auto it=longname_m.find(string(xcmd+2,eq_pos));
    if (it==longname_m.end()) {
     ERROR(logger,"Cannot interpret option ["<<xcmd<<"], aborting.");
     return 1;
    }
    xoption=it->second;
    if (xoption.arg_num) {
     if (eq_pos==xcmd_e) {
      ERROR(logger,"Option ["<<xoption.longname<<"] requires argument concatenated with '"<<EQ_CHR<<"'. Aborting.");
      return 1;
     }
     arg_chr_shift=eq_pos-xcmd+1;
    } else {
     if (eq_pos!=xcmd_e) {
      ERROR(logger,"Option ["<<xoption.longname<<"] does not require any argument concatenated with '"<<EQ_CHR<<"'. Aborting.");
      return 1;
     }
     ++i;
    }
   } else {
    auto it=flag_m.find(xcmd+1);
    if (it==flag_m.end()) {
     ERROR(logger,"Cannot interpret option ["<<xcmd<<"], skipped.");
      return 1;
    }
    xoption=it->second;
    ++i;
   }
   vector<char *> value_v;
   for (int j=0;j<xoption.arg_num;++j,++i){
    value_v.push_back(argv[i]+arg_chr_shift);
    arg_chr_shift=0;
   }
   value_m.insert(make_pair(xoption,value_v));
   
  } else {
   ERROR(logger,"Parse error at cmdline argument #"<<i<<": "<<xcmd);
   return 1;
  }
 }
 DEBUG(logger,"parsed: "<<value_m);
 parsed=true;
 return retv;
};

bool CommandLine::is_set_option(const Option &a) const {
 return value_m.find(a)!=value_m.end();
};
bool CommandLine::is_set_flag(const char *a) const {
 return flag_m.find(a)!=flag_m.end() && is_set_option(flag_m.find(a)->second);
};
bool CommandLine::is_set_longname(const char *a) const {
 return longname_m.find(a)!=longname_m.end() && is_set_option(longname_m.find(a)->second);
};

vector<vector<char*> > CommandLine::get_values_for_option(const Option &a) const {
 vector<vector<char*> > retv;
 for (auto it = value_m.lower_bound(a);it!=value_m.upper_bound(a); ++it)
  retv.push_back(it->second);
 return retv;
};
vector<vector<char*> > CommandLine::get_values_for_flag(const char *a) const {
 return flag_m.find(a)!=flag_m.end()?get_values_for_option(flag_m.find(a)->second):vector<vector<char*> >();
};
vector<vector<char*> > CommandLine::get_values_for_longname(const char *a) const {
 return longname_m.find(a)!=longname_m.end()?get_values_for_option(longname_m.find(a)->second):vector<vector<char*> >();
};

