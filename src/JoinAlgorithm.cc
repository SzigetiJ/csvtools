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
#include <algorithm>
#include "JoinAlgorithm.h"
#include "CsvIStream.h"

using namespace std;

JoinType &operator<<(JoinType &a, const string &b){
 a=
  "natural"==b?JOIN_NATURAL:
  "cross"==b?JOIN_CROSS:
  "inner"==b?JOIN_INNER:
  "outer"==b?JOIN_OUTER:
  JOIN_UNDEF;
 return a;
}

/// Standard constuctor. Initializes attributes.
JoinAlgorithm::JoinAlgorithm(
 CsvIStream * const a,
 CsvIStream * const b,
 const JoinType &c,
 const pair<ColIvalV,ColIvalV> &d
):left_stream(a),right_stream(b),type(c) {
 const CsvRow &lhead=a->get_head();
 const CsvRow &rhead=b->get_head();
 join_fields=derive_join_fields(lhead,rhead,type,d);
}

/// Static method that calculates column sequences from column intervals.
FieldPV JoinAlgorithm::derive_join_fields(
 const CsvRow &lhead,
 const CsvRow &rhead,
 const JoinType t,
 const pair<ColIvalV,ColIvalV> &col_pair
) const {
 FieldPV retv;
 pair<ColIvalV,ColIvalV> join_columns=col_pair;
 if (t == JOIN_CROSS) { // cross join overrides join column setting
  join_columns=make_pair(ColIvalV("",0u),ColIvalV("",0u));
 }
 if (t == JOIN_NATURAL) {
  for (unsigned int i=0; i<rhead.size(); ++i) {
   auto it=find(lhead.begin(),lhead.end(),rhead.at(i));
   if (it!=lhead.end()){
    retv.push_back({it-lhead.begin(),i});
   }
  }
 } else {
  auto lf=join_columns.first.extract_ival(lhead.size());
  auto rf=join_columns.second.extract_ival(rhead.size());
  transform(
   lf.begin(),lf.end(),
   rf.begin(),back_inserter(retv),
   [](const ColID &a, const ColID &b){return make_pair(a,b);});
 }
 return retv;
}
