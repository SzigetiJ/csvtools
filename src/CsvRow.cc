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
#include "CsvRow.h"
#include "CsvCell.h"

using namespace std;

/// Projection
/// \param a cells to be selected (order sensitive)
/// \return row of selected cells
CsvRow CsvRow::get_fields(const FieldV &a) const {
 CsvRow retv;
 for (ColID i=0;i<a.size();++i){
  retv.push_back(at(a[i]));
 }
 return retv;
};

/// Merge. Pastes a row to the end of the current row.
/// \param a row to paste.
/// \return reference to the current, modified row.
CsvRow &CsvRow::operator+=(const CsvRow &a) {
 if (&a!=this)
  insert(end(),a.begin(),a.end());
 else {
  ColID asize=a.size();
  for (ColID i=0;i<asize;++i)
   push_back(a[i]);
 }
 return *this;
}

/// Print.
/// Standard output function of CsvRow objects.
void CsvRow::print(ostream &a, const Delimiters &delims, const EscapeStrategy &strat){
 for (ColID i=0;i<size();++i) {
  if (i!=0)
   a<<delims.get(OFS);
  at(i).print(a,delims,strat);
 }
}

/// Parses the input stream and builds the CsvRow object.
/// \param a input stream to parse
/// \return input has more lines (no eof has been encountered)
bool CsvRow::parse(istream &a, const Delimiters &delims){
 char c_end;
  CsvCell cx;
 do {
  c_end=cx.parse(a, delims);
  push_back(cx);
 } while (c_end==delims.get(IFS));
 return (c_end!=delims.get(EoF));
}


/// Standard constructor.
/// Init. private member.
CsvRowCompareWrapper::CsvRowCompareWrapper(CsvRowCompare &cmp) : cmp(cmp) {
}
/// Comparison operator.
/// Calls wrapped member.
bool CsvRowCompareWrapper::operator()(const CsvRow &a, const CsvRow &b) const {
 return cmp(a, b); 
}

