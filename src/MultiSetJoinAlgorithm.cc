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
#include "MultiSetJoinAlgorithm.h"
#include "CsvRow.h"
#include "CsvIStream.h"

using namespace std;

/// Standard constructor.
CsvRowForJoinLess::CsvRowForJoinLess(const FieldPV &a):join_fields(a){}

/// This comparison checks only those fields that are selected for the join.
bool CsvRowForJoinLess::operator()(const CsvRowForJoin &a, const CsvRowForJoin &b){
 for (unsigned int i=0;i<join_fields.size();++i) {
  pair<ColID,ColID> actual=join_fields[i];
  const CsvCell &a_cell=a.first[(a.second?actual.first:actual.second)];
  const CsvCell &b_cell=b.first[(b.second?actual.first:actual.second)];
  if (a_cell.get_dat() < b_cell.get_dat()) return true;
  if (b_cell.get_dat() < a_cell.get_dat()) return false;
 }
 return false;
}

/// Standard Constuctor. Initializes attributes.
MultiSetJoinAlgorithm::MultiSetJoinAlgorithm(
 CsvIStream * const a, CsvIStream * const b,
 const JoinType &c, const std::pair<ColIvalV,ColIvalV> &d
):JoinAlgorithm(a,b,c,d),right_rows(join_fields),row_cnt(0){
  while(right_stream->next()) {
   right_rows.insert({right_stream->get_current_line(),false});
  }
  if (type==JOIN_OUTER) {	// insert pseudo (null) row
   CsvRow empty_row;
   empty_row.resize(right_stream->get_head().size());
   right_rows.insert(right_rows.begin(),{empty_row,false});
  }
  current_b=current_e=current_it=right_rows.end();
}

/// The header of the join is the concatenation of the two headers.
CsvRow MultiSetJoinAlgorithm::get_head() const {
 CsvRow retv=left_stream->get_head();
 retv+=right_stream->get_head();
 return retv;
}

/// The current line is the concatenation of the two current lines.
CsvRow MultiSetJoinAlgorithm::get_current_line() const {
 CsvRow retv=left_stream->get_current_line();
 return retv+=current_it->first;
}

int MultiSetJoinAlgorithm::get_record_cnt() const {return row_cnt;}

bool MultiSetJoinAlgorithm::next(){
 if (current_it==current_e) {	// before first line (initialization)
  if (type==JOIN_NATURAL && join_fields.empty()){
   return false;
  }
 } else {	// get next element on right table subset
  ++current_it;
  ++row_cnt;
 }
 if (current_it!=current_e) {	// next element is valid on right table subset
  return true;
 } else {	// get next element on left table and find matching right table subset
  do {
   bool lnext=left_stream->next();
   if (!lnext) {
    return false;
   }
   current_b=current_it=right_rows.lower_bound({left_stream->get_current_line(),true});
   current_e=right_rows.upper_bound({left_stream->get_current_line(),true});
  } while (type!=JOIN_OUTER && (current_it==current_e || current_b==current_e));
  ++row_cnt;
  if (type==JOIN_OUTER && current_b==current_e) {
   current_it=current_b=current_e=right_rows.begin();
   ++current_e;
  }
 }
 return true;
}
