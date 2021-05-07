#include "CsvCell.h"
#include <sstream>
#include <string>
#include <cassert>

using namespace std;

bool test_CsvCellParse(const string &obj, const Delimiters &delims, const CsvCell &expected_obj, char expected_result) {
  bool retv = true;

  CsvCell actual;
  stringstream ss(obj);
  auto result = actual.parse(ss,delims);

  if (!(actual == expected_obj)) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual.get_dat()<<"], expected: ["<<expected_obj.get_dat()<<"]"<<endl;
  }
  if (result != expected_result) {
    retv = false;
    cerr<<"Diff. actual result: ["<<(int)result<<"], expected result: ["<<(int)expected_result<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {
  const Delimiters delims;

  assert(test_CsvCellParse("", delims, CsvCell("",false),delims.get(EoF)));
  assert(test_CsvCellParse("simple_01", delims, CsvCell("simple_01",false),delims.get(EoF)));
  assert(test_CsvCellParse("simple_02,other", delims, CsvCell("simple_02",false),delims.get(IFS)));
  assert(test_CsvCellParse("simple_03\nother", delims, CsvCell("simple_03",false),delims.get(IRS)));
  assert(test_CsvCellParse(",other04", delims, CsvCell("",false),delims.get(IFS)));
  assert(test_CsvCellParse("\nother05", delims, CsvCell("",false),delims.get(IRS)));
  assert(test_CsvCellParse("simple\"_06", delims, CsvCell("simple\"_06",false),delims.get(EoF)));
  assert(test_CsvCellParse("simple\"\"_07", delims, CsvCell("simple\"\"_07",false),delims.get(EoF)));

  // escaped
  assert(test_CsvCellParse("\"\"", delims, CsvCell("",true),delims.get(EoF)));
  assert(test_CsvCellParse("\"escaped_01\"", delims, CsvCell("escaped_01",true),delims.get(EoF)));
  assert(test_CsvCellParse("\"escaped_02\",\"other\"", delims, CsvCell("escaped_02",true),delims.get(IFS)));
  assert(test_CsvCellParse("\"escaped_03\"\n\"other\"", delims, CsvCell("escaped_03",true),delims.get(IRS)));
  assert(test_CsvCellParse("\"\",\"other04\"", delims, CsvCell("",true),delims.get(IFS)));
  assert(test_CsvCellParse("\"\"\"a\"\"\",\"other05\"", delims, CsvCell("\"a\"",true),delims.get(IFS)));
  assert(test_CsvCellParse("\"\"other06", delims, CsvCell("\"other06",true),delims.get(EoF)));  //FIXME: introduce fail strategy?
  assert(test_CsvCellParse("\"escaped,07\"", delims, CsvCell("escaped,07",true),delims.get(EoF)));
  assert(test_CsvCellParse("\"escaped\n08\"", delims, CsvCell("escaped\n08",true),delims.get(EoF)));
  assert(test_CsvCellParse("\"escaped\"\n09\"", delims, CsvCell("escaped",true),delims.get(IRS)));
  assert(test_CsvCellParse("\"escaped\"\"\n10\"", delims, CsvCell("escaped\"\n10",true),delims.get(EoF)));

  return 0;
}
