#include "RowFunc.h"
#include "CsvRow.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

RowFilter::RowFilter(const pair<OpFun,pair<ColIvalV, string> > &a):pair<OpFun,pair<ColIvalV, string> >(a){};

bool RowFilter::row_matches(const CsvRow &row) const {
   const FieldV &xfv=second.first.extract_ival(row.size());
   CsvRow dfline_csv=row.get_fields(xfv);
   stringstream ss;
   string dfline_s;
   dfline_csv.print(ss,Delimiters(),ESC_PRESERVE);
   dfline_s=ss.str();
   return first(dfline_s,second.second);
};
