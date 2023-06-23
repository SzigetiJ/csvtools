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
#include <sstream>
#include "Numeric.h"

using namespace std;

constexpr char NEG_SIGN = '-';
constexpr char DECIMAL_SEP = '.';
const string ZERO_STR = "0";
constexpr int BASE = 10;

int Numeric::digits_to_num(int a) {
  int retv = 1;
  for (auto i = 0; i < a; ++i) {
    retv *= BASE;
  }
  return retv;
}

/// Standard constuctor as string parser.
Numeric::Numeric(const string &a) {
  val = 0;
  prec = 0;
  try {
    size_t idx;
    size_t start = 0;
    bool negative = false;
    if (a.at(start) == NEG_SIGN) {
      negative = true;
      ++start;
    }
    if (a.at(start) == DECIMAL_SEP)
      idx = start;
    else {
      val = stoi(a.substr(start), &idx);
      idx += start;
    }
    if (idx != a.size()) {
      int xx = stoi(a.substr(idx + 1), &prec);
      val *= digits_to_num(prec);
      val += xx;
    }
    val *= (negative ? -1 : 1);
  } catch (...) {
  }
}

string Numeric::to_string() const {
  auto str = std::to_string(val);
  bool is_signed = (val < 0);
  int int_digits = str.size() - is_signed - prec;
  stringstream ss;
  if (is_signed) ss<<NEG_SIGN;
  size_t str_ptr = is_signed;
  if (0 < int_digits) {ss<<str.substr(str_ptr,int_digits); str_ptr+=int_digits;}
  else ss<<ZERO_STR;
  if (prec == 0) return ss.str();
  ss<<DECIMAL_SEP;
  size_t dec_digits = 0;
  while ((int_digits + (int)dec_digits) < 0 && dec_digits < prec) {ss<<ZERO_STR; ++dec_digits;}
  while (dec_digits < prec && str_ptr < str.size()) {ss<<str.at(str_ptr);++dec_digits; ++str_ptr;}
  while (dec_digits < prec) {ss<<ZERO_STR; ++dec_digits;}
  return ss.str();
}

Numeric &Numeric::set_prec(size_t _prec) {
 while (prec < _prec) {
  val *= BASE;
  ++prec;
 }
 while (_prec < prec) {
  val /= BASE;
  --prec;
 }
 return *this;
}

Numeric &Numeric::operator+=(const string &a) {
  Numeric tmp(a);
  while (prec < tmp.prec) {
    val *= 10;
    ++prec;
  }
  while (tmp.prec < prec) {
    tmp.val *= 10;
    ++tmp.prec;
  }
  val += tmp.val;
  return *this;
}

/// Very simple implementation of comparison.
bool Numeric::operator<(const Numeric &x) const {
  Numeric a(*this);
  Numeric b(x);
  while (a.prec < b.prec) {
    a.val *= 10;
    ++a.prec;
  }
  while (b.prec < a.prec) {
    b.val *= 10;
    ++b.prec;
  }
  return a.val < b.val;
}
