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
#ifndef _ROWFUNC_H_
#define _ROWFUNC_H_
#include <functional>
#include <string>

/// Generic row function
typedef std::function<std::string (const std::string*) > RowFun;

class RowFunSpec {
 const char *name;
 const char *desc;
 unsigned int argc;
 RowFun fun;
public:
 RowFunSpec(const char *_name, const char *_desc, unsigned int _argc, const RowFun &_fun);
 const char *get_name() const;
 const char *get_desc() const;
 unsigned int argnum() const;
 std::string exec(const std::string *_argv) const;
};

typedef std::initializer_list<RowFunSpec> RowFunSpecs;

extern const RowFunSpecs std_rowfun_a;

#endif
