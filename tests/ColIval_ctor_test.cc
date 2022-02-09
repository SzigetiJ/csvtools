#include "ColTypes.h"
#include <string>
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

bool test_ColIvalParse(const char *a, const pair<ColID,ColID> &expected, bool exp_success) {
  bool retv = true;

  ColIval actual;
  int act_result = actual.parse(a, strlen(a));

  if (exp_success != (act_result==0)) {
   retv = false;
   cerr << "Success/failure mismatch. Actual return: ["<<act_result<<"], expected success: ["<<(exp_success?"true":"false")<<"]"<<endl;
  }
  if (actual.first != expected.first || actual.second != expected.second) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual.first<<","<<actual.second<<"], expected: ["<<expected.first<<","<<expected.second<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert((ColIval(1)==pair<ColID,ColID>(1,1)));

  assert(test_ColIvalParse("1",pair<ColID,ColID>(1,1),true));
  assert(test_ColIvalParse("1-1",pair<ColID,ColID>(1,1),true));
  assert(test_ColIvalParse("0-1",pair<ColID,ColID>(0,1),true));
  assert(test_ColIvalParse("1-0",pair<ColID,ColID>(1,0),true));

  assert(test_ColIvalParse("1-",pair<ColID,ColID>(1,COLID_UNDEF),true));
  assert(test_ColIvalParse("-1",pair<ColID,ColID>(COLID_UNDEF,1),true));
  assert(test_ColIvalParse("-",pair<ColID,ColID>(COLID_UNDEF,COLID_UNDEF),true));

  assert(test_ColIvalParse("",pair<ColID,ColID>(COLID_UNDEF,COLID_UNDEF),true));

  assert(test_ColIvalParse("123",pair<ColID,ColID>(123,123),true));

  assert(test_ColIvalParse("1+",pair<ColID,ColID>(1,1),false));

  assert(test_ColIvalParse(".",pair<ColID,ColID>(COLID_UNDEF,COLID_UNDEF),false));
  assert(test_ColIvalParse("1--",pair<ColID,ColID>(1,COLID_UNDEF),false));

  return 0;
}
