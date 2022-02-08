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
#include <iostream>
#include "DefaultCommandLine.h"
#include "../config.h"

using namespace std;

const Option default_option_a[]={
{"h","help",0,IGNORE,"Display help."},
{"V","version",0,IGNORE,"Display version."},
{"q","quiet",0,IGNORE,"Quiet output (display log messages of ERROR level or higher)."},
{"qq","very-quiet",0,IGNORE,"Quiet output (display log messages only of FATAL level)."},
{"v","verbose",0,IGNORE,"Verbose output (display log messages of INFO level or higher)."},
{"vv","more-verbose",0,IGNORE,"Verbose output (display log messages of DEBUG level or higher)."},
{"vvv","most-verbose",0,IGNORE,"Verbose output (display log messages of TRACE level or higher)."},
{"D","debug",0,IGNORE,"Include debug information (file:line) in log."}
};
const int default_option_n = sizeof(default_option_a)/sizeof(Option);

void DefaultCommandLine::print_usage() const {
 cout<<"Common usage: "<<endl;
 cout<<progname<<" "<<usage<<endl<<endl;
}

void DefaultCommandLine::print_help() const {
 cout<<"Available options: "<<endl;
 for (auto option : option_s) {
 cout<<"\t"<<option<<endl;
 }
}

void DefaultCommandLine::print_description() const {
 cout<<description<<endl;
}

void DefaultCommandLine::print_version() const {
 cout<<PACKAGE_STRING<<endl;
}

template<typename Iterator1, typename Iterator2>
set<Option> set_merge(Iterator1 begin1, Iterator1 end1, Iterator2 begin2, Iterator2 end2) {
 set<Option> retv(begin1,end1);
 retv.insert(begin2,end2);
 return retv;
}

DefaultCommandLine::DefaultCommandLine(const string &desc, const string &use, const set<Option> &opt):
 CommandLine(set_merge(opt.begin(),opt.end(),default_option_a,default_option_a+default_option_n)),
 description(desc),
 usage(use){}

void DefaultCommandLine::set_global_logger() {
 global_logger=get_log_config();
}

CommandLineExecuteResponse DefaultCommandLine::execute(int argc, const char *argv[]) {
 if (parse(argc, argv)) {
  print_help();
  return CMDLINE_ERROR;
 }
 DefaultCommandLine::set_global_logger();
 if (print_if_needed()) {
  return CMDLINE_BREAK;
 }
 if (process()) {
  print_help();
  return CMDLINE_ERROR;
 }
 return CMDLINE_OK;
}

LogConfig DefaultCommandLine::get_log_config() const {
 LogLevel level=
 is_set_flag("vvv")?TRACE:
 is_set_flag("vv")?DEBUG:
 is_set_flag("v")?INFO:
 is_set_flag("q")?ERROR:
 is_set_flag("qq")?FATAL:
 WARN;
 bool debug=is_set_flag("D");
 return LogConfig(&cerr,level,debug);
}

int DefaultCommandLine::print_if_needed() const {
 if (is_set_longname("help")) {
  print_description();
  print_usage();
  print_help();
  return 1;
 }
 if (is_set_longname("version")) {
  print_version();
  print_description();
  return 2;
 }
 return 0;
}
