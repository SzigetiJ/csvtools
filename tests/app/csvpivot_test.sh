#!/bin/sh

WDIR=`dirname "$0"`

CSVPIVOT="../../src/csvpivot"
CSVCONV="../../src/csvconv"

< "$WDIR/t6.csv" "$CSVPIVOT" -p 0 1 2 sum | "$CSVCONV" -esc remove | diff - "$WDIR/t6pivot_sum.csv" || exit 1
< "$WDIR/t6.csv" "$CSVPIVOT" -p 0 1 2 concat | "$CSVCONV" -esc remove | diff - "$WDIR/t6pivot_concat.csv" || exit 1
< "$WDIR/t6.csv" "$CSVPIVOT" -p 0 1 2 count | "$CSVCONV" -esc remove | diff - "$WDIR/t6pivot_count.csv" || exit 1
< "$WDIR/t6.csv" "$CSVPIVOT" -p 0 1 2 first | "$CSVCONV" -esc remove | diff - "$WDIR/t6pivot_first.csv" || exit 1
< "$WDIR/t6.csv" "$CSVPIVOT" -p 0 1 2 last | "$CSVCONV" -esc remove | diff - "$WDIR/t6pivot_last.csv" || exit 1
< "$WDIR/t6.csv" "$CSVPIVOT" -p 0 1 2 max | "$CSVCONV" -esc remove | diff - "$WDIR/t6pivot_max.csv" || exit 1
< "$WDIR/t6.csv" "$CSVPIVOT" -p 0 1 2 min | "$CSVCONV" -esc remove | diff - "$WDIR/t6pivot_min.csv" || exit 1

echo "## ERROR lines expected">&2
< "$WDIR/t6.csv" "$CSVPIVOT" >/dev/null && exit 1
< "$WDIR/t6.csv" "$CSVPIVOT" -p >/dev/null && exit 1
< "$WDIR/t6.csv" "$CSVPIVOT" -p 0 1 2 >/dev/null && exit 1
echo "## END (ERROR lines expected)">&2

true
