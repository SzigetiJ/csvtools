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
#include "Delimiters.h"
#include "CsvPipe.h"
#include "CsvCell.h"
#include "CsvRow.h"

using namespace std;

CsvPipe &CsvPipe::set_projection(const ColIvalV &a){
 proj_v=a;
 return *this;
};

CsvPipe &CsvPipe::set_filter(const RowFilterV &a){
 filter_v=a;
 return *this;
};

const CsvPipe &CsvPipe::process(istream &xin, ostream &xout) const {
 // filter each row except head
 // project each row
 // get colnum from head and transform v<ColIval> into FieldV
 CsvRow head;
 if (!head.parse(xin))
  return *this;
 FieldV xproj_v=extract_ival(proj_v,head.size());
 xout<<head.get_fields(xproj_v)<<Delimiters::get(ORS);

 CsvRow rx;
 while (rx.parse(xin)){
  if (row_matches(rx,filter_v)){
   xout<<rx.get_fields(xproj_v)<<Delimiters::get(ORS);
  }
  rx.clear();
 }
 return *this;
};

