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

static int digitseq_parse_(const char *a_begin, int len, const char *&a_end);

/// Because strtoxx, sttoxx, etc. parse '-' and '+'.
/// For us, '-' has special meaning, and '+' is not accepted.
/// \param a_begin cstring to parse.
/// \param len Maximal number of chars to parse.
/// \param a_end Output: end of parse position.
/// \return Parsed number (overflow is not handled).
static int digitseq_parse_(const char *a_begin, int len, const char *&a_end) {
 int retv = 0;
 const char *it=a_begin;
 while (it < a_begin + len) {
  char x = *it;
  if (!isdigit(x)) break;
  retv = 10 * retv + (x - '0');
  ++it;
 }
 a_end=it;
 return retv;
}

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
///  projexpr::=[{first}][-[{last}]].
/// first and last are column identifier numbers (first column is identified by 0).
/// If the expression contains no dash ("-"), the interval contains only a single column.
/// If the left side of the dash (first) is omitted, the interval begin will be undefined (meaning that the interval begins with the first column);
/// if the right side of the dash (last) is omitted, the interval end will be undefined (meaning that the interval ends with the last column).
/// @param a Character sequence (cstring) to parse.
/// @param len length of parsing.
int ColIval::parse(const char *a, size_t len){
 const char *eotok0;
 const char *eotok1;
 auto lo = digitseq_parse_(a, len, eotok0);
 bool is_point = (eotok0 == (a + len));
 bool is_interval = !is_point && (*eotok0 == SYM_IVAL);
 first = (eotok0 == a) ? COLID_UNDEF : lo;
 if (!is_interval) {
  second = first;
  if (!is_point) { // there is an illegal character in a
   return 1;
  }
 } else { // interval
  auto a2 = eotok0 + 1;
  auto len2 = len - (a2 - a);
  auto hi = digitseq_parse_(a2, len2, eotok1);
  second = (eotok1 == a2) ? COLID_UNDEF : hi;
  if (eotok1 != (a2 + len2)) { // there is (are) remaining illegal character(s)
   return 1;
  }
 }
 return 0;
}

/// Point interval constructor.
/// \param a Lower and upper bound of the interval.
ColIval::ColIval(const ColID &a) {
 first = a;
 second = a;
}

/// Standard constructor.
/// \param a Lower bound of the interval.
/// \param b Upper bound of the interval (inclusive).
ColIval::ColIval(const ColID &a, const ColID &b) {
 first = a;
 second = b;
}

/// Standard parser function.
/// Note, this function does append to existing data.
/// \param a cstring to parse.
/// \param len Number of characters to parse.
/// \return Error code.
int ColIvalV::parse(const char *a, size_t len) {
 const char *a_end = a + len;
 const char *it=a;
 while (it < a_end) { // the question is: what to do with a==""? Now we accept it.
  const char* const comma_p=find(it, a_end, SYM_ENUM);
  ColIval elem;
  if (elem.parse(it, comma_p-it)) {
   return 1;
  }
  push_back(elem);
  it = comma_p + 1;
  if (it == a_end) { // ',' is illegal ending!
   return 1;
  }
 }
 return 0;
}

/// Wrapper to standard parser function.
/// \param a cstring to parse (its length is determined by strlen(a)).
/// \return Inherited.
int ColIvalV::parse(const char *a) {
 return parse(a,strlen(a));
}

/// Standard ctor of column sequence (ColIvalV).
/// \param a true: every column; false: no column.
ColIvalV::ColIvalV(bool a){
 if (a) {
  static const char all = SYM_IVAL;
  parse(&all, 1);
 }
}

/// Extracts sequence of column identifiers from column intervals.
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
