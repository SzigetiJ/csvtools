#include "CsvCell.h"
#include <string>
#include <cassert>

using namespace std;

bool test_CsvCellToDecimal(const CsvCell &orig, const CsvCell &expected) {
  bool retv = true;

  auto actual=orig;
  actual.to_decimal();

  if (!(actual == expected)) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual.get_dat()<<"], expected: ["<<expected.get_dat()<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert(test_CsvCellToDecimal(CsvCell("",false),CsvCell("",false)));
  assert(test_CsvCellToDecimal(CsvCell("1,2",true),CsvCell("1.2",true)));
  assert(test_CsvCellToDecimal(CsvCell(",",true),CsvCell(".",true)));
  assert(test_CsvCellToDecimal(CsvCell(",,",true),CsvCell("..",true)));

  return 0;
}
