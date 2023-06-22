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
 RowFunSpec fun;
 string name;
} AppendCol;

class RowFunArgument {
public:
 virtual string get_value(const CsvRow &row) const = 0;
};

class CRowFunArgument : public RowFunArgument {
 unsigned int col_idx;
public:
 CRowFunArgument(unsigned int _col_idx):col_idx(_col_idx){}
 string get_value(const CsvRow &row) const {
  return row[col_idx].get_dat();
 }
};

class VRowFunArgument : public RowFunArgument {
 string value;
public:
 VRowFunArgument(const string &_value):value(_value){}
 string get_value(const CsvRow &row) const {
  return value;
 }
};

typedef struct {
 unsigned int fun_idx;
 unsigned int arg_idx;
 shared_ptr<RowFunArgument> arg_ptr;
} FunArg;

initializer_list<RowFunSpec> rowfun_a = {
 RowFunSpec("add","arg0 + arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) + stoi(a[1]));}),
 RowFunSpec("sub","arg0 - arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) - stoi(a[1]));}),
 RowFunSpec("mul","arg0 * arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) * stoi(a[1]));}),
 RowFunSpec("div","arg0 / arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) / stoi(a[1]));}),
 RowFunSpec("mod","arg0 % arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) % stoi(a[1]));}),
 RowFunSpec("or","arg0 | arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) | stoi(a[1]));}),
 RowFunSpec("and","arg0 & arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) & stoi(a[1]));}),
 RowFunSpec("int","int(arg0),\tint(numeric)", 1, [](const string * a) {return to_string((int)stof(a[0]));}),

 RowFunSpec("id","arg0,\tstring(string)", 1, [](const string * a) {return string(a[0]);}),
 RowFunSpec("concat","arg0.append(arg1),\tstring(string,string)", 2, [](const string * a) {return string(a[0]).append(a[1]);}),
 RowFunSpec("length","arg0.length(),\tint(string)", 1, [](const string * a) {return to_string(a[0].length());}),
 RowFunSpec("index","arg0.find(arg1),\tint(string, string)", 2, [](const string * a) {return to_string(a[0].find(a[1]));}),
 RowFunSpec("substr","arg0.substr(arg1, arg2),\tstring(string, int, int)", 3, [](const string * a) {return a[0].substr(stoi(a[1]),stoi(a[2]));}),

 RowFunSpec("ifeq","arg0==arg1?arg2:arg3,\tstring(string, string, string, string)", 4, [](const string * a) {return a[0]==a[1]?a[2]:a[3];}),
 RowFunSpec("iflt_str","arg0<arg1?arg2:arg3,\tstring(string, string, string, string)", 4, [](const string * a) {return a[0]<a[1]?a[2]:a[3];}),
 RowFunSpec("iflt_int","(int)arg0<(int)arg1?arg2:arg3,\tstring(int, int, string, string)", 4, [](const string * a) {return stoi(a[0])<stoi(a[1])?a[2]:a[3];})
};

class AppenderCommandLine : public DefaultCommandLine {
 map<string, RowFunSpec > rowfun_m;
 vector<AppendCol> extcol_v;
 vector<FunArg> args_v;
 set<pair<unsigned int, unsigned int> > argpos_s;
 vector<vector<shared_ptr<RowFunArgument> > > arg_a;

 int parse_funargstr(bool pc, const string &str, FunArg &res) {
  auto a = str_split(str, PARAM_INTERNAL_SEP, 3);
  if (a.size() != 3) {
   ERROR(get_log_config(), "Illegal parameter [" << str << "]");
   return -1;
  }
  unsigned int fpos = stoul(a[0]);
  unsigned int apos = stoul(a[1]);
  shared_ptr<RowFunArgument> xarg = pc ?
   (shared_ptr<RowFunArgument>)make_shared<CRowFunArgument>(stoul(a[2])):
   (shared_ptr<RowFunArgument>)make_shared<VRowFunArgument>(a[2]);
  res = (FunArg) {fpos, apos, xarg};
  return 0;
 }

public:

 AppenderCommandLine(const string &desc, const string &usage, const initializer_list<RowFunSpec> _rowfuns) :
 DefaultCommandLine(desc, usage, set<Option>(aggr_option_a, aggr_option_a + aggr_option_n)) {
  for (RowFunSpec spec : _rowfuns) {
   rowfun_m.insert({spec.get_name(), spec});
  }
 };

 int process() {
  if (is_set_flag("hf")) {
   cout << "Supported functions:"<<endl;
   for (auto spec : rowfun_m) {
    cout<<" "<<spec.first<<"\t"<<spec.second.get_desc()<<endl;
   }
   return -1; // TODO
  }
  for (vector<const char*> arg : get_values_for_flag("a")) {
   auto a = str_split(arg[0], PARAM_INTERNAL_SEP, 2);
   if (a.size() != 2) {
    ERROR(get_log_config(), "Illegal parameter syntax [" << arg[0] << "]");
    return -1;
   }
   auto rowfun_it = rowfun_m.find(a[0]);
   if (rowfun_it == rowfun_m.end()) {
    ERROR(get_log_config(), "Function not implemented [" << a[0] << "]");
    return -1;
   }
   extcol_v.push_back((AppendCol){rowfun_it->second, a[1]});
  }

  for (vector<const char*> arg : get_values_for_flag("pc")) {
   FunArg fa;
   int ret = parse_funargstr(true, arg[0],fa);
   if (ret!=0) {
    return ret;
   }
   args_v.push_back(fa);
   argpos_s.insert({fa.fun_idx, fa.arg_idx});
  }

  for (vector<const char*> arg : get_values_for_flag("pv")) {
   FunArg fa;
   int ret = parse_funargstr(false, arg[0], fa);
   if (ret!=0) {
    return ret;
   }
   args_v.push_back(fa);
   argpos_s.insert({fa.fun_idx, fa.arg_idx});
  }

  bool err_arg_oor = false;
  arg_a.resize(extcol_v.size());
  for (unsigned int i = 0; i < extcol_v.size(); ++i) {
   arg_a[i].resize(extcol_v[i].fun.argnum());
  }
  for (FunArg fa : args_v) {
   if (extcol_v.size() <= fa.fun_idx) {
    ERROR(get_log_config(), "fpos #" << fa.fun_idx << " out of range");
    err_arg_oor = true;
    continue;
   }
   auto extcol = extcol_v[fa.fun_idx];
   if (extcol.fun.argnum() <= fa.arg_idx) {
    ERROR(get_log_config(), "apos #" << fa.arg_idx << " in function #" << fa.fun_idx << " out of range");
    err_arg_oor = true;
    continue;
   }
   arg_a[fa.fun_idx][fa.arg_idx] = fa.arg_ptr;
  }

  bool err_arg_missing = false;
  for (unsigned int fpos = 0; fpos < extcol_v.size(); ++fpos) {
   auto &extcol = extcol_v[fpos];
   for (unsigned int apos = 0; apos < extcol.fun.argnum(); ++apos) {
    if (argpos_s.find({fpos, apos}) == argpos_s.end()) {
     ERROR(get_log_config(), "Function #" << fpos << " requires " << extcol.fun.argnum() << " arguments. Arg #" << apos << " not defined.");
     err_arg_missing = true;
    }
   }
  }
  return (err_arg_missing || err_arg_oor) ? -2 : 0;
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

 AppenderCommandLine cmdline = AppenderCommandLine(DESCRIPTION, USAGE, rowfun_a);
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
