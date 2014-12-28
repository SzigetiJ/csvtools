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
#include <cstdlib>
#include <algorithm>
#include "CsvCell.h"
#include "globals.h"

using namespace std;

/// States of csv parser
typedef enum {
 PSTART,	///< parse begins
 PRAW,	///< raw field, unescaped
 PSTR,	///< we are parsing a string (waiting for string delimiter)
 PSTRX,	///< within a string we have found a string delimiter. This means that either the string is over or a delimiter character is encoded in the string.
 PENDF,	///< we have parsed end-of-field
} PState;

/// Standard constructor.
CsvCell::CsvCell(const string &a, bool b):dat(a),quote(b){
};
CsvCell::CsvCell():dat(),quote(false){};

/// Cell datum getter
string CsvCell::get_dat() const {
 return dat;
};

/// Sets the quote flag.
/// @param a Quote flag.
void CsvCell::set_escaped(bool a){
 quote=a;
};

/// Some locales use decimal comma instead of decimal point.
/// This funciton replaces comma characters to points.
void CsvCell::to_decimal(){
 replace(dat.begin(),dat.end(),',','.');
};

/// Determines whether the cell must be escaped at the input/output or not.
/// \param a true: output, false: input.
bool CsvCell::requires_escape_at(bool a, const Delimiters &delims) const {
 return dat[0]==delims.get(a?OESC:IESC)
  || dat.find(delims.get(a?OFS:IFS))!=dat.npos
  || dat.find(delims.get(a?ORS:IRS))!=dat.npos;
}

/// Determines whether the cell must be escaped at the output or not.
bool CsvCell::requires_escape_for_strategy(const EscapeStrategy &a, const Delimiters &delims) const {
 return requires_escape_at(true, delims)
  || a==ESC_ALL
  || (a==ESC_PRESERVE && quote)
  || (a==ESC_RESOLVE && quote && !requires_escape_at(false, delims));
}

/// @return Escaped version of the cell.
string CsvCell::get_escaped(const Delimiters &a) const {
 string retv;
 retv.reserve(dat.length()+2);	// at least
 retv.push_back(a.get(OESC));
 for (unsigned int i=0;i<dat.length();++i){
  if (dat[i]==a.get(OESC))
   retv.push_back(a.get(OESC));
  retv.push_back(dat[i]);
 }
 retv.push_back(a.get(OESC));
 return retv;
}

/// Parses the input stream and builds CsvCell object.
/// \param a input stream to parse
/// \return last read character (IFS,IRS, EoF)
char CsvCell::parse(istream &a, const Delimiters &delims){
  char c;
  string xdat;
  bool escaped=false;
  PState xs=PSTART;
  try {
   while (xs!=PENDF && !a.get(c).eof()){
    if (c == delims.get(IESC)) {
     if (xs==PSTART) {
      xs=PSTR;escaped=true;
     } else if (xs==PSTR) {
      xs=PSTRX;
     } else {
      if (xs==PSTRX)
       xs=PSTR;
      xdat.push_back(c);
     }
    } else if (c == delims.get(IRS) || c == delims.get(IFS)) {
     if (xs==PSTR) {
      xdat.push_back(c);
     } else {
      xs=PENDF;
     }
    } else {	// not special char
     if (xs==PSTRX) {
      WARN(global_logger,"Unescaped escape character");
      xdat.push_back(delims.get(IESC));
      xs=PSTR;
     } else if (xs==PSTART) {
      xs=PRAW;
     }
     xdat.push_back(c);
    }
   }
  } catch (...){};
  if (a.eof()){
//   cerr<<"end of file.."<<endl;
   c=delims.get(EoF);
  }
  dat=xdat;
  quote=escaped;
//  cerr<<"read field: "<<*this<<endl;
  return c;
}



/// Case sensitive equality check. Does not check the quote flag.
bool CsvCell::operator==(const CsvCell &a) const {
 return (quote==a.quote && dat==a.dat);
}
/// Comparison operator of the CsvCell instances.
/// Compares stored strings. If the stored strings are the same,
/// unquoted instances come first.
bool CsvCell::operator<(const CsvCell &a) const {
 int x=dat.compare(a.dat);
 if (x==0)
  return quote < a.quote;
 return x<0;
};

/// Standard output function of CsvCell instances.
void CsvCell::print(ostream &a, const Delimiters &delims, const EscapeStrategy &strat) const {
 a<<(requires_escape_for_strategy(strat,delims)?get_escaped(delims):get_dat());
}

