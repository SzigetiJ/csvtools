/*
 *  Copyright (C) 2014 SZIGETI János <szigeti at pilar dot hu>
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
#include "Numeric.h"

using namespace std;

int Numeric::digits_to_num(int a) {
 int retv=1;
 for (auto i=0;i<a;++i){retv*=10;}
 return retv;
}

/// Standard constuctor as string parser.
Numeric::Numeric(const string &a) {
 val=0;
 prec=0;
 size_t idx;
 bool negative=false;
 size_t start=0;
 try {
  if (a.at(start)=='-'){
   negative=true;
   ++start;
  }
  if (a.at(start)=='.')
   idx=start;
  else {
   val=stoi(a.substr(start),&idx);
   idx+=start;
  }
  if (idx!=a.size()){
   int xx=stoi(a.substr(idx+1),&prec);
   val*=digits_to_num(prec);
   val+=xx;
  }
  val*=(negative?-1:1);
 } catch(...){
 }
}

string Numeric::to_string() const {
 auto str = std::to_string(val);
 bool put_zero = (prec+(val<0?1:0) == str.size());
 return prec?
  (str.substr(0,str.size()-prec) +
   (put_zero?"0":"") + "." +
   str.substr(str.size()-prec)):
  str;
}

Numeric &Numeric::operator+=(const string &a){
 Numeric tmp(a);
 while (prec<tmp.prec) {
  val*=10;
  ++prec;
 }
 while (tmp.prec<prec) {
  tmp.val*=10;
  ++tmp.prec;
 }
 val+=tmp.val;
 return *this;
}

/// Very simple implementation of comparison.
bool Numeric::operator<(const Numeric &x) const {
 Numeric a(*this);
 Numeric b(x);
 while (a.prec<b.prec) {
  a.val*=10;
  ++a.prec;
 }
 while (b.prec<a.prec) {
  b.val*=10;
  ++b.prec;
 }
 return a.val<b.val; 
}
