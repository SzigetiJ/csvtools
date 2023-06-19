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

bool test_CsvIStreamMap(const string &obj, const Delimiters &delims, const vector<map<string,CsvCell> > &expected_rows) {
  bool retv = true;

  vector<map<string, CsvCell> > actual;
  stringstream ss(obj);
  CsvIStream cis(ss, delims);

  while (cis.next()) {
    actual.push_back(cis.get_current_line_m());
  }
  retv = (actual == expected_rows);
  if (!retv) {
    cerr<<"actual size: "<<actual.size()<<endl;
    cerr<<"expected size: "<<expected_rows.size()<<endl;
    for (unsigned int i=0; i<actual.size() && i<expected_rows.size(); ++i) {
      cerr<<"#"<<i<<" actual row: "<<actual[i].size()<<", expected row: "<<expected_rows[i].size()<<endl;
    }
  }
  return retv;
}

bool test_CsvIStreamRecords(const string &obj, const Delimiters &delims, const int &expected_records) {
  stringstream ss(obj);
  CsvIStream cis(ss, delims);

  while (cis.next()) {
  }

  int actual_records = cis.get_record_cnt();
  bool retv = (actual_records == expected_records);
  if (!retv) {
    cerr<<"records mismatch (actual:"<< actual_records<<", expected: "<<expected_records<<")."<<endl;
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

  assert(test_CsvIStreamMap("a,b\n1,2\n3,4\n",delims,{{{"a",1},{"b",2}},{{"a",3},{"b",4}}}));
  assert(test_CsvIStreamMap("a,b\n\"1\",2\n3,\"4\"\n",delims,{{{"a","1"},{"b",2}},{{"a",3},{"b","4"}}}));

  assert(test_CsvIStreamRecords("a,b\n\"1\",2\n3,\"4\"\n",delims,2));
  assert(test_CsvIStreamRecords("a,b\n\"1,2\n3\",\"4\"\n",delims,1));
  return 0;
}
