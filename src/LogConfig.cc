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
#include "LogConfig.h"

using namespace std;

/// Stream output function for LogLevel.
std::ostream &operator<<(std::ostream &a, const LogLevel &b){
	return a<<(b==FATAL?"FATAL":
	b==ERROR?"ERROR":
	b==WARN?"WARN":
	b==INFO?"INFO":
	b==DEBUG?"DEBUG":
	b==TRACE?"TRACE":
	"UNKNOWN");
}

LogConfig::LogConfig(std::ostream &a, LogLevel b, bool c):
 out(a),min_level(b),file_info(c){
};

/// Returns whether a givel LogLevel needs to be logged or not.
bool LogConfig::is_enabled(LogLevel a) const {
 return a<=min_level;
}

/// Getter method of attribute file_info.
bool LogConfig::do_file_info() const {
 return file_info;
}

/// Getter method of attribute out.
ostream &LogConfig::get_out() const {
 return out;
}

