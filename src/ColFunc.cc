/*
 *  Copyright (C) 2023 SZIGETI János <szigeti at pilar dot hu>
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

#include "ColFunc.h"

using namespace std;

const AggrFunMap std_colfun_m = {
  {"sum", make_shared<AggrFunNumeric>([](Numeric &a, const string & b)->Numeric& {
      return a += b;
    })},
  {"min", make_shared<AggrFunNumeric>([](Numeric &a, const string & b)->Numeric& {
      Numeric bnum(b);
      if (bnum < a) {
        a = bnum;
      }
      return a;
    })},
  {"max", make_shared<AggrFunNumeric>([](Numeric &a, const string & b)->Numeric& {
      Numeric bnum(b);
      if (a < bnum) {
        a = bnum;
      }
      return a;
    })},
  {"count", make_shared<AggrFunCnt<int> >()},
  {"concat", make_shared<AggrFunBase<string> >([](string &a, const string & b)->string& {
      return a += b;
    })},
  {"first", make_shared<AggrFunBase<string> >([](string &a, const string & b)->string& {
      return a;
    })},
  {"last", make_shared<AggrFunBase<string> >([](string &a, const string & b)->string& {
      a = b;
      return a;
    })}
};
