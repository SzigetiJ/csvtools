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
#ifndef _COLTYPES_H_
#define _COLTYPES_H_
#include <string>
#include <vector>

constexpr unsigned int COLID_UNDEF = 65535U;
extern const char *colivalv_expr_help_str;

/// Column ID.
/// The first column has ColID=0, etc.
/// The undefined column (coded by the '-' input key) has ColID=COLID_UNDEF.
typedef unsigned int ColID;
typedef std::pair<bool, ColID> ColRef;  ///< first: is_reverse (right-to-left), second: column index

/// Column interval defined by the first and the last (!!!inclusive!!!) column.
class ColIval : public std::pair<ColRef, ColRef> {
public:
 ColIval() = default;
 ColIval(const ColRef&); // WARNING: many types are implicitly converted to unsigned int
 ColIval(const ColRef&, const ColRef&);
 int parse(const char *, size_t);
};

/// Array of columns
typedef std::vector<ColID> FieldV;

/// Array of intervals
class ColIvalV : public std::vector<ColIval>{
public:
 ColIvalV(bool); // WARNING: many types are implicitly converted to bool
 int parse(const char*, size_t len);
 int parse(const char*);
 FieldV extract_ival(int) const;
};

#endif
