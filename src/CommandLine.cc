/*
 *  Copyright (C) 2014 -- 2023 SZIGETI János <szigeti at pilar dot hu>
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
#include <algorithm>
#include "CommandLine.h"
#include "stl_out.h"

constexpr char DASH_CHR = '-';
constexpr char EQ_CHR = '=';

using namespace std;

/// strict weak ordering by flag (lexicographic).
bool Option::operator<(const _Option &b) const {
  return (flag && b.flag) ? std::strcmp(flag, b.flag) < 0 : (flag < b.flag);
}

ostream &operator<<(ostream &a, const Option &b) {
  if (b.flag)
    a << DASH_CHR << b.flag;
  if (b.longname) {
    a << "\t" << DASH_CHR << DASH_CHR << b.longname;
    if (b.arg_num) {
      for (int i = 0; i < b.arg_num; ++i) {
        a << (i == 0 ? EQ_CHR : ' ') << "<arg" << (i + 1) << ">";
      }
    }
  }
  if (b.help) {
    a << "\t" << b.help;
  }
  return a;
}

CommandLine::CommandLine(const set<Option> &a) : logger(LogConfig()), option_s(a) {
  for (auto option : a) {
    if (option.flag) {
      flag_m.insert(make_pair(option.flag, option));
    }
    if (option.longname) {
      longname_m.insert(make_pair(option.longname, option));
    }
  }
}

int CommandLine::parse(int argc, const char *argv[]) {
  progname = argv[0];
  int i = 1;
  while (i < argc) {
    const char *xcmd = argv[i];
    Option xoption;
    // option begins with dash
    if (xcmd[0] == DASH_CHR) {
      int arg_chr_shift = 0;
      // double dash denotes longname option
      if (xcmd[1] == DASH_CHR) {
        // longname option may be followed by equals sign
        const char *xcmd_e = xcmd + strlen(xcmd);
        const char *eq_pos = find(xcmd, xcmd_e, EQ_CHR);
        auto it = longname_m.find(string(xcmd + 2, eq_pos));
        if (it == longname_m.end()) {
          ERROR(logger, "Cannot interpret option [" << xcmd << "], aborting.");
          return 1;
        }
        xoption = it->second;
        if (xoption.arg_num) {
          if (eq_pos == xcmd_e) {
            ERROR(logger, "Option [" << xoption.longname << "] requires argument concatenated with '" << EQ_CHR << "'. Aborting.");
            return 1;
          }
          arg_chr_shift = eq_pos - xcmd + 1;
        } else {
          if (eq_pos != xcmd_e) {
            ERROR(logger, "Option [" << xoption.longname << "] does not require any argument concatenated with '" << EQ_CHR << "'. Aborting.");
            return 1;
          }
          ++i;
        }
      } else {
        auto it = flag_m.find(xcmd + 1);
        if (it == flag_m.end()) {
          ERROR(logger, "Cannot interpret option [" << xcmd << "], skipped.");
          return 1;
        }
        xoption = it->second;
        ++i;
      }
      vector<const char *> value_v;
      for (int j = 0; j < xoption.arg_num; ++j, ++i) {
       if (argc <= i) {
        ERROR(logger, "Unexpected end of command line (option argument missing)");
        return 1;
       }
        value_v.push_back(argv[i] + arg_chr_shift);
        arg_chr_shift = 0;
      }
      // decide what to do according to multidef setting
      auto found_option = value_m.find(xoption);
      if (xoption.multidef == IGNORE && found_option != value_m.end()) {
        WARN(logger, "Option " << xcmd << " ignored (multiple definition).");
      } else if (xoption.multidef == OVERRIDE && found_option != value_m.end()) {
        WARN(logger, "Option " << xcmd << " overrides previous setting (multiple definition).");
        found_option->second = value_v;
      } else {
        value_m.insert(make_pair(xoption, value_v));
      }
    } else {
      ERROR(logger, "Parse error at cmdline argument #" << i << ": " << xcmd);
      return 1;
    }
  }
  DEBUG(logger, "parsed: " << value_m);
  parsed = true;
  return 0;
}

bool CommandLine::is_set_option(const Option &a) const {
  return value_m.find(a) != value_m.end();
}

bool CommandLine::is_set_flag(const char *a) const {
  return a && flag_m.find(a) != flag_m.end() && is_set_option(flag_m.find(a)->second);
}

bool CommandLine::is_set_longname(const char *a) const {
  return a && longname_m.find(a) != longname_m.end() && is_set_option(longname_m.find(a)->second);
}

vector<vector<const char*> > CommandLine::get_values_for_option(const Option &a) const {
  vector<vector<const char*> > retv;
  for (auto it = value_m.lower_bound(a); it != value_m.upper_bound(a); ++it)
    retv.push_back(it->second);
  return retv;
}

vector<vector<const char*> > CommandLine::get_values_for_flag(const char *a) const {
  return (a && flag_m.find(a) != flag_m.end()) ? get_values_for_option(flag_m.find(a)->second) : vector<vector<const char*> >();
}

vector<vector<const char*> > CommandLine::get_values_for_longname(const char *a) const {
  return (a && longname_m.find(a) != longname_m.end()) ? get_values_for_option(longname_m.find(a)->second) : vector<vector<const char*> >();
}

const char* CommandLine::get_arg_for_option(const Option &a, int i) const {
  return is_set_option(a) ? value_m.find(a)->second.at(i) : NULL;
}

const char* CommandLine::get_arg_for_flag(const char *a, int i) const {
  return is_set_flag(a) ? value_m.find(flag_m.find(a)->second)->second.at(i) : NULL;
}

const char* CommandLine::get_arg_for_longname(const char *a, int i) const {
  return is_set_longname(a) ? value_m.find(longname_m.find(a)->second)->second.at(i) : NULL;
}

std::vector<std::string> str_split(const std::string &str, char sep, int parts) {
 vector<string> retv;
 int idx_b = 0;
 for (int i = 0; (i < parts - 1); ++i) {
  int idx_sep = str.find(sep, idx_b);
  if (idx_sep == string::npos) break;
  retv.push_back(str.substr(idx_b, idx_sep - idx_b));
  idx_b = idx_sep + 1;
 }
 retv.push_back(str.substr(idx_b));
 return retv;
}
