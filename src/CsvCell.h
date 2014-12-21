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

/// Field escaping strategy.
typedef enum {
 ESC_ALL,	///< Every field must be escaped.
 ESC_PRESERVE,	///< Exising escaping must be preserved.
 ESC_RESOLVE,	///< If input escaping was requisite but output escaping is not needed than do not escape the field.
 ESC_REMOVE,	///< Do not escape fields if not required.
 ESC_UNDEF	///< Undefined value (denotes error).
} EscapeStrategy;


class CsvCell {
/// Cell escaping strategy.
 static EscapeStrategy esc_strat;
/// The stored data.
 std::string dat;
/// Whether the data type is numeric (no quote) or text (quote). This influences the comparison of the data and also the output of the data.
 bool quote:1;

 bool requires_escape_at(bool) const;
public:
 static EscapeStrategy parse_esc_strat(const std::string&);
 static void set_esc_strat(EscapeStrategy);

 char parse(std::istream &);
 CsvCell(const std::string &, bool);
 CsvCell();
 std::string get_dat() const;
 void set_escaped(bool);
 void to_decimal();
 bool requires_escape() const;
 std::string get_escaped() const;
 bool operator<(const CsvCell&) const;
 bool operator==(const CsvCell&) const;
 friend std::ostream &operator<<(std::ostream &, const CsvCell&);
};
#endif
