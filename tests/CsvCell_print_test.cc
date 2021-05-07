#include "CsvCell.h"
#include <sstream>
#include <string>
#include <cassert>

using namespace std;

bool test_CsvCellPrintByStrat(const CsvCell &obj, const Delimiters &delims, const EscapeStrategy &strat, const string &expected) {
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
  const Delimiters delims;

  assert(test_CsvCellPrintByStrat(CsvCell("hello",false), delims, ESC_PRESERVE, "hello"));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",false), delims, ESC_ALL, "\"hello\""));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",false), delims, ESC_REMOVE, "hello"));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",false), delims, ESC_RESOLVE, "hello"));

  assert(test_CsvCellPrintByStrat(CsvCell("hello",true), delims, ESC_PRESERVE, "\"hello\""));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",true), delims, ESC_ALL, "\"hello\""));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",true), delims, ESC_REMOVE, "hello"));
  assert(test_CsvCellPrintByStrat(CsvCell("hello",true), delims, ESC_RESOLVE, "\"hello\""));  // escaping was not requisite, still it was there. Not removing

  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), delims, ESC_PRESERVE, "\"quote\"\"within\""));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",false), delims, ESC_PRESERVE, "quote\"within"));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), delims, ESC_ALL, "\"quote\"\"within\""));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), delims, ESC_REMOVE, "quote\"within"));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), delims, ESC_RESOLVE, "\"quote\"\"within\""));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",false), delims, ESC_RESOLVE, "quote\"within"));

  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), delims, ESC_PRESERVE, "\"comma,\""));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), delims, ESC_ALL, "\"comma,\""));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), delims, ESC_REMOVE, "\"comma,\""));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), delims, ESC_RESOLVE, "\"comma,\""));

  Delimiters delims_x;
  delims_x.set(OESC,'\'');
  delims_x.set(OFS,';');

  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), delims_x, ESC_PRESERVE, "\'quote\"within\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), delims_x, ESC_ALL, "\'quote\"within\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), delims_x, ESC_REMOVE, "quote\"within"));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",true), delims_x, ESC_RESOLVE, "\'quote\"within\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("quote\"within",false), delims_x, ESC_RESOLVE, "quote\"within"));
  assert(test_CsvCellPrintByStrat(CsvCell("\'quote\"within",false), delims_x, ESC_RESOLVE, "\'\'\'quote\"within\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("\"quote_begin",true), delims_x, ESC_RESOLVE, "\"quote_begin"));

  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), delims_x, ESC_PRESERVE, "\'comma,\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), delims_x, ESC_ALL, "\'comma,\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), delims_x, ESC_REMOVE, "comma,"));
  assert(test_CsvCellPrintByStrat(CsvCell("comma,",true), delims_x, ESC_RESOLVE, "comma,"));

  assert(test_CsvCellPrintByStrat(CsvCell("semicolon;",false), delims_x, ESC_PRESERVE, "\'semicolon;\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("semicolon;",false), delims_x, ESC_ALL, "\'semicolon;\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("semicolon;",false), delims_x, ESC_REMOVE, "\'semicolon;\'"));
  assert(test_CsvCellPrintByStrat(CsvCell("semicolon;",false), delims_x, ESC_RESOLVE, "\'semicolon;\'"));

  return 0;
}
