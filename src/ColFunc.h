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

#ifndef COLFUNC_H
#define COLFUNC_H

#include <string>
#include <map>
#include <memory>
#include <functional>
#include "Numeric.h"

/// General interface for aggregation (column) functions.
class AggrFunIface {
public:
 virtual void *init_value(const std::string&) const = 0;
 virtual void *accumulate(void*, const std::string&) const = 0;
 virtual std::string to_string(void*) const = 0;
 virtual void delete_value(void*) const = 0;
 virtual ~AggrFunIface() = default;
};

typedef std::shared_ptr<AggrFunIface> AggrFunSmartPtr;
typedef AggrFunIface* AggrFunPtr;

/// Base implementation of aggregation interface.
/// It can be used with any type that supports T->string conversion.
/// Accumulator function MUST be given,
/// initializer function MAY be given (defaults to T(string&) constructor).
template<typename T>
class AggrFunBase : public AggrFunIface {
 std::function<T & (T&, const std::string&)> accumulator;
 std::function<void * (const std::string&)> initializer;
public:
 explicit AggrFunBase(const std::function<T & (T&, const std::string&)> &a, const std::function<void * (const std::string&)> &b=[](const std::string &a)->void*{return new T(a);}):accumulator(a),initializer(b){}
 void *init_value(const std::string &a) const {return initializer(a);}
 void *accumulate(void *a, const std::string &b) const {
  T &item=*(reinterpret_cast<T*>(a));
  accumulator(item,b);
  return &item;
 }
 virtual std::string to_string(void *a) const { return *(reinterpret_cast<std::string*>(a));}
 void delete_value(void *a) const {delete reinterpret_cast<T*>(a);}
};

/// Aggregation function dealing with numeric values.
class AggrFunNumeric : public AggrFunBase<Numeric> {
public:
 explicit AggrFunNumeric(const std::function<Numeric& (Numeric&, const std::string&)> &a):AggrFunBase<Numeric>(a){}
 std::string to_string(void *a) const {
  return reinterpret_cast<Numeric*>(a)->to_string();
 }
};

/// Aggregation for counting. Use with type T=int|unsigned|long...
template<typename T>
class AggrFunCnt : public AggrFunBase<T> {
public:
 AggrFunCnt():AggrFunBase<T>([](T &a, const std::string &b)->T&{return b.empty()?a:++a;},[](const std::string &a)->void*{return new T(a.empty()?0:1);}){}
 std::string to_string(void *a) const {
  return std::to_string(*reinterpret_cast<T*>(a));
 }
};

/// Represents an aggregated value.
/// Internally the aggregated value can be of ANY type.
/// There are 3 requirements against the aggregated value:
/// 1.) it must accept a string as initial value;
/// 2.) it must accept a string value for accumulation;
/// 3.) it must present a string value as result.
class AggrValue {
 const AggrFunPtr fun;
 bool has_dat = false;
 void *dat = NULL;
public:
 explicit AggrValue(const AggrFunPtr &f):fun(f) {};
 bool init(const std::string &a){if (!has_dat) {dat=fun->init_value(a); has_dat=true; return true;} return false;};
 void accumulate(const std::string &a){fun->accumulate(dat,a);};
 std::string get_dat(){return fun->to_string(dat);};
 bool cleanup(){if (has_dat) {fun->delete_value(dat);has_dat=false; return true;} return false;};
 bool valid_dat() const {return has_dat;};
};

typedef std::map<std::string,AggrFunSmartPtr> AggrFunMap;

extern const AggrFunMap std_colfun_m;

#endif /* COLFUNC_H */

