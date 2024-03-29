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
#include "CsvRow.h"
#include "RowOrderKey.h"
#include "DefaultCommandLine.h"
#include <iostream>
#include <list>
#include <algorithm>

constexpr char SYM_DESC = 'd';
constexpr char SYM_NUMERIC = 'n';
constexpr char SYM_EMPTYHI = 'e';
constexpr char MODIFIERS[] = {SYM_DESC, SYM_NUMERIC, SYM_EMPTYHI};

using namespace std;


// Sorting specific options.
const Option sort_option_a[]={
{"k","order-key",1,APPEND,"Define sort order key."},
};
const int sort_option_n = sizeof(sort_option_a)/sizeof(Option);

/// Extension to DefaultCommandLine: SortCommandLine can interpret sort order keys.
class SortCommandLine : public DefaultCommandLine {
 list<RowOrderKey> order_l;
public:
 SortCommandLine(const string &desc, const string &usage) :
  DefaultCommandLine(desc, usage,set<Option>(sort_option_a,sort_option_a+sort_option_n)){};
 /// Checks for order keys.
 int process() {
  if (is_set_flag("k")) {
   vector< vector<const char*> > key_v=get_values_for_flag("k");
   for (vector<const char*> key : key_v) {
    const string a(key[0]);

    int a_col;
    size_t a_off;
    try {
     a_col = stoi(a,&a_off);
    } catch (...) {
     ERROR(get_log_config(), "Column number missing in argument '" << a << "'.");
     return -1;
    }
    const string a_mod = a.substr(a_off);
    if (a_mod.find_first_not_of(MODIFIERS, 0, 3) != string::npos) {
     ERROR(get_log_config(), "Invalid modifier in argument '" << a << "'.");
     return -2;
    }
    order_l.push_back(RowOrderKey(a_col,
            (a_mod.find(SYM_DESC) != string::npos),
            (a_mod.find(SYM_NUMERIC) != string::npos),
            (a_mod.find(SYM_EMPTYHI) != string::npos)
    ));
   }
  }
  return 0;
 }
 list<RowOrderKey> get_keys() const {return order_l;}
};

const string DESCRIPTION="Sorts csv data rows. Multiple keys can be defined.\n";
const string USAGE="[-k {num}[d][n][e]] [-k ...]\nwhere\n\tnum is column number,\n\td: descending order,\n\tn: compare as numeric,\n\te: empty fields are greater than not empty fields.";

int main(int argc, const char *argv[]) {
 const Delimiters delims;
 const EscapeStrategy strat=ESC_PRESERVE;

 SortCommandLine cmdline=SortCommandLine(DESCRIPTION,USAGE);
 CommandLineExecuteResponse resp=cmdline.execute(argc, argv);
 if (resp!=CMDLINE_OK) {
  return resp;
 }
// header
 CsvRow head;
 if (!head.parse(cin, delims)){
  ERROR(cmdline.get_log_config(),"Cannot parse CSV header");
  return -2;
 }
 head.print(cout, delims, strat);
 cout<<delims.get(ORS);

// body in
 list<CsvRow> body;
 CsvRow rx;
 while (rx.parse(cin, delims)){
  body.push_back(rx);
  rx.clear();
 }

// sort
 body.sort([&cmdline](const CsvRow &a, const CsvRow &b)->bool {
  for (auto key : cmdline.get_keys()) {
   int diff=key.compare(a,b);
   if (diff!=0) {
    return diff<0;
   }
  }
  return false;
 });
// body out
 for_each(body.begin(),body.end(),[delims, strat](CsvRow &row) {
  row.print(cout, delims, strat);
  cout<<delims.get(ORS);
 });
}

