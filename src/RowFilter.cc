/*
 *  Copyright (C) 2014 -- 2022 SZIGETI János <szigeti at pilar dot hu>
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
#include "RowFilter.h"
#include "CsvRow.h"
#include <sstream>

using namespace std;

RowFilter::RowFilter(const pair<OpFun,pair<ColIvalV, string> > &a):pair<OpFun,pair<ColIvalV, string> >(a){}

bool RowFilter::row_matches(const CsvRow &row) const {
   const FieldV &xfv=second.first.extract_ival(row.size());
   CsvRow dfline_csv=row.get_fields(xfv);
   stringstream ss;
   string dfline_s;
   dfline_csv.print(ss,Delimiters(),ESC_PRESERVE);
   dfline_s=ss.str();
   return first(dfline_s,second.second);
}
