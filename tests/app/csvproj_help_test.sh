#!/bin/sh

WDIR=`dirname "$0"`

CSVPROJ="../../src/csvproj"

< "$WDIR/csvproj_help.txt.in" sed "s:@@FULLPATH@@:$CSVPROJ:" > "./csvproj_help.txt"
< "$WDIR/csvproj_version.txt.in" sed "s:@@VERSION@@:$PACKAGE_STRING:" > "./csvproj_version.txt"

< "$WDIR/t0.csv" "$CSVPROJ" --help | diff - "./csvproj_help.txt" || exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -h | diff - "./csvproj_help.txt" || exit 1
"$CSVPROJ" -V | diff - "./csvproj_version.txt" || exit 1

rm "./csvproj_help.txt"
rm "./csvproj_version.txt"
