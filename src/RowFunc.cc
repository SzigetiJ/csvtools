/*
 *  Copyright (C) 2014 -- 2022 SZIGETI János <szigeti at pilar dot hu>
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
#include "RowFunc.h"
#include "CsvRow.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

RowFilter::RowFilter(const pair<OpFun,pair<ColIvalV, string> > &a):pair<OpFun,pair<ColIvalV, string> >(a){}

bool RowFilter::row_matches(const CsvRow &row) const {
   const FieldV &xfv=second.first.extract_ival(row.size());
   CsvRow dfline_csv=row.get_fields(xfv);
   stringstream ss;
   string dfline_s;
   dfline_csv.print(ss,Delimiters(),ESC_PRESERVE);
   dfline_s=ss.str();
   return first(dfline_s,second.second);
}

RowFunSpec::RowFunSpec(const char *_name, const char *_desc, unsigned int _argc, const RowFun &_fun) : name(_name), desc(_desc), argc(_argc), fun(_fun) {
}

const char *RowFunSpec::get_name() const {
 return name;
}

const char *RowFunSpec::get_desc() const {
 return desc;
}

unsigned int RowFunSpec::argnum() const {
 return argc;
}

std::string RowFunSpec::exec(const std::string *_argv) const {
 return fun(_argv);
}
