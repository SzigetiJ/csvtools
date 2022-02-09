#!/bin/sh

WDIR=`dirname "$0"`

CSVJOIN="../../src/csvjoin"

< "$WDIR/t1.csv" "$CSVJOIN" -jt natural -jf "$WDIR/t2.csv" | diff - "$WDIR/t1t2_natural.csv" || exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt inner -jc 1:0 -jf "$WDIR/t2.csv" | diff - "$WDIR/t1t2_natural.csv" || exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt outer -jc 1:0 -jf "$WDIR/t2.csv" | diff - "$WDIR/t1t2_outer.csv" || exit 1
< "$WDIR/t2.csv" "$CSVJOIN" -jt cross -jf "$WDIR/t3.csv" | diff - "$WDIR/t2t3_cross.csv" || exit 1

echo "## ERROR lines expected">&2
< "$WDIR/t1.csv" "$CSVJOIN" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt natural >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt left -jc 1:0 -jf "$WDIR/t2.csv" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt inner -jc 1 -jf "$WDIR/t2.csv" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt inner -jc 1:+0 -jf "$WDIR/t2.csv" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt inner -jc 1--:--0 -jf "$WDIR/t2.csv" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt inner -jf "$WDIR/t2.csv" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt outer -jf "$WDIR/t2.csv" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVJOIN" -jt natural -jf "$WDIR/t2_noexist.csv" >/dev/null && exit 1
## TODO
#echo "a,\"b" | "$CSVJOIN" -jt natural -jf "$WDIR/t2.csv" >/dev/null && exit 1
echo "## END (ERROR lines expected)">&2

true
