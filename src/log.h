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
#include "LogConfig.h"

// Logging macros
#define FATAL(CFG,X) LOG_AT_LEVEL(CFG,FATAL,X)
#define ERROR(CFG,X) LOG_AT_LEVEL(CFG,ERROR,X)
#define WARN(CFG,X) LOG_AT_LEVEL(CFG,WARN,X)
#define INFO(CFG,X) LOG_AT_LEVEL(CFG,INFO,X)
#define DEBUG(CFG,X) LOG_AT_LEVEL(CFG,DEBUG,X)

#define LOG_AT_LEVEL(CFG,LEVEL,X) if (CFG.is_enabled(LEVEL)) {LOG(CFG.do_file_info(), CFG.get_out(), LEVEL, X);}
#define LOG(FILEINFO, STREAM, LEVEL, MSG) ((FILEINFO?STREAM << "["<< LEVEL <<":" << __FILE__ << ":" << __LINE__ << "] ":STREAM << LEVEL << ": ") << MSG << std::endl).flush()

#endif
