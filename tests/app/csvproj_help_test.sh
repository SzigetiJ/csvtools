#!/bin/sh

WDIR="`dirname $0`"

CSVPROJ="../../src/csvproj"

cat "$WDIR/csvproj_help.txt.in" | sed "s:@@FULLPATH@@:$CSVPROJ:" > "./csvproj_help.txt"

cat "$WDIR/t0.csv" | "$CSVPROJ" --help | diff - "./csvproj_help.txt" || exit 1
cat "$WDIR/t0.csv" | "$CSVPROJ" -h | diff - "./csvproj_help.txt" || exit 1
