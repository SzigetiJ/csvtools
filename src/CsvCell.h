/*
 *  Copyright (C) 2014 - 2023 SZIGETI János <szigeti at pilar dot hu>
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
#include "Delimiters.h"
#include "log.h"

class CsvCell {
/// The stored data.
 std::string dat;
/// Whether the data type is numeric (no quote) or text (quote). Has impact on comparison of the data and also the output of the data.
 bool quote:1;

 bool requires_escape_at(bool, const Delimiters&) const;
public:
 char parse(std::istream &, const Delimiters &);
 CsvCell(const std::string &, bool);
 CsvCell(std::string &&, bool);
 CsvCell(const std::string &);
 CsvCell(std::string &&);
 CsvCell(const char *);
 CsvCell(int);
 CsvCell();
 const std::string &get_dat() const;
 bool is_quoted() const;
 void set_escaped(bool);
 void to_decimal();
 bool requires_escape_for_strategy(const EscapeStrategy&, const Delimiters&) const;
 std::string get_escaped(const Delimiters&) const;
 bool operator<(const CsvCell&) const;
 bool operator==(const CsvCell&) const;
 std::ostream &print(std::ostream &, const Delimiters&, const EscapeStrategy&) const;
};

#endif
