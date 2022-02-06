#!/bin/sh

WDIR=`dirname "$0"`

CSVJOIN="../../src/csvjoin"

< "$WDIR/t1.csv" "$CSVJOIN" -jt natural -jf "$WDIR/t2.csv" | diff - "$WDIR/t1t2_natural.csv" || exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt inner -jc 1:0 -jf "$WDIR/t2.csv" | diff - "$WDIR/t1t2_natural.csv" || exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt outer -jc 1:0 -jf "$WDIR/t2.csv" | diff - "$WDIR/t1t2_outer.csv" || exit 1
< "$WDIR/t2.csv" "$CSVJOIN" -jt cross -jf "$WDIR/t3.csv" | diff - "$WDIR/t2t3_cross.csv" || exit 1
