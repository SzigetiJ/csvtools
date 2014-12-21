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
#ifndef _DELIMITERS_H_
#define _DELIMITERS_H_
#include <iostream>

/// Field escaping strategy.
typedef enum {
 ESC_ALL,	///< Every field must be escaped.
 ESC_PRESERVE,	///< Exising escaping must be preserved.
 ESC_RESOLVE,	///< If input escaping was requisite but output escaping is not needed than do not escape the field.
 ESC_REMOVE,	///< Do not escape fields if not required.
 ESC_UNDEF	///< Undefined value (denotes error).
} EscapeStrategy;

EscapeStrategy &operator<<(EscapeStrategy&, const std::string&);

/// Enumeration of special characters.
typedef enum {IFS, OFS, IRS, ORS, IESC, OESC, EoF} Delimiter; 

/// Container of special characters.
class Delimiters {
 char ifs;	///< Input field separator character.
 char ofs;	///< Output field separator character.
 char irs;	///< Input record separator character.
 char ors;	///< Output record separator character.
 char iesc;	///< Input escape character.
 char oesc;	///< Output escape character.
 char eof;	///< End of file character.
public:
 Delimiters();
 void set(Delimiter, char);
 char get(Delimiter) const;
};

#endif
