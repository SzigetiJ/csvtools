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
#ifndef _CSVROW_H_
#define _CSVROW_H_
#include <string>
#include <vector>
#include "CsvCell.h"
#include "ColTypes.h"

/// Class representing a line (row) of a csv file.
class CsvRow : public std::vector<CsvCell>{
public:
 bool parse(std::istream&, const Delimiters&);
 void print(std::ostream&, const Delimiters&, const EscapeStrategy&) const;
 CsvRow get_fields(const FieldV&) const;
 CsvRow &operator+=(const CsvRow&);
};

/// Pure virtual parent class for row comparing functors.
class CsvRowCompare {
public:
 virtual bool operator()(const CsvRow&, const CsvRow&) const=0;
};

/// Wrapper class for row comparing functors.
/// This wrapper class must be used in functions.
class CsvRowCompareWrapper
{
private:
    CsvRowCompare &cmp;
public:
    CsvRowCompareWrapper(CsvRowCompare&);
    bool operator()(const CsvRow&, const CsvRow&) const;
};

#endif
