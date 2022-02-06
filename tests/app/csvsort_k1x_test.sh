#!/bin/sh

WDIR=`dirname "$0"`

CSVSORT="../../src/csvsort"

< "$WDIR/t0.csv" "$CSVSORT" -k 1 | diff - "$WDIR/t0_sort1x.csv" || exit 1
< "$WDIR/t0.csv" "$CSVSORT" -k 1n | diff - "$WDIR/t0_sort1n.csv" || exit 1
< "$WDIR/t0.csv" "$CSVSORT" -k 1d | diff - "$WDIR/t0_sort1d.csv" || exit 1
< "$WDIR/t0.csv" "$CSVSORT" -k 1dn | diff - "$WDIR/t0_sort1dn.csv" || exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e -k 0d | diff - "$WDIR/t1_sort1e0d.csv" || exit 1
