#!/bin/sh

WDIR=`dirname "$0"`

CSVFILTER="../../src/csvfilter"

< "$WDIR/t1.csv" "$CSVFILTER" -r =:1:a | diff - "$WDIR/t1_filt1a.csv" || exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r !=:1:b -r !=:1:c -r !=:1:d -r !=:1: | diff - "$WDIR/t1_filt1a.csv" || exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r =:0:2 | diff - "$WDIR/t1_filt02.csv" || exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r ">:0:1" | diff - "$WDIR/t1_filt02.csv" || exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r ">=:0:2" | diff - "$WDIR/t1_filt02.csv" || exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r "!=:1,0:a,1" | diff - "$WDIR/t1_filtne.csv" || exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r "<:0:2" | diff - "$WDIR/t1_filt01.csv" || exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r "<=:0:1" | diff - "$WDIR/t1_filt01.csv" || exit 1

echo "## ERROR lines expected">&2
< "$WDIR/t1.csv" "$CSVFILTER" -r "==:0:1" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r "=" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r "=:0" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -q "=:0:1" >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVFILTER" -r "=:0,:1," >/dev/null && exit 1
## TODO
#echo "a,\"b" | "$CSVFILTER" -r =:0:a >/dev/null && exit 1
echo "## END (ERROR lines expected)">&2

true
