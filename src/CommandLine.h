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
#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_
#include <cstring>
#include <set>
#include <vector>
#include <map>
#include <string>
#include "log.h"
#include "globals.h"

/// Possible responses of CommandLine.execute method.
typedef enum {
 CMDLINE_BREAK = 0,	///< Program must be terminated with exit status 0.
 CMDLINE_ERROR = 1,	///< Program must be terminated due to an error (1).
 CMDLINE_OK = 2	///< Everything is fine.
} CommandLineExecuteResponse;

/// Possible handling of options that are define multiple times.
typedef enum {
 IGNORE,	///< First in effect, others ignored.
 APPEND,	///< Keep each option in a list.
 OVERRIDE	///< Last is effect, others overridden.
} MultipleDefinition;

/// Commandline option definition. Each option can be set either with -{flag} or with --{longname}.
/// If the option requires argument(s) (arg_num>0),
/// they must be given in format -{flag} {arg1} {arg2} ... or --{longname}={arg1} {arg2}.
/// Usually options do not have more than one argument.
typedef struct _Option {
 const char *flag;	///< Short (1-2 char) representation of the option. Must not begin with '-' (DASH_CHR).
 const char *longname;	///< One-two word long representation of the option. Must not contain '=' (EQ_CHR).
 int arg_num;	///< Number of required arguments.
 MultipleDefinition multidef;	///< Option override rule. Currently override rule can be given only for a single option, there is no possibility to define override rule for a set of options (e.g., verbosity level).
 const char *help;	///< One sentence long help message.
 bool operator<(const _Option &b) const;
} Option;

std::ostream &operator<<(std::ostream&, const Option&);

/// Base CommandLine class. Provides container for available options with flag=>Option and longname=>Option lookup maps.
/// It is responsible for parsing command line and stores parsed options in option=>values map.
class CommandLine {
protected:
 const char *progname;	///< Name of the invoked program (0th command line argument).
 LogConfig logger;	///< CommandLine prints its messages (e.g., parse problems) via logger. However, there is a chicken-egg problem here, since the appropriate verbosity level is not defined prior to command line parsing. Thus CommandLine uses its own predefined logger.
 std::set<Option> option_s;	///< Set of available options.
 std::map<std::string, Option> flag_m;	///< Flag=>Option lookup map.
 std::map<std::string, Option> longname_m;	///< Longname=>Option lookup map.
 std::multimap<Option,std::vector<char*> > value_m;	///< Option=>given argument map. 
 bool parsed;	/// Indicating whether parsing is already done.
 int parse(int, char**);
 virtual int process() = 0;
public:
 CommandLine(const std::set<Option>&);
 virtual CommandLineExecuteResponse execute(int, char**) = 0;

 bool is_set_option(const Option&) const;
 bool is_set_flag(const char*) const;
 bool is_set_longname(const char*) const;

 std::vector<std::vector<char*> > get_values_for_option(const Option&) const;
 std::vector<std::vector<char*> > get_values_for_flag(const char*) const;
 std::vector<std::vector<char*> > get_values_for_longname(const char*) const;
};

#endif
