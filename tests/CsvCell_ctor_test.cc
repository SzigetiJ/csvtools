#include "CsvCell.h"
#include <string>
#include <cassert>
#include <climits>

using namespace std;

bool test_CsvCellEquals(const CsvCell &actual, const string &expected_dat, const bool &expected_quoted)
{
  bool retv_dat=(actual.get_dat() == expected_dat);
  bool retv_quoted=(actual.is_quoted() == expected_quoted);

  bool retv = retv_dat && retv_quoted;
  if (!retv)
  {
    cerr << "Diff. actual: [" << actual.get_dat() << ',' << actual.is_quoted() << "], expected: [" << expected_dat << ',' << expected_quoted << "]" << endl;
  }

  return retv;
}

int main(int argc, const char *argv[])
{

  assert(test_CsvCellEquals(CsvCell(), "", false));
  // move ctor
  assert(test_CsvCellEquals(CsvCell(string("a")), "a", true));
  // copy ctor
  string b("b");
  assert(test_CsvCellEquals(CsvCell(b), "b", true));
  // std ctor (move)
  assert(test_CsvCellEquals(CsvCell(string("c0"), true), "c0", true));
  assert(test_CsvCellEquals(CsvCell(string("c1"), false), "c1", false));
  // std ctor (copy)
  string d0("d0");
  string d1("d1");
  assert(test_CsvCellEquals(CsvCell(d0, true), "d0", true));
  assert(test_CsvCellEquals(CsvCell(d1, false), "d1", false));
  // c_str conversion
  char e1[]="e1";
  assert(test_CsvCellEquals(CsvCell("e0"), "e0", true));
  assert(test_CsvCellEquals(CsvCell(e1), "e1", true));
  // number conversion
  assert(test_CsvCellEquals(CsvCell(-1), "-1", false));
  assert(test_CsvCellEquals(CsvCell(0), "0", false));
  assert(test_CsvCellEquals(CsvCell(INT_MAX), to_string(INT_MAX), false));
  assert(test_CsvCellEquals(CsvCell(INT_MIN), to_string(INT_MIN), false));

  return 0;
}
