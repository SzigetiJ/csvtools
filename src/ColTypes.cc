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

#define SYM_IVAL '-'
#define SYM_ENUM ','

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
ColIval::ColIval(const string &a){
 auto dash_p = a.find(SYM_IVAL);
 bool is_interval = (dash_p != string::npos);
 if (!is_interval) dash_p = a.length();
 bool undef_begin = (dash_p == 0u);
 bool undef_end = (dash_p == a.length() - 1) || a.empty();
 first=undef_begin?COLID_UNDEF:stoi(a.substr(0, dash_p));
 second=undef_end?COLID_UNDEF:
  is_interval?stoi(a.substr(dash_p + 1, a.length() - (dash_p + 1))):first;
}

ColIval::ColIval(const ColID &a) {
 first = a;
 second = a;
}

/// Common part of ColIvalV ctors.
/// \param res ColIvalV instance to initialize.
/// \param b begin of column expression cstring.
/// \param e end of column expression cstring.
static void init(ColIvalV &res, const char *b, const char *e) {
 while (b < e) {
  const char* const comma_p=find(b, e, SYM_ENUM);
  res.push_back(ColIval(string(b, comma_p)));
  b = comma_p + 1;
 }
}

/// Parses a projection parameter. The syntax of the projection parameter is
///  projparam::=projexpr[,projparam]
/// @param a Character sequence (cstring) to parse.
ColIvalV::ColIvalV(const char *a){
 init(*this, a, a + strlen(a));
}

ColIvalV::ColIvalV(const char *a, unsigned int len){
 init(*this, a, a + len);
}

ColIvalV::ColIvalV(const char *a, const char *a_end) {
 init(*this, a, a_end);
}

/// Standard ctor of column sequence (ColIvalV).
/// \param a true: every column; false: no column.
ColIvalV::ColIvalV(bool a){
 if (a) {
  static const char all = SYM_IVAL;
  init(*this, &all, &all + 1);
 }
}

/// Extracts sequence of column indentifiers from column intervals.
/// @param len total number of columns (used only if {last} is omitted).
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
