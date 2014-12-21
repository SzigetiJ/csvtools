#include "RowFunc.h"
#include "CsvRow.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

bool row_matches(const CsvRow &row, const RowFilterV &filter_v){
  bool ok=true;
  for (unsigned int filt=0;ok && filt<filter_v.size();++filt){
   const FieldV &xfv=extract_ival(filter_v[filt].second.first,row.size());
   CsvRow dfline_csv=row.get_fields(xfv);
   stringstream ss;
   string dfline_s;
   dfline_csv.print(ss,Delimiters(),ESC_PRESERVE);
   dfline_s=ss.str();
   if (!filter_v[filt].first(dfline_s,filter_v[filt].second.second))
    ok=false;
  }
  return ok;
}
