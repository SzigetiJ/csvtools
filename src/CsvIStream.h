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
#ifndef _CSVISTREAM_H_
#define _CSVISTREAM_H_

#include <map>
#include <iostream>
#include "CsvClasses.h"
#include "CsvStream.h"
#include "CsvRow.h"

/// Csv input stream reader class.
/// Supports sequential row reading.
/// Rows can be read either as vectors or as column name => cell maps.
/// Does not support copy constructor / destructor
/// (i.e., stream must be closed explicitely after usage).
class CsvIStream : public CsvStream {
protected:
 std::istream &is;	///< Input stream as source of the csv file.
 Delimiters delims; ///< Delimiters.
 int record_cnt;	///< Record counter (increased by next()).
 mutable bool curr_m_u2d;	///< (not) dirty flag of curr_m.

 CsvRow head;	///< Conventional csv head (set by the constructor).
 mutable std::map<std::string,CsvCell> curr_m;	///< Current line, fields mapped by column name.
 void update_curr_m() const;	///< Internal function to update curr_m.
 CsvRow curr_row;	///< Raw current row modified by next().
public:
 CsvIStream(std::istream&, const Delimiters&);
 std::map<std::string,CsvCell> get_current_line_m() const;
 CsvRow get_head() const;
 CsvRow get_current_line() const;
 int get_record_cnt() const;
 bool next();
};

#endif
