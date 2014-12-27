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
#include "ColTypes.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <functional>
#include "log.h"

using namespace std;

/// \file ColTypes.cc Column related types and procedures.

/// Simple incremental integer generator. It can be initialized (first generated value).
class Sequence : public unary_function<void, int>{
 int last;
public:
 Sequence(int a=0):last(a){};
 int operator()(){
  return last++;
 }
};

/// Parses a column interval (projection expression).
/// The syntax of a column interval is
///  projexpr::={first}[-[{last}]]|-{last}.
/// first and last are column identifier numbers (first column is identified by 0).
/// If the expression contains no dash ("-"), the interval contains only a single column.
/// If the left side of the dash (first) is omitted, the interval begin will be undefined (meaning that the interval begins with the first column);
/// if the right side of the dash (last) is omitted, the interval end will be undifined (meaning that the interval ends with the last column).
/// @param a Character sequence (cstring) to parse.
ColIval::ColIval(const char *a){
 const char *a_end=a+strlen(a);
 const char *dash_p=find(a,a_end,'-');
 bool is_interval=(dash_p!=a_end);
 bool undef_begin=(dash_p==a);
 bool undef_end=(dash_p==a_end-1);
 first=undef_begin?COLID_UNDEF:stoi(string(a,dash_p));
 second=undef_end?COLID_UNDEF:
 is_interval?stoi(dash_p+1):first;
}

ColIval::ColIval(const ColID &a) {
 first=a;
 second=a;
}
/// Parses a projection parameter. The syntax of the projection parameter is
///  projparam::=projexpr[,projparam]
/// @param a Character sequence (cstring) to parse.
ColIvalV::ColIvalV(const char *a){
 const char* const a_end=a+strlen(a);
 while (a<a_end) {
  const char* const comma_p=find(a,a_end,',');
  push_back(ColIval(string(a,comma_p).c_str()));
  a=comma_p+1;
 }
}

/// Extracts sequence of column indentifiers from column intervals.
/// @param len total number of columns (needed if {last} is omitted).
/// @return Sequence of column identifiers.
FieldV ColIvalV::extract_ival(int len) const {
 FieldV retv;
 for (unsigned int i=0;i<size();++i){
  int cbegin=(at(i).first==COLID_UNDEF?0:at(i).first);
  int cend=(at(i).second==COLID_UNDEF?len:at(i).second+1);
   // exception:
  if (cend<cbegin) {
//   WARN(logger, "Invalid interval: ["<<cbegin<<","<<cend<<"]");
   cend=cbegin;
  }
  generate_n(back_inserter(retv),cend-cbegin,Sequence(cbegin));
 }
 return retv;
}


