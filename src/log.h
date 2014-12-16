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

typedef struct _LogLevel {
 const char * level;
 bool enabled;
 std::ostream &stream;
 _LogLevel(const char *a, bool b, std::ostream &c):
  level(a),enabled(b),stream(c){}; 
} LogLevel;

class LogConfig {
public:
 LogLevel fatal;
 LogLevel error;
 LogLevel warn;
 LogLevel info;
 LogLevel debug;
 LogConfig():
  fatal("FATAL",true,std::cerr),
  error("ERROR",true,std::cerr),
  warn("WARN",true,std::cerr),
  info("INFO",true,std::cerr),
  debug("DEBUG",false,std::cerr){};
};

extern LogConfig logger;

#define FATAL(CFG,X) LOG_AT_LEVEL(CFG.fatal,X)
#define ERROR(CFG,X) LOG_AT_LEVEL(CFG.error,X)
#define WARN(CFG,X) LOG_AT_LEVEL(CFG.warn,X)
#define INFO(CFG,X) LOG_AT_LEVEL(CFG.info,X)
#define DEBUG(CFG,X) LOG_AT_LEVEL(CFG.debug,X)

#define LOG_AT_LEVEL(LEVEL,X) LOG(LEVEL.enabled, LEVEL.stream, LEVEL.level, X)
#define LOG(ENABLED, STREAM, LEVEL, MSG) if (ENABLED) {STREAM << "["<< LEVEL <<":" << __FILE__ << ":" << __LINE__ << "] " << MSG << "\n"; STREAM.flush();}

#endif
