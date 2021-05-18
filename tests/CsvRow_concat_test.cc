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

bool test_CsvRowConcat(CsvRow &actual, const CsvRow &added, const CsvRow &expected) {
  bool retv = true;

  actual+=added;

  if (!(actual == expected)) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual<<"], expected: ["<<expected<<"]"<<endl;
  }

  return retv;
}

bool test_CsvRowConcat_str(const string &orig_str, const string &added_str, const string &expected_str) {
  Delimiters delims;
  CsvRow orig,added,expected;

  stringstream orig_ss(orig_str);
  stringstream added_ss(added_str);
  stringstream expected_ss(expected_str);

  orig.parse(orig_ss, delims);
  added.parse(added_ss, delims);
  expected.parse(expected_ss, delims);

  return test_CsvRowConcat(orig, added, expected);
}
int main(int argc, const char *argv[]) {

  assert(test_CsvRowConcat_str("1","2","1,2"));
  assert(test_CsvRowConcat_str("1","","1,"));
  assert(test_CsvRowConcat_str("","",","));

  assert(test_CsvRowConcat_str("1,2,",",3,4","1,2,,,3,4"));

  Delimiters delims;
  CsvRow t0;
  stringstream t0_ss("1,2");
  t0.parse(t0_ss,delims);
  CsvRow ex0;
  stringstream ex0_ss("1,2,1,2");
  ex0.parse(ex0_ss,delims);
  CsvRow ex1;
  stringstream ex1_ss("1,2,1,2,1,2,1,2");
  ex1.parse(ex1_ss,delims);
  assert(test_CsvRowConcat(t0,t0,ex0));
  assert(test_CsvRowConcat(t0,t0,ex1));
  return 0;
}
