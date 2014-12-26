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
#ifndef _DEFAULTCOMMANDLINE_H_
#define _DEFAULTCOMMANDLINE_H_
#include <string>
#include "CommandLine.h"
#include "log.h"

/// Default CommandLine class. It defines commonly used options, e.g., verbosity level, help, version.
/// It has getter method on LogConfig so we can get the logging configuration.
/// Furthermore, it has printer methods for displaying usage, help, etc. messages.
class DefaultCommandLine : public CommandLine {
 std::string description;	///< Brief description about the tool.
 std::string usage;	///< Short usage tip.
public:
 DefaultCommandLine(const std::string&, const std::string&, const std::set<Option>&);
 int process();
 LogConfig get_log_config() const;
 int print_if_needed() const;
 void print_usage() const;
 void print_help() const;
 void print_description() const;
 void print_version() const;
};
#endif
