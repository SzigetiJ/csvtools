#include "CommandLine.h"
#include <sstream>
#include <string>
#include <cassert>
#include "stl_out.h"

using namespace std;

bool test_Option_print(const Option &obj, const string &expected) {
 bool retv = true;

 stringstream ss;
 ss << obj;
 string actual = ss.str();

 retv = (actual == expected);
 if (!retv) {
  cerr << "actual: " << actual << endl;
  cerr << "expected: " << expected << endl;
 }
 return retv;
}

bool vvchrptr_eq(const vector<vector<const char*>> &a, const vector<vector<const char*>> &b) {
 bool retv = true;
 if (a.size()!=b.size()) return false;
 for (unsigned int i = 0; i<a.size(); ++i) {
  if (a[i].size()!=b[i].size()) return false;
  for (unsigned int j =0; j< a[i].size(); ++j) {
   if (strcmp(a[i][j],b[i][j])) return false;
  }
 }
 return retv;
}

bool test_CommandLine_getopt(CommandLine *obj, const Option &check_option, bool expected_found, const vector<vector<const char*> > &expected_values) {
 bool retv = true;

 bool actual_found_flag = obj->is_set_flag(check_option.flag);
 bool actual_found_longname = obj->is_set_longname(check_option.longname);
 bool actual_found_option = obj->is_set_option(check_option);

 auto actual_val_flag = obj->get_values_for_flag(check_option.flag);
 auto actual_val_longname = obj->get_values_for_longname(check_option.longname);
 auto actual_val_option = obj->get_values_for_option(check_option);

 if (actual_found_flag != expected_found) {
  cerr << "is_set_flag mismatch\tflag: " << check_option.flag << "\tactual: " << actual_found_flag << "\texpected: " << expected_found << endl;
  retv = false;
 }
 if (actual_found_longname != expected_found) {
  cerr << "is_set_longname mismatch\tlongname: " << check_option.longname << "\tactual: " << actual_found_longname << "\texpected: " << expected_found << endl;
  retv = false;
 }
 if (actual_found_option != expected_found) {
  cerr << "is_set_option mismatch\toption: " << check_option << "\tactual: " << actual_found_option << "\texpected: " << expected_found << endl;
  retv = false;
 }
 if (!vvchrptr_eq(actual_val_flag,expected_values)) {
  cerr<<"get_values_for_flag mismatch\tflag: "<<check_option.flag<<endl;
  cerr<<"actual: "<<actual_val_flag<<" vs. expected: "<<expected_values<<endl;
  retv = false;
 }
 if (!vvchrptr_eq(actual_val_longname,expected_values)) {
  cerr<<"get_values_for_longname mismatch\tlongname: "<<check_option.longname<<endl;
  retv = false;
 }
 if (!vvchrptr_eq(actual_val_option,expected_values)) {
  cerr<<"get_values_for_option mismatch\toption: "<<check_option<<endl;
  retv = false;
 }
 if (retv) {
  for (unsigned int i = 0; i<check_option.arg_num; ++i) {
   auto actual_flag_val_i = obj->get_arg_for_flag(check_option.flag,i);
   auto actual_longname_val_i = obj->get_arg_for_longname(check_option.longname,i);
   auto actual_option_val_i = obj->get_arg_for_option(check_option,i);
   if (expected_values.empty() || (expected_values[0].size() <= i)) {
    if (actual_flag_val_i!=nullptr) {
     cerr<<"get_arg_for_flag should have return nullptr\tflag"<<check_option.flag<<" arg #"<<i<<"actual: "<<actual_flag_val_i<<endl;
     retv = false;
    }
    if (actual_longname_val_i!=nullptr) {
     cerr<<"get_arg_for_longname should have return nullptr\tlongname"<<check_option.longname<<" arg #"<<i<<"actual: "<<actual_longname_val_i<<endl;
     retv = false;
    }
    if (actual_option_val_i!=nullptr) {
     cerr<<"get_arg_for_option should have return nullptr\toption"<<check_option<<" arg #"<<i<<"actual: "<<actual_option_val_i<<endl;
     retv = false;
    }
   } else {
    if (strcmp(actual_flag_val_i, expected_values[0][i])) {
     cerr<<"get_arg_for_flag mismatch\tflag "<<check_option.flag<<" arg #"<<i<<"\t actual: "<<actual_flag_val_i<<" vs. expected: "<<expected_values[0][i]<<endl;
     retv = false;
    }
    if (strcmp(actual_longname_val_i, expected_values[0][i])) {
     cerr<<"get_arg_for_longname mismatch\tlongname "<<check_option.longname<<" arg #"<<i<<"\t actual: "<<actual_longname_val_i<<" vs. expected: "<<expected_values[0][i]<<endl;
     retv = false;
    }
    if (strcmp(actual_option_val_i, expected_values[0][i])) {
     cerr<<"get_arg_for_option mismatch\toption "<<check_option<<" arg #"<<i<<"\t actual: "<<actual_option_val_i<<" vs. expected: "<<expected_values[0][i]<<endl;
     retv = false;
    }
   }
  }
 }
 return retv;
}

class TestCommandLine : public CommandLine {

 int process() {
  return 0;
 }
public:

 TestCommandLine(const set<Option> &_opts) : CommandLine(_opts) {
 }

 CommandLineExecuteResponse execute(int argc, const char *a[]) {
  parse(argc, a);
  return CommandLineExecuteResponse::CMDLINE_OK;
 }
};

int main(int argc, const char *argv[]) {
 Option opt1 = {"a", "append", 2, MultipleDefinition::APPEND, "This is opt1.\n"};
 Option opt1_noflag = {nullptr, "append", 2, MultipleDefinition::APPEND, "This is opt1.\n"};
 Option opt1_nolongname = {"a", nullptr, 2, MultipleDefinition::APPEND, "This is opt1.\n"};
 Option opt1_nohelp = {"a", "append", 2, MultipleDefinition::APPEND, nullptr};
 Option opt1_nothing = {nullptr, nullptr, 2, MultipleDefinition::APPEND, nullptr};

 assert(test_Option_print(opt1, "-a\t--append=<arg1> <arg2>\tThis is opt1.\n"));
 assert(test_Option_print(opt1_noflag, "\t--append=<arg1> <arg2>\tThis is opt1.\n"));
 assert(test_Option_print(opt1_nolongname, "-a\tThis is opt1.\n"));
 assert(test_Option_print(opt1_nohelp, "-a\t--append=<arg1> <arg2>"));
 assert(test_Option_print(opt1_nothing, ""));

 Option opts[] = {
  {"a", "alpha", 0, MultipleDefinition::IGNORE, "This is option alpha."},
  {"b", "bravo", 1, MultipleDefinition::IGNORE, "This is option bravo."},
  {"c", "charlie", 2, MultipleDefinition::OVERRIDE, "This is option charlie."},
  {"d", "delta", 1, MultipleDefinition::OVERRIDE, "This is option delta."}
 };
 TestCommandLine cmdline0(set<Option>(opts, opts + sizeof (opts) / sizeof (opts[0])));
 TestCommandLine cmdline1=cmdline0;
 const char *cmdline0args[] = {"progname", "-a", "-b", "1"};
 cmdline0.execute(4, cmdline0args);
 const char *cmdline1args[] = {"progname", "-b", "1", "-b", "2", "--charlie=a", "b", "--charlie=c", "d"};
 cmdline1.execute(9, cmdline1args);

 assert(test_CommandLine_getopt(&cmdline0, opts[0], true, {{}}));
 assert(test_CommandLine_getopt(&cmdline0, opts[1], true, {{"1"}}));
 assert(test_CommandLine_getopt(&cmdline0, opts[2], false, {}));
 assert(test_CommandLine_getopt(&cmdline0, opts[3], false, {}));

 assert(test_CommandLine_getopt(&cmdline0, opt1_noflag, false, {}));
 assert(test_CommandLine_getopt(&cmdline0, opt1_nothing, false, {}));


 assert(test_CommandLine_getopt(&cmdline1, opts[0], false, {}));
 assert(test_CommandLine_getopt(&cmdline1, opts[1], true, {{"1"}}));
 assert(test_CommandLine_getopt(&cmdline1, opts[2], true, {{"c","d"}}));
 assert(test_CommandLine_getopt(&cmdline1, opts[3], false, {}));

 return 0;
}
