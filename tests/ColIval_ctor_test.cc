#include "ColTypes.h"
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

bool test_ColIvalCtor(const char *a, const pair<ColID,ColID> &expected) {
  bool retv = true;

  ColIval actual(a);

  if (actual.first != expected.first || actual.second != expected.second) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual.first<<","<<actual.second<<"], expected: ["<<expected.first<<","<<expected.second<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert((ColIval(1)==pair<ColID,ColID>(1,1)));

  assert(test_ColIvalCtor("1",pair<ColID,ColID>(1,1)));
  assert(test_ColIvalCtor("1-1",pair<ColID,ColID>(1,1)));
  assert(test_ColIvalCtor("0-1",pair<ColID,ColID>(0,1)));
  assert(test_ColIvalCtor("1-0",pair<ColID,ColID>(1,0)));

  assert(test_ColIvalCtor("1-",pair<ColID,ColID>(1,COLID_UNDEF)));
  assert(test_ColIvalCtor("-1",pair<ColID,ColID>(COLID_UNDEF,1)));
  assert(test_ColIvalCtor("-",pair<ColID,ColID>(COLID_UNDEF,COLID_UNDEF)));

  assert(test_ColIvalCtor("",pair<ColID,ColID>(COLID_UNDEF,COLID_UNDEF)));

  assert(test_ColIvalCtor("123",pair<ColID,ColID>(123,123)));

  assert(test_ColIvalCtor("1+",pair<ColID,ColID>(1,1)));

  // invalid arg
//  assert(test_ColIvalCtor(".",pair<ColID,ColID>(COLID_UNDEF,COLID_UNDEF)));
//  assert(test_ColIvalCtor("1--",pair<ColID,ColID>(1,COLID_UNDEF)));

  return 0;
}
