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
#include "ColTypes.h"
#include "RowFunc.h"
#include <iostream>

/// Projection and selection can be performed row by row as stream filtering.
/// Stream filtering does not require loading the whole csv file into memory
/// and multiple filters can be pipelined.
class CsvPipe {
/// Sequence of projected columns.
 ColIvalV proj_v;
/// Set of filter conditions.
 RowFilterV filter_v;

public:
 CsvPipe &set_projection(const ColIvalV&);
 CsvPipe &set_filter(const RowFilterV&);
 const CsvPipe &process(std::istream &xin, std::ostream &xout, const Delimiters &delims=Delimiters(), const EscapeStrategy &strat=ESC_PRESERVE) const;
};

