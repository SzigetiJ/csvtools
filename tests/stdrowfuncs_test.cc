#include "RowFunc.h"
#include "stl_out.h"
#include <string>
#include <vector>
#include <cassert>

using namespace std;

bool test_RowFun(const RowFunSpec &fun, const vector<string> &args, const string &expected) {
 bool retv = true;

 auto actual = fun.exec(&args[0]);

 if (!(actual == expected)) {
  retv = false;
  cerr << "actual: [" << actual << "], expected: [" << expected << "]" << endl;
 }

 return retv;
}

int main(int argc, const char *argv[]) {

 map<string, RowFunSpec> fun_m;
 for (auto spec : std_rowfun_a) {
  fun_m.insert({spec.get_name(),spec});
 }
  assert(test_RowFun(fun_m.find("add")->second, {"0","0"},"0"));
  assert(test_RowFun(fun_m.find("add")->second, {"1","2"},"3"));
  assert(test_RowFun(fun_m.find("add")->second, {"2","1"},"3"));

  assert(test_RowFun(fun_m.find("add")->second, {"2","10hello"},"12"));

  assert(test_RowFun(fun_m.find("sub")->second, {"0","0"},"0"));
  assert(test_RowFun(fun_m.find("sub")->second, {"1","2"},"-1"));
  assert(test_RowFun(fun_m.find("sub")->second, {"2","1"},"1"));

  assert(test_RowFun(fun_m.find("mul")->second, {"0","100"},"0"));
  assert(test_RowFun(fun_m.find("mul")->second, {"3","7"},"21"));
  assert(test_RowFun(fun_m.find("mul")->second, {"19","5"},"95"));

  assert(test_RowFun(fun_m.find("div")->second, {"0","100"},"0"));
  assert(test_RowFun(fun_m.find("div")->second, {"3","7"},"0"));
  assert(test_RowFun(fun_m.find("div")->second, {"19","5"},"3"));

  assert(test_RowFun(fun_m.find("mod")->second, {"0","100"},"0"));
  assert(test_RowFun(fun_m.find("mod")->second, {"3","7"},"3"));
  assert(test_RowFun(fun_m.find("mod")->second, {"19","5"},"4"));

  assert(test_RowFun(fun_m.find("and")->second, {"0","100"},"0"));
  assert(test_RowFun(fun_m.find("and")->second, {"3","7"},"3"));
  assert(test_RowFun(fun_m.find("and")->second, {"19","5"},"1"));

  assert(test_RowFun(fun_m.find("or")->second, {"0","100"},"100"));
  assert(test_RowFun(fun_m.find("or")->second, {"3","7"},"7"));
  assert(test_RowFun(fun_m.find("or")->second, {"19","5"},"23"));

  assert(test_RowFun(fun_m.find("int")->second, {"1332.43"},"1332"));
  assert(test_RowFun(fun_m.find("int")->second, {"-42.42"},"-42"));
  assert(test_RowFun(fun_m.find("int")->second, {".5"},"0"));

  assert(test_RowFun(fun_m.find("fadd")->second, {"0","0","-1"},"0.000000")); // default precision
  assert(test_RowFun(fun_m.find("fadd")->second, {"0","0","6"},"0.000000"));
  assert(test_RowFun(fun_m.find("fadd")->second, {"0","0","7"},"0.0000000"));
  assert(test_RowFun(fun_m.find("fadd")->second, {"0.12","0.27","1"},"0.3"));
  assert(test_RowFun(fun_m.find("fadd")->second, {"0.12","0.27","2"},"0.39"));
  assert(test_RowFun(fun_m.find("fadd")->second, {"0.12","0.27","3"},"0.390"));

  assert(test_RowFun(fun_m.find("fsub")->second, {"0","0","-1"},"0.000000")); // default precision
  assert(test_RowFun(fun_m.find("fsub")->second, {"0.12","0.27","1"},"-0.1"));
  assert(test_RowFun(fun_m.find("fsub")->second, {"0.12","0.27","2"},"-0.15"));
  assert(test_RowFun(fun_m.find("fsub")->second, {"0.12","0.27","3"},"-0.150"));

  assert(test_RowFun(fun_m.find("fmul")->second, {"0","0","-1"},"0.000000")); // default precision
  assert(test_RowFun(fun_m.find("fmul")->second, {"0","0","0"},"0"));
  assert(test_RowFun(fun_m.find("fmul")->second, {"0.125","8.0","0"},"1"));
  assert(test_RowFun(fun_m.find("fmul")->second, {"0.125","8.0","1"},"1.0"));

  assert(test_RowFun(fun_m.find("fdiv")->second, {"1.0","8.0","0"},"0"));
  assert(test_RowFun(fun_m.find("fdiv")->second, {"1.0","8.0","1"},"0.1"));

  assert(test_RowFun(fun_m.find("prec")->second, {"1.0","4"},"1.0000"));
  assert(test_RowFun(fun_m.find("prec")->second, {"-1.0","4"},"-1.0000"));
  assert(test_RowFun(fun_m.find("prec")->second, {"1.00002","4"},"1.0000"));
  assert(test_RowFun(fun_m.find("prec")->second, {"-1.00002","4"},"-1.0000"));
  assert(test_RowFun(fun_m.find("prec")->second, {"1.00002","0"},"1"));
  assert(test_RowFun(fun_m.find("prec")->second, {"-1.00002","0"},"-1"));
  assert(test_RowFun(fun_m.find("prec")->second, {"0.0123","0"},"0"));
  assert(test_RowFun(fun_m.find("prec")->second, {"0.0123","1"},"0.0"));
  assert(test_RowFun(fun_m.find("prec")->second, {"0.0123","2"},"0.01"));
  assert(test_RowFun(fun_m.find("prec")->second, {"0.0123","3"},"0.012"));
  assert(test_RowFun(fun_m.find("prec")->second, {"0.0123","4"},"0.0123"));
  assert(test_RowFun(fun_m.find("prec")->second, {"0.0123","5"},"0.01230"));

  assert(test_RowFun(fun_m.find("id")->second, {"1332.43"},"1332.43"));
  assert(test_RowFun(fun_m.find("id")->second, {"-42.42"},"-42.42"));
  assert(test_RowFun(fun_m.find("id")->second, {".5"},".5"));
  assert(test_RowFun(fun_m.find("id")->second, {"HellO"},"HellO"));
  assert(test_RowFun(fun_m.find("id")->second, {"WorlD!"},"WorlD!"));

  assert(test_RowFun(fun_m.find("concat")->second, {"Hello,"," WorlD!"},"Hello, WorlD!"));
  assert(test_RowFun(fun_m.find("concat")->second, {"",""},""));

  assert(test_RowFun(fun_m.find("length")->second, {""},"0"));
  assert(test_RowFun(fun_m.find("length")->second, {"1234567890"},"10"));

  assert(test_RowFun(fun_m.find("index")->second, {"","a"},"-1"));
  assert(test_RowFun(fun_m.find("index")->second, {"","ab"},"-1"));
  assert(test_RowFun(fun_m.find("index")->second, {"hehehe", "e"},"1"));
  assert(test_RowFun(fun_m.find("index")->second, {"hehehe", "a"},"-1"));
  assert(test_RowFun(fun_m.find("index")->second, {"heheehe", "ee"},"3"));

  assert(test_RowFun(fun_m.find("substr")->second, {"","0","0"},""));
  assert(test_RowFun(fun_m.find("substr")->second, {"ab","1","1"},"b"));
  assert(test_RowFun(fun_m.find("substr")->second, {"ab","1","-1"},"b"));
  assert(test_RowFun(fun_m.find("substr")->second, {"abc","1","-1"},"bc"));

  assert(test_RowFun(fun_m.find("upper")->second, {"HellO, "},"HELLO, "));
  assert(test_RowFun(fun_m.find("upper")->second, {"WorlD!"},"WORLD!"));

  assert(test_RowFun(fun_m.find("lower")->second, {"HellO, "},"hello, "));
  assert(test_RowFun(fun_m.find("lower")->second, {"WorlD!"},"world!"));

  assert(test_RowFun(fun_m.find("ifeq")->second, {"a","a","foo","bar"},"foo"));
  assert(test_RowFun(fun_m.find("ifeq")->second, {"a","b","foo","bar"},"bar"));

  assert(test_RowFun(fun_m.find("iflt_int")->second, {"12","2","foo","bar"},"bar"));
  assert(test_RowFun(fun_m.find("iflt_str")->second, {"12","2","foo","bar"},"foo"));
  assert(test_RowFun(fun_m.find("iflt_int")->second, {"23","23","foo","bar"},"bar"));
  assert(test_RowFun(fun_m.find("iflt_str")->second, {"23","23","foo","bar"},"bar"));

  assert(test_RowFun(fun_m.find("ifle_int")->second, {"12","2","foo","bar"},"bar"));
  assert(test_RowFun(fun_m.find("ifle_str")->second, {"12","2","foo","bar"},"foo"));
  assert(test_RowFun(fun_m.find("ifle_int")->second, {"23","23","foo","bar"},"foo"));
  assert(test_RowFun(fun_m.find("ifle_str")->second, {"23","23","foo","bar"},"foo"));

  return 0;
}
