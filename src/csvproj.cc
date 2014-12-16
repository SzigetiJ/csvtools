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
#include <iostream>
#include "CsvPipe.h"
#include "ColTypes.h"
#include "log.h"

using namespace std;

LogConfig logger;

string HELP="Projects selected columns of the csv file. The program works with <stdin> and <stdout>\n"
"Usage:\n"
"csvproj [OPTIONS]\n"
"Options:\n"
"\t-f<exp>[,<exp>]\tfield list\n"
"\t<exp>:\tnum|-num|num-|num-num\n";

ColIvalV proj_v;


int proc_param(char *a){
 INFO(logger,"Parsing parameter ["<<a<<"]");
 switch (a[0]){
  case '-':
   switch (a[1]){
    case 'f': // projected fields
     {
      ColIvalV tmpproj=parse_projparam(a+2);
      proj_v.insert(proj_v.end(),tmpproj.begin(),tmpproj.end());
     }
     break;
    default:
     cerr<<HELP<<endl;
   }
   break;
  default:
   cerr<<"cannot parse parameter "<<a<<endl;
 }
 INFO(logger,"Parameter parsed.");
 return 0;
};

int main(int argc, char **argv){
 if (argc<2){
  cerr<<HELP<<endl;
  return -1;
 }
 for (int i=1;i<argc;++i){
  proc_param(argv[i]);
 }

 CsvPipe()
 .set_projection(proj_v)
 .process(cin,cout);
 return 0;
}
