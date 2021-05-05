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

/// Dummy tool for testing CsvPipe projecting every column and selecting every row.
#include <iostream>
#include "CsvPipe.h"
#include "CsvRow.h"
#include "Delimiters.h"
#include "DefaultCommandLine.h"
#include "log.h"

using namespace std;

// Piping specific options.
const Option conv_option_a[]={
{"ifs","input-field-separator",1,OVERRIDE,"Define input field separator character."},
{"ofs","output-field-separator",1,OVERRIDE,"Define output field separator character."},
{"esc","escape-strategy",1,OVERRIDE,"Set (un)escape strategy {all|preserve|resolve|remove}. Default: preserve."}
};
const int conv_option_n = sizeof(conv_option_a)/sizeof(Option);

/// Extension to DefaultCommandLine: PipeCommandLine can change default field separator characters.
class PipeCommandLine : public DefaultCommandLine {
 Delimiters delims;
 EscapeStrategy strat=ESC_PRESERVE;
public:
 PipeCommandLine(const string &desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(conv_option_a,conv_option_a+conv_option_n)){};
 /// Checks whether ifs / ofs is given and sets CsvRow static attributes.
 int process() {
  if (is_set_flag("ifs"))
   delims.set(IFS,get_arg_for_flag("ifs")[0]);
  if (is_set_flag("ofs"))
   delims.set(OFS,get_arg_for_flag("ofs")[0]);
  if (is_set_flag("esc")) {
   if ((strat<<string(get_arg_for_flag("esc")))==ESC_UNDEF) {
    ERROR(logger,"Invalid escape-strategy.");
    return -1;
   }
  }
  return 0;
 }
 Delimiters get_delims() const {return delims;}
 EscapeStrategy get_strat() const {return strat;}
};

const string DESCRIPTION="Pipes csv from stdin to stdout and applies specified conversions on rows and fields. Field separator character may be overridden.\n";
const string USAGE="[-ifs {chr}] [-ofs {chr}] [-esc {strategy}]\n";


int main(int argc, char **argv) {
 PipeCommandLine cmdline=PipeCommandLine(DESCRIPTION,USAGE);
 CommandLineExecuteResponse resp=cmdline.execute(argc, argv);
 if (resp!=CMDLINE_OK) {
  return resp;
 }
 CsvPipe()
 .process(cin,cout,cmdline.get_delims(),cmdline.get_strat());
}

