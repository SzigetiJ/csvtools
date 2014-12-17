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

/// Dummy tool for testing CsvPipe projecting every column and selecting every row.
#include <iostream>
#include "CsvPipe.h"
#include "log.h"

using namespace std;

int main() {
 CsvPipe()
 .set_projection(ColIvalV(1,ColIval(COLID_UNDEF,COLID_UNDEF)))
 .process(cin,cout);
}

