#ifndef _ROWFUNC_H_
#define _ROWFUNC_H_
#include "ColTypes.h"
#include "CsvClasses.h"
#include <string>
#include <functional>

/// bool function with two string parameters (binary predicate)
typedef std::function<bool (const std::string&, const std::string&)> OpFun;

/// row filter structure naming the binary predicate and the two parameters of the predicate.
/// The first parameter defines a list of fields, the second parameter is a string invariant.
class RowFilter : public std::pair<OpFun,std::pair<ColIvalV, std::string> > {
public:
 RowFilter(const std::pair<OpFun,std::pair<ColIvalV, std::string> >&);
 bool row_matches(const CsvRow&) const;
};

typedef std::vector<RowFilter> RowFilterV;

#endif
