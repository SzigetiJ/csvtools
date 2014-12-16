#ifndef _ROWFUNC_H_
#define _ROWFUNC_H_
#include "ColTypes.h"
#include "CsvClasses.h"
#include <string>

/// bool function with two string parameters (binary predicate)
typedef bool (*OpFun)(const std::string&, const std::string&);

/// row filter structure naming the binary predicate and the two parameters of the predicate.
/// The first parameter defines a list of fields, the second parameter is a string invariant.
typedef std::pair<OpFun,std::pair<ColIvalV, std::string> > RowFilter;
typedef std::vector<RowFilter> RowFilterV;

bool row_matches(const CsvRow &, const RowFilterV &);
#endif
