#!/bin/sh

WDIR="`dirname $0`"

CSVFILTER="../../src/csvfilter"

cat "$WDIR/t1.csv" | "$CSVFILTER" -r =:1:a | diff - "$WDIR/t1_filt1a.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVFILTER" -r !=:1:b -r !=:1:c -r !=:1:d -r !=:1: | diff - "$WDIR/t1_filt1a.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVFILTER" -r =:0:2 | diff - "$WDIR/t1_filt02.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVFILTER" -r ">:0:1" | diff - "$WDIR/t1_filt02.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVFILTER" -r ">=:0:2" | diff - "$WDIR/t1_filt02.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVFILTER" -r "!=:1,0:a,1" | diff - "$WDIR/t1_filtne.csv" || exit 1
