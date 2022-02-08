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
#ifndef _ROWFUNC_H_
#define _ROWFUNC_H_
#include "ColTypes.h"
#include "CsvClasses.h"
#include <string>
#include <functional>

/// bool function with two string parameters (binary predicate)
typedef std::function<bool (const std::string&, const std::string&)> OpFun;

/// row filter structure naming the binary predicate and the two parameters of the predicate.
/// The first parameter defines a list of fields, the second parameter is a string invariant.
class RowFilter : public std::pair<OpFun,std::pair<ColIvalV, std::string> > {
public:
 RowFilter(const std::pair<OpFun,std::pair<ColIvalV, std::string> >&);
 bool row_matches(const CsvRow&) const;
};

typedef std::vector<RowFilter> RowFilterV;

#endif
