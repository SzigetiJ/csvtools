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
#ifndef _LOG_H_
#define _LOG_H_

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
inline std::ostream &operator<<(std::ostream &a, const LogLevel &b){
	return a<<(b==FATAL?"FATAL":
	b==ERROR?"ERROR":
	b==WARN?"WARN":
	b==INFO?"INFO":
	b==DEBUG?"DEBUG":
	b==TRACE?"TRACE":
	"UNKNOWN");
}

/// Configuration class for logging.
class LogConfig {
 std::ostream &out;	///< Where to put the log messages
 LogLevel min_level;	///< What is the "lowest" level of logging.
 bool file_info;	///< Whether to log the place of log (file:line) or not.
public:
 LogConfig(std::ostream &a=std::cerr, LogLevel b=WARN, bool c=false):out(a),min_level(b),file_info(c){};	///< Standard constructor setting all of the private members.
 bool is_enabled(LogLevel a) const {return a<=min_level;}	///< Returns whether a givel LogLevel needs to be logged or not.
 bool do_file_info() const {return file_info;}	///< Getter for attribute file_info.
 std::ostream &get_out() const {return out;}	///< Getter for attribute out.
};

// Logging macros
#define FATAL(CFG,X) LOG_AT_LEVEL(CFG,FATAL,X)
#define ERROR(CFG,X) LOG_AT_LEVEL(CFG,ERROR,X)
#define WARN(CFG,X) LOG_AT_LEVEL(CFG,WARN,X)
#define INFO(CFG,X) LOG_AT_LEVEL(CFG,INFO,X)
#define DEBUG(CFG,X) LOG_AT_LEVEL(CFG,DEBUG,X)

#define LOG_AT_LEVEL(CFG,LEVEL,X) if (CFG.is_enabled(LEVEL)) {LOG(CFG.do_file_info(), CFG.get_out(), LEVEL, X);}
#define LOG(FILEINFO, STREAM, LEVEL, MSG) ((FILEINFO?STREAM << "["<< LEVEL <<":" << __FILE__ << ":" << __LINE__ << "] ":STREAM << LEVEL << ": ") << MSG << std::endl).flush()

#endif
