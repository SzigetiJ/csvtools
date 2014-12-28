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
#include "CsvIStream.h"
#include <utility>

using namespace std;

void CsvIStream::update_curr_m() const {
 if (!curr_m_u2d){
  curr_m.clear();
  for (ColID i=0; i<head.size();++i){
   curr_m.insert(make_pair(head[i].get_dat(),curr_row[i]));
  }
 }
};

/// Standard constuctor.
CsvIStream::CsvIStream(std::istream &a, const Delimiters &b):is(a),delims(b),record_cnt(0),curr_m_u2d(false){
 head.parse(is,delims);
};

/// Getter method of curr_m. If curr_m is dirty, first updated curr_m.
std::map<std::string,CsvCell> CsvIStream::get_current_line_m() const {
 update_curr_m();
 return curr_m;
};

/// Simple getter method of head.
CsvRow CsvIStream::get_head() const{
 return head;
};

/// Simple getter method of curr_row.
CsvRow CsvIStream::get_current_line() const {
 return curr_row;
};

/// Simple getter method of record_cnt.
int CsvIStream::get_record_cnt() const {
 return record_cnt;
};

/// Steps to the next line if possible.
/// \return the next line could be read (otherwise the current data is not updated).
bool CsvIStream::next() {
 CsvRow rx;
 bool retv;
 if ((retv=rx.parse(is,delims))){
  curr_row=rx;
  curr_m_u2d=false;
  ++record_cnt;
 }
 return retv;
};

