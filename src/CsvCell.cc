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
#include "Delimiters.h"
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

/// @return Escaped version of the cell.
string CsvCell::get_escaped() const {
 string retv;
 for (unsigned int i=0;i<dat.length();++i){
  if (dat[i]==Delimiters::get(OESC))
   retv.push_back(Delimiters::get(OESC));
  retv.push_back(dat[i]);
 }
 return retv;
}

/// Parses the input stream and builds the CsvData object.
/// \param a input stream to parse
/// \return last read character (CHR_IFS,CHR_RS, eof)
char CsvCell::parse(istream &a){
  char c;
  string xdat;
  bool xq=false;
  PState xs=PSTART;
  try {
   while (xs!=PENDF && !a.get(c).eof()){
//   cerr<<"read: "<<c<<endl;
	if (c == Delimiters::get(IRS) || c == Delimiters::get(IFS)) {
     if (xs == PSTR) {	// within an str value
      xdat.push_back(c);
     } else {	// outside an str value
      xs = PENDF;
     }
    } else if (c == Delimiters::get(IESC)) {
     if (xs==PSTR) {xs=PSTRX;}
     else if (xs==PSTRX) {xdat.push_back(Delimiters::get(IESC));xs=PSTR;}
     else if (xs==PSTART) {xs=PSTR;xq=true;}
     else {cerr<<"unexpected char.."<<endl;}
    } else {
     xdat.push_back(c);
    }
   }
  } catch (...){};
  if (a.eof()){
//   cerr<<"end of file.."<<endl;
   c=Delimiters::get(EoF);
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
ostream &operator<<(ostream &a, const CsvCell &b){
 return (b.quote?a<<Delimiters::get(OESC)<<b.get_escaped()<<Delimiters::get(OESC):a<<b.get_escaped());
}

