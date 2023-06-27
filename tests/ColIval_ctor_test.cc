#include "ColTypes.h"
#include <string>
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

bool test_ColIvalParse(const char *a, const pair<ColRef,ColRef> &expected, bool exp_success) {
  bool retv = true;

  ColIval actual;
  int act_result = actual.parse(a, strlen(a));

  if (exp_success != (act_result==0)) {
   retv = false;
   cerr << "Success/failure mismatch. Actual return: ["<<act_result<<"], expected success: ["<<(exp_success?"true":"false")<<"]"<<endl;
  }
  if (actual.first != expected.first || actual.second != expected.second) {
    retv = false;
    cerr<<"Diff. actual: ["<<actual.first.first<<'.'<<actual.first.second<<","<<actual.second.first<<'.'<<actual.second.second
            <<"], expected: ["<<expected.first.first<<'.'<<expected.first.second<<","<<expected.second.first<<'.'<<expected.second.second<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert((ColIval({false, 1})==pair<ColRef,ColRef>({false, 1},{false, 1})));

  assert(test_ColIvalParse("1",pair<ColRef,ColRef>({false, 1},{false, 1}),true));
  assert(test_ColIvalParse("1-1",pair<ColRef,ColRef>({false, 1},{false, 1}),true));
  assert(test_ColIvalParse("0-1",pair<ColRef,ColRef>({false, 0},{false, 1}),true));
  assert(test_ColIvalParse("1-0",pair<ColRef,ColRef>({false, 1},{false, 0}),true));

  assert(test_ColIvalParse("1-",pair<ColRef,ColRef>({false, 1},{false, COLID_UNDEF}),true));
  assert(test_ColIvalParse("-1",pair<ColRef,ColRef>({false, COLID_UNDEF},{false, 1}),true));
  assert(test_ColIvalParse("-",pair<ColRef,ColRef>({false, COLID_UNDEF},{false, COLID_UNDEF}),true));

  assert(test_ColIvalParse("",pair<ColRef,ColRef>({false, COLID_UNDEF},{false, COLID_UNDEF}),true));

  assert(test_ColIvalParse("123",pair<ColRef,ColRef>({false, 123},{false, 123}),true));

  assert(test_ColIvalParse("1+",pair<ColRef,ColRef>({false, 1},{false, 1}),false));

  assert(test_ColIvalParse(".",pair<ColRef,ColRef>({false, COLID_UNDEF},{false, COLID_UNDEF}),false));
  assert(test_ColIvalParse("1--",pair<ColRef,ColRef>({false, 1},{false, COLID_UNDEF}),false));

  return 0;
}
