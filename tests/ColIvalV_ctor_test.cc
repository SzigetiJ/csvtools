#include "ColTypes.h"
#include "stl_out.h"
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

bool test_ColIvalVCtor(const char *a, const vector<ColIval> &expected) {
  bool retv = true;

  ColIvalV actual(false);
  actual.parse(a);

  if (actual != expected) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual<<"], expected: ["<<expected<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert(test_ColIvalVCtor("",{}));
  assert(test_ColIvalVCtor("1",{ColIval(1)}));
  assert(test_ColIvalVCtor("1,2",{ColIval(1),ColIval(2)}));

  assert(test_ColIvalVCtor("-1,3-",
  {ColIval(COLID_UNDEF,1),ColIval(3,COLID_UNDEF)}));
  assert(test_ColIvalVCtor("-1,,3-",
  {ColIval(COLID_UNDEF,1),ColIval(COLID_UNDEF),ColIval(3,COLID_UNDEF)}));

  return 0;
}
