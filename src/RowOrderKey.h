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
#ifndef _ROW_ORDER_KEY_H_
#define _ROW_ORDER_KEY_H_

#include "CsvClasses.h"
#include "ColTypes.h"

template<typename T>
int signum(const T&);

class RowOrderKey {
protected:
 ColID colid;
 bool desc;
 bool numeric;
 bool high_empty;
public:
 RowOrderKey(const ColID&, bool, bool, bool);
 int compare(const CsvRow&, const CsvRow&);
};

#endif
