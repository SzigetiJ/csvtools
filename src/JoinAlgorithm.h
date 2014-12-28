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
#ifndef _JOIN_ALGORITHM_H_
#define _JOIN_ALGORITHM_H_
#include "CsvStream.h"
#include "CsvClasses.h"
#include "ColTypes.h"
#include <string>

/// Supported types of join.
typedef enum {JOIN_NATURAL, JOIN_CROSS, JOIN_INNER, JOIN_OUTER, JOIN_UNDEF} JoinType;

/// Join column pairs.
typedef std::vector<std::pair<ColID,ColID> > FieldPV;

/// JoinType parser.
JoinType &operator<<(JoinType&, const std::string&);

/// Abstract JoinAlgorithm parent class.
class JoinAlgorithm : public CsvStream {
protected:
 CsvIStream * const left_stream;	///< Left table given as CsvIStream.
 CsvIStream * const right_stream;	///< Right table given as CsvIStream.
 JoinType type;	///< Type of the join. 
 FieldPV join_fields;	///< Column pairs that have to match.

private:
 FieldPV derive_join_fields(const CsvRow&, const CsvRow&, const JoinType, const std::pair<ColIvalV,ColIvalV>&) const;

public:
 JoinAlgorithm(CsvIStream * const, CsvIStream * const, const JoinType&, const std::pair<ColIvalV,ColIvalV>&);
 virtual ~JoinAlgorithm() = default;
};

#endif
