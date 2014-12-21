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
#include "CsvCell.h"
#include <cstdlib>
#include <algorithm>

using namespace std;

/// States of csv parser
typedef enum {
 PSTART,	///< parse begins
 PSTR,	///< we are parsing a string (waiting for string delimiter)
 PSTRX,	///< within a string we have found a string delimiter. This means that either the string is over or a delimiter character is encoded in the string.
 PENDF,	///< we have parsed end-of-field
} PState;

/// Standard constructor.
CsvCell::CsvCell(const string &a, bool b):dat(a),quote(b){
};
CsvCell::CsvCell(){};

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
  bool xq=false;
  PState xs=PSTART;
  try {
   while (xs!=PENDF && !a.get(c).eof()){
//   cerr<<"read: "<<c<<endl;
	if (c == delims.get(IRS) || c == delims.get(IFS)) {
     if (xs == PSTR) {	// within an str value
      xdat.push_back(c);
     } else {	// outside an str value
      xs = PENDF;
     }
    } else if (c == delims.get(IESC)) {
     if (xs==PSTR) {xs=PSTRX;}
     else if (xs==PSTRX) {xdat.push_back(delims.get(IESC));xs=PSTR;}
     else if (xs==PSTART) {xs=PSTR;xq=true;}
     else {cerr<<"unexpected char.."<<endl;}
    } else {
     xdat.push_back(c);
    }
   }
  } catch (...){};
  if (a.eof()){
//   cerr<<"end of file.."<<endl;
   c=delims.get(EoF);
  }
  dat=xdat;
  quote=xq;
//  cerr<<"read field: "<<*this<<endl;
  return c;
}



/// Case sensitive equality check. Does not check the quote flag.
bool CsvCell::operator==(const CsvCell &a) const {
 return !(*this<a) && !(a<*this); 
}
/// Comparison operator of the CsvCell instances.
/// If the data is text, we compare them as strings,
/// if they are numeric, we compare them as floats.
/// Hopefully the compared CsvCells are both either text or numeric.
bool CsvCell::operator<(const CsvCell &a) const {
 if (quote) {
  return dat<a.dat;
 }
 return atof(dat.c_str())<atof(a.dat.c_str());
};

/// Standard output function of CsvCell instances.
void CsvCell::print(ostream &a, const Delimiters &delims, const EscapeStrategy &strat) const {
 a<<(requires_escape_for_strategy(strat,delims)?get_escaped(delims):get_dat());
}

