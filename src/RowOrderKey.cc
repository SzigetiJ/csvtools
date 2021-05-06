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
#include "RowOrderKey.h"
#include "CsvCell.h"
#include "CsvRow.h"

using namespace std;

template<typename T>
int signum(const T &x) {
 return (T(0) < x) - (x < T(0));
}

/// Standard Constructor. Initializes protected attributes.
RowOrderKey::RowOrderKey(const ColID &a, bool b, bool c, bool d):colid(a),desc(b),numeric(c),high_empty(d){}

/// Compares two rows.
/// @param a Left hand side agrument of comparison.
/// @param b Right hand side agrument of comparison.
/// @return >0: a precedes b, <0: b precedes a, 0: a equals to b according to this RowOrderKey.
int RowOrderKey::compare(const CsvRow &a, const CsvRow &b) {
 const CsvCell &a_cell=a[colid];
 const CsvCell &b_cell=b[colid];
 if (a_cell.get_dat().empty() && b_cell.get_dat().empty()) {
  return 0;
 }
 if (!a_cell.get_dat().empty() && !b_cell.get_dat().empty()) {
  return (numeric?signum(stof(a_cell.get_dat())-stof(b_cell.get_dat())):a_cell.get_dat().compare(b_cell.get_dat()))*(desc?-1:1);
 }
 return (a_cell.get_dat().empty()?-1:1)*(high_empty?-1:1);
}
