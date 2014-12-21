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
#ifndef _LOG_CONFIG_H_
#define _LOG_CONFIG_H_
#include <iostream>

/// Logging levels. Order matters.
typedef enum {
 FATAL = 0,	///< Very serious problem, when execution has to be terminated. Highest level.
 ERROR = 1,	///< There is a problem that cannot be resolved.
 WARN = 2,	///< Problem is detected but it can be resolved.
 INFO = 3,	///< General information, e.g., entering function.
 DEBUG = 4,	///< Detailed information about variables.
 TRACE = 5	///< Very detailed information. Rarely used. Lowest level.
} LogLevel;

/// Stream output function for LogLevel.
std::ostream &operator<<(std::ostream&, const LogLevel&);

/// Configuration class for logging.
class LogConfig {
 std::ostream &out;	///< Where to put the log messages
 LogLevel min_level;	///< What is the "lowest" level of logging.
 bool file_info;	///< Whether to log the place of log (file:line) or not.
public:
 LogConfig(std::ostream &a=std::cerr, LogLevel b=WARN, bool c=false);
 bool is_enabled(LogLevel a) const;
 bool do_file_info() const;
 std::ostream &get_out() const;
};

#endif
