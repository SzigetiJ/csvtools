#include "CsvRow.h"
#include "CsvIStream.h"
#include <sstream>
#include <string>
#include <cassert>
#include "stl_out.h"

using namespace std;

bool test_CsvIStream(const string &obj, const Delimiters &delims, const vector<CsvRow> &expected_obj) {
  bool retv = true;

  vector<CsvRow> actual;
  stringstream ss(obj);
  CsvIStream cis(ss, delims);

  actual.push_back(cis.get_head());
  while (cis.next()) {
    actual.push_back(cis.get_current_line());
  }
  retv = (actual == expected_obj);
  if (!retv) {
    cerr<<"actual size: "<<actual.size()<<endl;
    cerr<<"expected size: "<<expected_obj.size()<<endl;
    for (unsigned int i=0; i<actual.size() && i<expected_obj.size(); ++i) {
      cerr<<"#"<<i<<" actual row: "<<actual[i].size()<<", expected row: "<<expected_obj[i].size()<<endl;
    }
  }
  return retv;
}


int main(int argc, const char *argv[]) {
  const Delimiters delims;

  CsvRow r1;
  CsvRow r2;
  r1.push_back(CsvCell("hello",false));
  r2.push_back(CsvCell("",false));
  assert(test_CsvIStream("hello\n", delims, {r1}));
  assert(test_CsvIStream("hello", delims, {r1}));
  assert(test_CsvIStream("hello\n\n", delims, {r1,r2}));

  return 0;
}
