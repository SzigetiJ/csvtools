#!/bin/sh

WDIR="`dirname $0`"

CSVPROJ="../../src/csvproj"

cat "$WDIR/t0.csv" | "$CSVPROJ" -c 0 | diff - "$WDIR/t0_proj0.csv"
