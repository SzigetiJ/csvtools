#include "ColTypes.h"
#include "stl_out.h"
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

bool test_ColIvalVExtractIval(const ColIvalV &a, int n, const FieldV &expected) {
  bool retv = true;

  FieldV actual=a.extract_ival(n);

  if (actual != expected) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual<<"], expected: ["<<expected<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert(test_ColIvalVExtractIval(ColIvalV("1"),0,{1}));
  assert(test_ColIvalVExtractIval(ColIvalV("1"),10,{1}));

  assert(test_ColIvalVExtractIval(ColIvalV("-1"),0,{0,1}));
  assert(test_ColIvalVExtractIval(ColIvalV("-1"),1,{0,1}));

  assert(test_ColIvalVExtractIval(ColIvalV("-1,3-"),1,{0,1}));
  assert(test_ColIvalVExtractIval(ColIvalV("-1,3-"),5,{0,1,3,4}));

  assert(test_ColIvalVExtractIval(ColIvalV("3-2"),0,{}));
  assert(test_ColIvalVExtractIval(ColIvalV("3-2"),5,{}));

  assert(test_ColIvalVExtractIval(ColIvalV("-1,0-1,2,1-3,4-,5-"),5,{0,1,0,1,2,1,2,3,4}));

  return 0;
}
