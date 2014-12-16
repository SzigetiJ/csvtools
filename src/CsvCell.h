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
#ifndef _CSVCELL_H_
#define _CSVCELL_H_
#include <iostream>
/// Default record separator character
#define CHR_RS '\n'
/// Pseudo end-of-line character (for return value)
#define CHR_EOF '\0'
/// Default quote character
#define CHR_QUOTE '"'

class CsvCell {
/// the stored data
 std::string dat;
/// whether the data type is numeric (no quote) or text (quote). This influences the comparison of the data and also the output of the data.
 bool quote:1;
public:
 char parse(std::istream &);
 CsvCell(const std::string &, bool);
 CsvCell();
 std::string get_dat() const;
 void set_escaped(bool);
 void to_decimal();
 std::string get_escaped() const;
 bool operator<(const CsvCell&) const;
 bool operator==(const CsvCell&) const;
 friend std::ostream &operator<<(std::ostream &, const CsvCell&);
};
#endif
