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
  assert(test_ColIvalVCtor("1",{ColIval({false,1})}));
  assert(test_ColIvalVCtor("1,2",{ColIval({false,1}),ColIval({false,2})}));

  assert(test_ColIvalVCtor("-1,3-",
  {ColIval({false,COLID_UNDEF},{false,1}),ColIval({false,3},{false,COLID_UNDEF})}));
  assert(test_ColIvalVCtor("-1,,3-",
  {ColIval({false,COLID_UNDEF},{false,1}),ColIval({false,COLID_UNDEF}),ColIval({false,3},{false,COLID_UNDEF})}));

  assert(test_ColIvalVCtor("r",{ColIval({true,0})}));
  assert(test_ColIvalVCtor("r-r",{ColIval({true,0})}));
  assert(test_ColIvalVCtor("-r",{ColIval({false,COLID_UNDEF},{true,0})}));
  assert(test_ColIvalVCtor("r-",{ColIval({true,0},{false,COLID_UNDEF})}));
  return 0;
}
