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
#include "ColFunc.h"
#include "CsvPipe.h"
#include "CsvRow.h"
#include "Delimiters.h"
#include "DefaultCommandLine.h"
#include "Numeric.h"
#include "log.h"
#include "stl_out.h"

using namespace std;

const string DESCRIPTION = "Creates pivot table of CSV file.\n";
const string USAGE = "-p <expr> <num> <num> <fun>\n"
        + string(colivalv_expr_help_str) +
        " fun ::= sum|min|max|count|concat|first|last\t\t(aggregation function, sum, min and max aggregates numeric values, count counts not empty cells, concat concatenates cells without separator)\n"
        "where str is string, num is integer\n";

const char *OPTION_PIVOT = "p";

// Aggregation specific options.
const Option pivot_option_a[] = {
  {OPTION_PIVOT, "pivot", 4, OVERRIDE, "Defines pivoting. arg1: key columns used for grouping, arg2: column that defines the header of the pivot table, arg3: column to aggregate by key, arg4: aggregation function (sum|min|max|count|concat|first|last)"}
};
const int pivot_option_n = sizeof (pivot_option_a) / sizeof (Option);

/// Extension to DefaultCommandLine: AggregationCommandLine can derive from option value(s) which columns are aggregated and which columns are grouped.
class PivotCommandLine : public DefaultCommandLine {
  ColIvalV col_v;
  FieldV field_v;
  ColID pcol_id;
  ColID acol_id;
  AggrFunPtr afun;
public:

  PivotCommandLine(const string &desc, const string &usage) :
  DefaultCommandLine(desc, usage, set<Option>(pivot_option_a, pivot_option_a + pivot_option_n)), col_v(false) {
  };

  int process() {
    if (!is_set_flag(OPTION_PIVOT)) {
      ERROR(get_log_config(), "No pivot parameters.");
      return -1;
    }
    vector<const char*> arg = get_values_for_flag(OPTION_PIVOT)[0];
    const char *key = arg[0];
    const char *pcol = arg[1];
    const char *acol = arg[2];
    const char *fun = arg[3];

    if (col_v.parse(key)) {
      ERROR(get_log_config(), "Illegal column expression [" << key << "].");
      return -1;
    }
    pcol_id = atoi(pcol);
    acol_id = atoi(acol);
    AggrFunMap::const_iterator fmi = std_colfun_m.find(fun);
    if (fmi == std_colfun_m.end()) {
      ERROR(logger, "Unrecognized function [" << fun << "]. Aborting.");
      return -2;
    }
    afun = fmi->second.get();
    INFO(global_logger, "Pivot parsed.");
    return 0;
  };

  void set_colnum(int size) {
    INFO(global_logger, "columns: " << size);
    field_v = col_v.extract_ival(size);
    INFO(global_logger, "columns: " << field_v);
  }

  FieldV get_key_cols() const {
    return field_v;
  }

  ColID get_pcol() const {
    return pcol_id;
  }

  ColID get_acol() const {
    return acol_id;
  }

  AggrFunPtr get_aggr() const {
    return afun;
  }
};

int main(int argc, const char *argv[]) {
  const Delimiters delims;
  const EscapeStrategy strat = ESC_PRESERVE;

  PivotCommandLine cmdline = PivotCommandLine(DESCRIPTION, USAGE);
  CommandLineExecuteResponse resp = cmdline.execute(argc, argv);
  if (resp != CMDLINE_OK) {
    return resp;
  }

  // parse header
  CsvRow head;
  if (!head.parse(cin, delims)) {
    ERROR(cmdline.get_log_config(), "Cannot parse CSV header");
    return -2;
  }
  cmdline.set_colnum(head.size());

  // variables
  vector<CsvRow> key_v;
  map<CsvRow, unsigned int> key_rev_m;
  vector<CsvCell> pcol_v;
  map<CsvCell, unsigned int> pcol_rev_m;

  map<pair<unsigned int, unsigned int>, AggrValue> avalues;

  // parse body
  CsvRow rx;
  while (rx.parse(cin, delims)) {
    CsvRow key_id = rx.get_fields(cmdline.get_key_cols());
    auto key_it = key_rev_m.find(key_id);
    unsigned int key_idx = 0;
    if (key_it != key_rev_m.end()) {
      key_idx = key_it->second;
    } else {
      key_idx = key_v.size();
      key_v.push_back(key_id);
      key_rev_m.insert(make_pair(key_id, key_idx));
    }
    CsvCell pcol_id = rx[cmdline.get_pcol()];
    auto pcol_it = pcol_rev_m.find(pcol_id);
    unsigned int pcol_idx = 0;
    if (pcol_it != pcol_rev_m.end()) {
      pcol_idx = pcol_it->second;
    } else {
      pcol_idx = pcol_v.size();
      pcol_v.push_back(pcol_id);
      pcol_rev_m.insert(make_pair(pcol_id, pcol_idx));
    }
    CsvCell aval = rx[cmdline.get_acol()];
    auto avalues_idx = make_pair(key_idx, pcol_idx);
    auto avalues_it = avalues.find(avalues_idx);
    if (avalues_it != avalues.end()) {
      avalues_it->second.accumulate(aval.get_dat());
    } else {
      auto res = avalues.insert(make_pair(avalues_idx, AggrValue(cmdline.get_aggr())));
      res.first->second.init(aval.get_dat());
    }
    rx.clear();
  }

  // output head
  CsvRow out_head = head.get_fields(cmdline.get_key_cols());
  for (CsvCell header_cell : pcol_v) {
    out_head.push_back(header_cell);
  }
  out_head.print(cout, delims, strat);
  cout << delims.get(ORS);

  // output body
  for (unsigned int key_idx = 0; key_idx < key_v.size(); ++key_idx) {
    CsvRow out_row = key_v[key_idx];
    for (unsigned int pcol_idx = 0; pcol_idx < pcol_v.size(); ++pcol_idx) {
      auto val_idx = make_pair(key_idx, pcol_idx);
      auto val_it = avalues.find(val_idx);
      auto out_cell = CsvCell(val_it != avalues.end() ? val_it->second.get_dat() : "", true);
      out_row.push_back(out_cell);
    }
    out_row.print(cout, delims, strat);
    cout << delims.get(ORS);
  }

  return 0;
}
