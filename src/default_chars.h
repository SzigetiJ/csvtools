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
#ifndef _DEFAULT_CHARS_H_
#define _DEFAULT_CHARS_H_

/// Default record separator character
#define CHR_RS '\n'
/// Pseudo end-of-line character (for return value)
#define CHR_EOF '\0'
/// Default quote character
#define CHR_QUOTE '"'

#ifndef DEFAULT_CHR_IFS
#define DEFAULT_CHR_IFS ','
#endif
#ifndef DEFAULT_CHR_OFS
#define DEFAULT_CHR_OFS ','
#endif

#endif

