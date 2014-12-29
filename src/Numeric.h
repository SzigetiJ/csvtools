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
#ifndef _NUMERIC_H_
#define _NUMERIC_H_
#include <string>

/// Dummy class representing numeric values.
/// Numeric values must be given in [-+][0-9]*\.?[0-9]* format.
/// TODO check value overflow (constr., op+=).
class Numeric {
 int val;	///< Numeric value.
 size_t prec;	///< Precision.
 int digits_to_num(int);	///< f(x)=10^x function for x>=0.
public:
 Numeric(const std::string&);
 std::string to_string() const;
 Numeric &operator+=(const std::string&);
 bool operator<(const Numeric&) const;
};

#endif
