/*
 *  Copyright (C) 2023 SZIGETI János <szigeti at pilar dot hu>
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
#include <sstream>
#include <algorithm>
#include <memory>
#include "RowFunc.h"
#include "CsvPipe.h"
#include "CsvRow.h"
#include "Delimiters.h"
#include "DefaultCommandLine.h"
#include "Numeric.h"
#include "log.h"
#include "stl_out.h"
#include "CsvIStream.h"

using namespace std;

constexpr char PARAM_INTERNAL_SEP = ':';

const string DESCRIPTION = "Appends newly defined columns of CSV file.\n"
        "The values in the column are results of a function. The arguments of the function can be either other cells in the row or constant values.\n";
const string USAGE = "-a <fun>:<name> [-a <fun>:<name> [...]] [-pc <fpos>:<apos>:<cnum> | -pv <fpos>:<apos>:<val>] [...]\n"
        " fun ::= str\t\t(function, see -hf / --help-functions for details)\n"
        " name ::= str\t\t(name of the appended output column)\n"
        " fpos ::= num\t\t(function selector (counting begins with 0))\n"
        " apos ::= num\t\t(argument selector (within the function, counting begins with 0))\n"
        " cnum ::= num\t\t(column selector (counting begins with 0))\n"
        " val ::= str\t\t(constant value used as argument)\n";

// Aggregation specific options.
const Option aggr_option_a[] = {
 {"a", "append", 1, APPEND, "Defines a new column. arg: <fun>:<name>, where <fun> is one of the predefined functions, <name> is the name of the new column."},
 {"pc", "pass-column", 1, APPEND, "Binds column to a function argument. arg: <fpos>:<apos>:<cnum>."},
 {"pv", "pass-value", 1, APPEND, "Binds constant value to a function argument. arg: <fpos>:<apos>:<val>."},
 {"hf", "help-functions", 0, IGNORE, "Displays information about predefined functions."}
};
const int aggr_option_n = sizeof (aggr_option_a) / sizeof (Option);

typedef struct {
 RowFun2 fun;
 string name;
} AppendCol;

class RowFunArgument {
public:
 virtual string get_value(const CsvRow &row) const = 0;
};

class CRowFunArgument : public RowFunArgument {
public:
 unsigned int col_idx;

 string get_value(const CsvRow &row) const {
  return row[col_idx].get_dat();
 }
};

class VRowFunArgument : public RowFunArgument {
public:
 string value;

 string get_value(const CsvRow &row) const {
  return value;
 }
};

typedef struct {
 unsigned int fun_idx;
 unsigned int arg_idx;
 shared_ptr<RowFunArgument> arg_ptr;
} FunArg;

class AppenderCommandLine : public DefaultCommandLine {
 vector<AppendCol> extcol_v;
 vector<FunArg> args_v;
 vector<vector<shared_ptr<RowFunArgument> > > arg_a;

 map<string, RowFun2 > rowfun_a = {
  {"add", RowFun2(2, [](const string * a) {
    return to_string(stoi(a[0]) + stoi(a[1]));
   })}
 };

public:

 AppenderCommandLine(const string &desc, const string &usage) :
 DefaultCommandLine(desc, usage, set<Option>(aggr_option_a, aggr_option_a + aggr_option_n)) {
 };

 int process() {
  for (vector<const char*> arg : get_values_for_flag("a")) {
   auto a = str_split(arg[0], PARAM_INTERNAL_SEP, 2);
   if (a.size() != 2) {
    ERROR(get_log_config(), "Illegal parameter [" << arg[0] << "]");
    return -1;
   }
   auto rowfun_it = rowfun_a.find(a[0]);
   if (rowfun_it == rowfun_a.end()) {
    ERROR(get_log_config(), "Function not implemented [" << a[0] << "]");
    return -1;
   }
   extcol_v.push_back((AppendCol){rowfun_it->second, a[1]});
  }

  for (vector<const char*> arg : get_values_for_flag("pc")) {
   auto a = str_split(arg[0], PARAM_INTERNAL_SEP, 3);
   if (a.size() != 3) {
    ERROR(get_log_config(), "Illegal parameter [" << arg[0] << "]");
    return -1;
   }
   CRowFunArgument xcra;
   xcra.col_idx = stoi(a[2]);
   args_v.push_back((FunArg){stoul(a[0]), stoul(a[1]), make_shared<CRowFunArgument>(CRowFunArgument(xcra))});
  }

  for (vector<const char*> arg : get_values_for_flag("pv")) {
   auto a = str_split(arg[0], PARAM_INTERNAL_SEP, 3);
   if (a.size() != 3) {
    ERROR(get_log_config(), "Illegal parameter [" << arg[0] << "]");
    return -1;
   }
   VRowFunArgument xvra;
   xvra.value = a[2];
   args_v.push_back((FunArg){stoul(a[0]), stoul(a[1]), make_shared<VRowFunArgument>(VRowFunArgument(xvra))});
  }

  arg_a.resize(extcol_v.size());
  for (unsigned int i = 0; i < extcol_v.size(); ++i) {
   arg_a[i].resize(extcol_v[i].fun.argnum());
  }
  for (FunArg fa : args_v) {
   if (extcol_v.size() <= fa.fun_idx) {
    ERROR(get_log_config(), "fpos out of range");
    return -2;
   }
   auto extcol = extcol_v[fa.fun_idx];
   if (extcol.fun.argnum() <= fa.arg_idx) {
    ERROR(get_log_config(), "apos out of range");
    return -2;
   }
   arg_a[fa.fun_idx][fa.arg_idx] = fa.arg_ptr;
  }

  return 0;
 };

 CsvRow &append_header(CsvRow &row) const {
  for (unsigned int i = 0; i < extcol_v.size(); ++i) {
   row.push_back(extcol_v[i].name);
  }
  return row;
 }

 CsvRow &append_body(CsvRow &row) const {
  for (unsigned int i = 0; i < extcol_v.size(); ++i) {
   auto &x = extcol_v[i].fun;
   vector<string> args(x.argnum());
   for (unsigned int arg_i = 0; arg_i < args.size(); ++arg_i) {
    args[arg_i] = arg_a[i][arg_i].get()->get_value(row);
   }
   row.push_back(x.exec(&args[0]));
  }
  return row;
 }
};

int main(int argc, const char *argv[]) {
 const Delimiters delims;
 const EscapeStrategy strat = ESC_PRESERVE;

 AppenderCommandLine cmdline = AppenderCommandLine(DESCRIPTION, USAGE);
 CommandLineExecuteResponse resp = cmdline.execute(argc, argv);
 if (resp != CMDLINE_OK) {
  return resp;
 }
 CsvIStream is(cin, delims);

 CsvRow head = is.get_head();
 cmdline.append_header(head).print(cout, delims, strat) << delims.get(Delimiter::ORS);

 while (is.next()) {
  CsvRow row = is.get_current_line();
  cmdline.append_body(row).print(cout, delims, strat) << delims.get(Delimiter::ORS);
 }
 return 0;
}
