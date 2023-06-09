#include "CsvRow.h"
#include <sstream>
#include <string>
#include <cassert>
#include "stl_out.h"

using namespace std;

ostream &operator<<(ostream &a, const CsvCell &b) {
  Delimiters delims;
  b.print(a,delims,ESC_PRESERVE);
  return a;
}

bool test_CsvRowParse(const string &obj, const Delimiters &delims, const CsvRow &expected_obj, bool expected_result) {
  bool retv = true;

  CsvRow actual;
  stringstream ss(obj);
  bool result = actual.parse(ss,delims);

  if (!(actual == expected_obj)) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual<<"], expected: ["<<expected_obj<<"]"<<endl;
  }
  if (result != expected_result) {
    retv = false;
    cerr<<"Diff. actual result: ["<<(int)result<<"], expected result: ["<<(int)expected_result<<"]"<<endl;
  }

  return retv;
}

bool test_CsvRowPrint(const string &str, const Delimiters &delims, const EscapeStrategy &strat, const string &expected) {
  bool retv = true;

  ostringstream ss;
  stringstream iss(str);
  CsvRow obj;
  obj.parse(iss,delims);
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

  assert(test_CsvRowParse("hello", delims, CsvRow({CsvCell("hello",false)}),false));
  assert(test_CsvRowParse("\"hello\"", delims, CsvRow({CsvCell("hello",true)}),false));
  assert(test_CsvRowParse("\"hello\",world", delims, CsvRow({CsvCell("hello",true),CsvCell("world",false)}),false));
  assert(test_CsvRowParse("1,2,\"3,4\"", delims, CsvRow({CsvCell("1",false),CsvCell("2",false),CsvCell("3,4",true)}),false));

  assert(test_CsvRowParse("\n", delims, CsvRow({CsvCell("",false)}),true));
  assert(test_CsvRowParse("1,2\n", delims, CsvRow({CsvCell("1",false),CsvCell("2",false)}),true));

  assert(test_CsvRowPrint("1",delims,ESC_PRESERVE,"1"));
  assert(test_CsvRowPrint("1\n2",delims,ESC_PRESERVE,"1"));
  assert(test_CsvRowPrint("1,2",delims,ESC_PRESERVE,"1,2"));
  assert(test_CsvRowPrint("1,2,\"3,4\"",delims,ESC_PRESERVE,"1,2,\"3,4\""));


  return 0;
}
