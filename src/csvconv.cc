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
#include "CsvIStream.h"

using namespace std;

// Piping specific options.
const Option conv_option_a[]={
{"ifs","input-field-separator",1,OVERRIDE,"Define input field separator character."},
{"ofs","output-field-separator",1,OVERRIDE,"Define output field separator character."},
{"irs","input-record-separator",1,OVERRIDE,"Define input record separator character."},
{"ors","output-record-separator",1,OVERRIDE,"Define output record separator character."},
{"esc", "escape-strategy", 1, OVERRIDE, "Set (un)escape strategy. Default: preserve."},
{"cesc", "column-escape-strategy", 2, MultipleDefinition::APPEND, "Set (un)escape strategy for a set of columns. Arg1: column expression; arg2: escape strategy (default: inherit)."},
{"hesc", "header-escape-strategy", 1, MultipleDefinition::OVERRIDE, "Set (un)escape strategy for the header row (default: inherit)."}
};
const int conv_option_n = sizeof(conv_option_a)/sizeof(Option);

constexpr EscapeStrategy DEFAULT_ESC = EscapeStrategy::ESC_PRESERVE;

/// Extension to DefaultCommandLine: PipeCommandLine can change default field separator characters.
class PipeCommandLine : public DefaultCommandLine {
 Delimiters delims;
 EscapeStrategy strat = DEFAULT_ESC;
 vector<pair<ColIvalV, EscapeStrategy> > col_strat;
 EscapeStrategy head_strat = EscapeStrategy::ESC_INHERIT;
 vector<EscapeStrategy> head_esc_v;
 vector<EscapeStrategy> body_esc_v;

public:
 PipeCommandLine(const string &desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(conv_option_a,conv_option_a+conv_option_n)){};
 /// Checks whether ifs / ofs is given and sets CsvRow static attributes.
 int process() {
  if (is_set_flag("ifs"))
   delims.set(IFS,get_arg_for_flag("ifs")[0]);
  if (is_set_flag("ofs"))
   delims.set(OFS,get_arg_for_flag("ofs")[0]);
  if (is_set_flag("irs"))
   delims.set(IRS,get_arg_for_flag("irs")[0]);
  if (is_set_flag("ors"))
   delims.set(ORS,get_arg_for_flag("ors")[0]);
  if (is_set_flag("esc")) {
   auto esc_str = string(get_arg_for_flag("esc"));
   if ((strat << esc_str) == ESC_UNDEF) {
    ERROR(logger, "Invalid global escape-strategy [" << esc_str << "].");
    return -1;
   }
  }
  if (is_set_flag("hesc")) {
   auto hesc_str = string(get_arg_for_flag("hesc"));
   if ((head_strat << hesc_str) == EscapeStrategy::ESC_UNDEF) {
    ERROR(logger, "Invalid header escape-strategy [" << hesc_str << "].");
    return -1;
   }
  }
  for (auto cesc_arg : get_values_for_flag("cesc")) {
   ColIvalV civ(false);
   EscapeStrategy cesc;
   if (civ.parse(cesc_arg[0])) {
    ERROR(logger, "Illegal column expression [" << cesc_arg[0] << "].");
    return -1;
   }
   if ((cesc << cesc_arg[1]) == EscapeStrategy::ESC_UNDEF) {
    ERROR(logger, "Invalid column escape-strategy [" << cesc_arg[1] << "].");
    return -1;
   }
   col_strat.push_back({civ, cesc});
  }
  return 0;
 }
 void extract_strat(unsigned int colnum) {
  // Prio 3
  head_esc_v.resize(colnum, strat);
  body_esc_v.resize(colnum, strat);
  // Prio 2
  for (auto xesc : col_strat) {
   if (xesc.second == EscapeStrategy::ESC_INHERIT) continue;
   for (ColID cid : xesc.first.extract_ival(colnum)) {
    head_esc_v[cid] = xesc.second;
    body_esc_v[cid] = xesc.second;
   }
  }
  // Prio 1
  if (head_strat != EscapeStrategy::ESC_INHERIT) {
   head_esc_v = vector<EscapeStrategy>(colnum, head_strat);
  }
 }
 Delimiters get_delims() const {return delims;}
 EscapeStrategy get_strat() const {return strat;}
 const vector<EscapeStrategy> &get_strat_h() const {return head_esc_v;}
 const vector<EscapeStrategy> &get_strat_b() const {return body_esc_v;}
};

const string DESCRIPTION="Pipes csv from stdin to stdout and applies specified conversions on rows and fields. Field and record separator characters may be overridden.\n";
const string USAGE="[-ifs <chr>] [-ofs <chr>] [-irs <chr>] [-ors <chr>] [-esc <strat>] [-cesc <expr> <strat>] [-hesc <strat>]\n"
"chr is an arbitrary character\n"
"strat ::= 'all'|'preserve'|'resolve'|'remove'\t(escape strategy)\n" // inherit is a hidden strategy (cannot even be parsed)
 + string(colivalv_expr_help_str) +
"where num is integer value, for the first column num=0.";



int main(int argc, const char *argv[]) {
 PipeCommandLine cmdline=PipeCommandLine(DESCRIPTION,USAGE);
 CommandLineExecuteResponse resp=cmdline.execute(argc, argv);
 if (resp!=CMDLINE_OK) {
  return resp;
 }
 CsvIStream cis(cin,cmdline.get_delims());
 auto head = cis.get_head();
 cmdline.extract_strat(head.size());

 head.print_refined(cout, cmdline.get_delims(), cmdline.get_strat_h())<<cmdline.get_delims().get(Delimiter::ORS);

 while (cis.next()) {
  auto line = cis.get_current_line();
  line.print_refined(cout, cmdline.get_delims(), cmdline.get_strat_b())<<cmdline.get_delims().get(Delimiter::ORS);
 }
}

