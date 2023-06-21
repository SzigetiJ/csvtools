#include "CommandLine.h"
#include "stl_out.h"
#include <string>
#include <vector>
#include <cassert>

using namespace std;

bool test_StrSplit(const string &orig, int parts, const vector<string> &expected) {
  bool retv = true;

  auto actual = str_split(orig, ':', parts);

  if (!(actual == expected)) {
    retv = false;
    cerr<<"actual size: "<<actual.size()<<", expected size: "<<expected.size()<<endl;
    cerr<<"actual: ["<<actual<<"], expected: ["<<expected<<"]"<<endl;
  }

  return retv;
}

int main(int argc, const char *argv[]) {

  assert(test_StrSplit("",0,{""}));
  assert(test_StrSplit("",1,{""}));
  assert(test_StrSplit("",2,{""}));

  assert(test_StrSplit("a",0,{"a"}));
  assert(test_StrSplit("a",1,{"a"}));
  assert(test_StrSplit("a",2,{"a"}));

  assert(test_StrSplit(":",0,{":"}));
  assert(test_StrSplit(":",1,{":"}));
  assert(test_StrSplit(":",2,{"",""}));

  assert(test_StrSplit("a:",0,{"a:"}));
  assert(test_StrSplit("a:",1,{"a:"}));
  assert(test_StrSplit("a:",2,{"a",""}));
  assert(test_StrSplit("a:",3,{"a",""}));

  assert(test_StrSplit(":a",0,{":a"}));
  assert(test_StrSplit(":a",1,{":a"}));
  assert(test_StrSplit(":a",2,{"","a"}));
  assert(test_StrSplit(":a",3,{"","a"}));

  assert(test_StrSplit(":a:",0,{":a:"}));
  assert(test_StrSplit(":a:",1,{":a:"}));
  assert(test_StrSplit(":a:",2,{"","a:"}));
  assert(test_StrSplit(":a:",3,{"","a",""}));

  return 0;
}
