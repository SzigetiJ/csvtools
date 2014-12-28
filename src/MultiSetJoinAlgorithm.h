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
#ifndef _MULTISET_JOIN_ALGORITHM_H_
#define _MULTISET_JOIN_ALGORITHM_H_
#include <set>
#include "JoinAlgorithm.h"

/// Extension to CsvRow: the bool flag denotes whether the CsvRow is member of the left (true) or the right (false) table.
typedef std::pair<CsvRow, bool> CsvRowForJoin;

/// Comparison functor for CsvRowForJoin objects.
class CsvRowForJoinLess : public std::less<CsvRowForJoin> {
 FieldPV join_fields;	///< column pairs used in join.
public:
 CsvRowForJoinLess(const FieldPV&);	///< Standard constructor.
 bool operator()(const CsvRowForJoin&, const CsvRowForJoin&);
};

/// This container stores Rows sorted by join columns.
typedef std::multiset<CsvRowForJoin,CsvRowForJoinLess> CsvRowForJoinSet;

/// Multiset based JoinAlgorithm implementation.
/// Right table is completely read from the input stream and stored in
/// multiset container.
/// Left table is processed as stream and result preserves row order of left table.
class MultiSetJoinAlgorithm : public JoinAlgorithm {
protected:
 CsvRowForJoinSet right_rows;	///< Container of rows in right table.
 int row_cnt;	///< Output record counter.
 CsvRowForJoinSet::const_iterator current_b;	///< Interval begin of matching rows.
 CsvRowForJoinSet::const_iterator current_e;	///< Interval end of matching rows.
 CsvRowForJoinSet::const_iterator current_it;	///< Pointer to current matching row in interval.
public:
 MultiSetJoinAlgorithm(CsvIStream * const, CsvIStream * const, const JoinType&, const std::pair<ColIvalV,ColIvalV>&);
 CsvRow get_head() const;
 CsvRow get_current_line() const;
 int get_record_cnt() const;
 bool next();
 ~MultiSetJoinAlgorithm() = default;
};

#endif
