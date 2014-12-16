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
#include "stl_out.h"
#include "log.h"

using namespace std;

/// \file ColTypes.cc Column related types and procedures.

/// Simple incremental integer generator. It can be initialized (first generated value).
class IncGen : public unary_function<void, int>{
 int last;
public:
 IncGen(int a):last(a){};
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
/// @return column interval.
ColIval parse_projexpr(char *a){
 ColIval retv=ColIval(COLID_UNDEF,COLID_UNDEF);
 char *ae=a+strlen(a);
 char *ptx=find(a,ae,'-');
 if (ptx==a){ // -num
  retv.second=atoi(a+1);
 } else if (ptx==ae-1){ // num-
  *ptx=0;
  retv.first=atoi(a);
 } else if (ptx==ae) { // num
  retv.first=atoi(a);
  retv.second=atoi(a);
 } else { // num-num
  *ptx=0;
  retv.first=atoi(a);
  retv.second=atoi(ptx+1);
 }
 INFO(logger,"ProjExpr: "<<retv);
 return retv;
}

/// Parses a projection parameter. The syntax of the projection parameter is
///  projparam::=projexpr[,projparam]
/// @param a Character sequence (cstring) to parse.
/// @return sequence of column intervals.
ColIvalV parse_projparam(char *a){
 ColIvalV retv;
 char *ae=a+strlen(a);
 while (a<ae) {
  char *ptx=find(a,ae,',');
  *ptx=0;
  ColIval projexpr=parse_projexpr(a);
  retv.push_back(projexpr);
  a=ptx+1;
 }
 return retv;
}

/// Extracts sequence of column indentifiers from column intervals.
/// @param a Column intervals.
/// @param len total number of columns (needed if {last} is omitted).
/// @return Sequence of column identifiers.
FieldV extract_ival(const ColIvalV &a, int len){
 FieldV retv;
 for (unsigned int i=0;i<a.size();++i){
  int cbegin=(a[i].first==COLID_UNDEF?0:a[i].first);
  int cend=(a[i].second==COLID_UNDEF?len:a[i].second+1);
   // exception:
  if (cend<cbegin) {
   WARN(logger, "Invalid interval: ["<<cbegin<<","<<cend<<"]");
   cend=cbegin;
  }
  generate_n(back_inserter(retv),cend-cbegin,IncGen(cbegin));
 }
 return retv;
}


