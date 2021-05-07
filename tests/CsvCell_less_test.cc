#include "CsvCell.h"
#include <string>
#include <cassert>

using namespace std;

bool test_CsvCellLess(const CsvCell &a, const CsvCell &b, bool expected) {
  bool retv = true;

  bool actual=a<b;

  if (actual != expected) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual<<"], expected: ["<<expected<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert(test_CsvCellLess(CsvCell("",false),CsvCell("",false), false));
  assert(test_CsvCellLess(CsvCell("",true),CsvCell("",true), false));
  assert(test_CsvCellLess(CsvCell("",false),CsvCell("",true), true));
  assert(test_CsvCellLess(CsvCell("",true),CsvCell("",false), false));

  assert(test_CsvCellLess(CsvCell("bar",false),CsvCell("foo",false), true));
  assert(test_CsvCellLess(CsvCell("foo",false),CsvCell("bar",false), false));

  assert(test_CsvCellLess(CsvCell("bar",false),CsvCell("barr",false), true));
  assert(test_CsvCellLess(CsvCell("bar",false),CsvCell("barr",true), true));
  assert(test_CsvCellLess(CsvCell("bar",true),CsvCell("barr",false), true));

  assert(test_CsvCellLess(CsvCell("10",false),CsvCell("2",false), true));

  return 0;
}
