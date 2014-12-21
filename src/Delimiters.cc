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
#include "default_chars.h"
#include "Delimiters.h"

using namespace std;

EscapeStrategy &operator<<(EscapeStrategy &a, const string &b){
 a=
  "all"==b?ESC_ALL:
  "preserve"==b?ESC_PRESERVE:
  "resolve"==b?ESC_RESOLVE:
  "remove"==b?ESC_REMOVE:
  ESC_UNDEF;
 return a;
};


Delimiters::Delimiters():
 ifs(DEFAULT_CHR_IFS),
 ofs(DEFAULT_CHR_OFS),
 irs(CHR_RS),
 ors(CHR_RS),
 iesc(CHR_QUOTE),
 oesc(CHR_QUOTE),
 eof(CHR_EOF){
};

void Delimiters::set(Delimiter a, char b){
 switch (a) {
  case IFS: ifs=b; break;
  case OFS: ofs=b; break;
  case IRS: irs=b; break;
  case ORS: ors=b; break;
  case IESC: iesc=b; break;
  case OESC: oesc=b; break;
  case EoF: eof=b; break;
  default:;
 }
}

char Delimiters::get(Delimiter a) const {
 return
  a == IFS ? ifs:
  a == OFS ? ofs:
  a == IRS ? irs:
  a == ORS ? ors:
  a == IESC ? iesc:
  a == OESC ? oesc:
  eof;
}

