/*
 *  Copyright (C) 2014 -- 2023 SZIGETI János <szigeti at pilar dot hu>
 *
 *  This file is part of CsvTools.
 *
 *  CsvTools is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  CsvTools is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 * along with CsvTools. If not, see http://www.gnu.org/licenses/.
 */
#include "RowFunc.h"
#include "Numeric.h"

using namespace std;

RowFunSpec::RowFunSpec(const char *_name, const char *_desc, unsigned int _argc, const RowFun &_fun) : name(_name), desc(_desc), argc(_argc), fun(_fun) {
}

const char *RowFunSpec::get_name() const {
 return name;
}

const char *RowFunSpec::get_desc() const {
 return desc;
}

unsigned int RowFunSpec::argnum() const {
 return argc;
}

std::string RowFunSpec::exec(const std::string *_argv) const {
 return fun(_argv);
}

const RowFunSpecs std_rowfun_a = {
 RowFunSpec("add","arg0 + arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) + stoi(a[1]));}),
 RowFunSpec("sub","arg0 - arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) - stoi(a[1]));}),
 RowFunSpec("mul","arg0 * arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) * stoi(a[1]));}),
 RowFunSpec("div","arg0 / arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) / stoi(a[1]));}),
 RowFunSpec("mod","arg0 % arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) % stoi(a[1]));}),
 RowFunSpec("or","arg0 | arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) | stoi(a[1]));}),
 RowFunSpec("and","arg0 & arg1,\tint(int, int)", 2, [](const string * a) {return to_string(stoi(a[0]) & stoi(a[1]));}),
 RowFunSpec("int","int(arg0),\tint(num)", 1, [](const string * a) {return to_string((int)stof(a[0]));}),

 RowFunSpec("fadd","arg0 + arg1 with precision,\tnum(num, num, int)", 3, [](const string * a) {string retv = to_string(stod(a[0]) + stod(a[1])); int prec = stoi(a[2]); if (prec!=-1) {retv = Numeric(retv).set_prec(prec).to_string();} return retv;}),
 RowFunSpec("fsub","arg0 - arg1 with precision,\tnum(num, num, int)", 3, [](const string * a) {string retv = to_string(stod(a[0]) - stod(a[1])); int prec = stoi(a[2]); if (prec!=-1) {retv = Numeric(retv).set_prec(prec).to_string();} return retv;}),
 RowFunSpec("fmul","arg0 * arg1 with precision,\tnum(num, num, int)", 3, [](const string * a) {string retv = to_string(stod(a[0]) * stod(a[1])); int prec = stoi(a[2]); if (prec!=-1) {retv = Numeric(retv).set_prec(prec).to_string();} return retv;}),
 RowFunSpec("fdiv","arg0 / arg1 with precision,\tnum(num, num, int)", 3, [](const string * a) {string retv = to_string(stod(a[0]) / stod(a[1])); int prec = stoi(a[2]); if (prec!=-1) {retv = Numeric(retv).set_prec(prec).to_string();} return retv;}),
 RowFunSpec("prec","setprec(arg0,arg1),\tnum(num, int)", 2, [](const string * a) {return Numeric(a[0]).set_prec(stoul(a[1])).to_string();}),


 RowFunSpec("id","arg0,\tstring(string)", 1, [](const string * a) {return string(a[0]);}),
 RowFunSpec("concat","arg0.append(arg1),\tstring(string,string)", 2, [](const string * a) {return string(a[0]).append(a[1]);}),
 RowFunSpec("length","arg0.length(),\tint(string)", 1, [](const string * a) {return to_string(a[0].length());}),
 RowFunSpec("index","arg0.find(arg1),\tint(string, string)", 2, [](const string * a) {return to_string((int)a[0].find(a[1]));}),
 RowFunSpec("substr","arg0.substr(arg1, arg2),\tstring(string, int, int)", 3, [](const string * a) {return a[0].substr(stoi(a[1]),stoi(a[2]));}),
 RowFunSpec("upper","arg0.to_upper(),\tstring(string)", 1, [](const string * a) {auto retv = a[0]; for (unsigned int i=0; i<retv.size(); ++i) retv.at(i) = std::toupper(retv.at(i));return retv;}),
 RowFunSpec("lower","arg0.to_lower(),\tstring(string)", 1, [](const string * a) {auto retv = a[0]; for (unsigned int i=0; i<retv.size(); ++i) retv.at(i) = std::tolower(retv.at(i));return retv;}),

 RowFunSpec("ifeq","arg0==arg1?arg2:arg3,\tstring(string, string, string, string)", 4, [](const string * a) {return a[0]==a[1]?a[2]:a[3];}),
 RowFunSpec("iflt_str","arg0<arg1?arg2:arg3,\tstring(string, string, string, string)", 4, [](const string * a) {return a[0]<a[1]?a[2]:a[3];}),
 RowFunSpec("iflt_int","(int)arg0<(int)arg1?arg2:arg3,\tstring(int, int, string, string)", 4, [](const string * a) {return stoi(a[0])<stoi(a[1])?a[2]:a[3];}),
 RowFunSpec("ifle_str","arg0<=arg1?arg2:arg3,\tstring(string, string, string, string)", 4, [](const string * a) {return a[0]<=a[1]?a[2]:a[3];}),
 RowFunSpec("ifle_int","(int)arg0<=(int)arg1?arg2:arg3,\tstring(int, int, string, string)", 4, [](const string * a) {return stoi(a[0])<=stoi(a[1])?a[2]:a[3];})
};
