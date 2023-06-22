#!/bin/sh

WDIR=`dirname "$0"`

CSVEXT="../../src/csvext"
CSVCONV="../../src/csvconv"

< "$WDIR/t0.csv" "$CSVEXT" -a id:x -pv 0:0:1 -a concat:xfoo -pc 1:0:2 -pc 1:1:0 | "$CSVCONV" -esc remove | diff - "$WDIR/t0_id1.csv" || exit 1
< "$WDIR/t0.csv" "$CSVEXT" -a add:sum -pc 0:0:0 -pc 0:1:1 | "$CSVCONV" -esc remove | diff - "$WDIR/t0_extadd.csv" || exit 1
< "$WDIR/t0.csv" "$CSVEXT" -a add:inc2 -pc 0:0:0 -pv 0:1:2 -a add:dec1 -pc 1:0:1 -pv 1:1:-1 | "$CSVCONV" -esc remove | diff - "$WDIR/t0_extadd2.csv" || exit 1
< "$WDIR/t0.csv" "$CSVEXT" -a sub:ba -a sub:ab -pc 0:0:1 -pc 0:1:0 -pc 1:0:0 -pc 1:1:1 | "$CSVCONV" -esc remove | diff - "$WDIR/t0_extsub.csv" || exit 1

echo "## ERROR lines expected">&2
< "$WDIR/t1.csv" "$CSVEXT" -a >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a add >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a sum:add >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -pc >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -pv >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -pv 0:0:0 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -pc 0:0:0 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a add:sum -pc 0 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a add:sum -pc 0:0 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a add:sum -pv 0 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a add:sum -pv 0:0 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a add:sum -pc 0:0:0 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a add:sum -pc 0:2:0 -pc 0:1:1 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVEXT" -a add:sum -pc 1:0:0 -pc 0:1:1 >/dev/null && exit 1
echo "## END (ERROR lines expected)">&2

true
