#include "CsvCell.h"
#include <sstream>
#include <string>
#include <cassert>

using namespace std;

bool test_CsvCellPrintByStrat(const CsvCell &obj, EscapeStrategy strat, string expected) {
  const Delimiters delims;
  bool retv = true;

  ostringstream ss;
  obj.print(ss,delims,strat);
  string actual = ss.str();

  if (actual != expected) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual<<"], expected: ["<<expected<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert(test_CsvCellPrintByStrat(CsvCell("hello",false), ESC_PRESERVE, "hello"));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",false), ESC_ALL, "\"hello\""));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",false), ESC_REMOVE, "hello"));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",false), ESC_RESOLVE, "hello"));

  assert(test_CsvCellPrintByStrat(CsvCell("hello",true), ESC_PRESERVE, "\"hello\""));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",true), ESC_ALL, "\"hello\""));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",true), ESC_REMOVE, "hello"));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",true), ESC_RESOLVE, "\"hello\""));  // escaping was not requisite, still it was there. Not removing

  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), ESC_PRESERVE, "\"quote\"\"within\""));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), ESC_ALL, "\"quote\"\"within\""));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), ESC_REMOVE, "quote\"within"));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), ESC_RESOLVE, "\"quote\"\"within\""));

  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), ESC_PRESERVE, "\"comma,\""));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), ESC_ALL, "\"comma,\""));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), ESC_REMOVE, "\"comma,\""));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), ESC_RESOLVE, "\"comma,\""));

  return 0;
}
