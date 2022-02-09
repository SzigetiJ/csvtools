#!/bin/sh

WDIR=`dirname "$0"`

CSVPROJ="../../src/csvproj"

< "$WDIR/t0.csv" "$CSVPROJ" -c 1 | diff - "$WDIR/t0_proj1.csv"

echo "## ERROR lines expected">&2
< "$WDIR/t0.csv" "$CSVPROJ" -c >/dev/null && exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -c 1 0 >/dev/null && exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -c 1, >/dev/null && exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -c +1 >/dev/null && exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -c 1+ >/dev/null && exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -c -- >/dev/null && exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -c --1 >/dev/null && exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -c 1-- >/dev/null && exit 1
< "$WDIR/t0.csv" "$CSVPROJ" -c 1--1 >/dev/null && exit 1
## TODO
#echo "a,\"b" | "$CSVPROJ" -c 0 >/dev/null && exit 1
echo "## END (ERROR lines expected)">&2

true
