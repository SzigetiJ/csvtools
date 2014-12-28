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
#ifndef _CSVSTREAM_H_
#define _CSVSTREAM_H_
#include "CsvClasses.h"

/// CsvStream interface with base virtual methods.
class CsvStream {
public:
 virtual CsvRow get_head() const=0;	///< Get csv header row.
 virtual CsvRow get_current_line() const=0;	///< Get the actual row. Call next() before it!
 virtual int get_record_cnt() const=0;	///< Get current line number, starting with 0.
 virtual bool next()=0;	///< Jumps to next row if possible /false if not possible/.
 virtual ~CsvStream() = default;	///< Virtual destructor.
};

#endif
