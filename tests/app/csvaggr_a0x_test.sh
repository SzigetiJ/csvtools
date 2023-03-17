#!/bin/sh

WDIR=`dirname "$0"`

CSVSORT="../../src/csvsort"
CSVAGGR="../../src/csvaggr"

< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a total sum 0 | diff - "$WDIR/t1_aggr0s.csv" || exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" --aggregation=total sum 0 | diff - "$WDIR/t1_aggr0s.csv" || exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a maximum max 0 | diff - "$WDIR/t1_aggr0mx.csv" || exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a minimum min 0 | diff - "$WDIR/t1_aggr0mn.csv" || exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a count count 0 | diff - "$WDIR/t1_aggr0cnt.csv" || exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a string concat 0 | diff - "$WDIR/t1_aggr0cc.csv" || exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 0e |  "$CSVAGGR" -a first first 1 | diff - "$WDIR/t1_aggr0first.csv" || exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 0e |  "$CSVAGGR" -a last last 1 | diff - "$WDIR/t1_aggr0last.csv" || exit 1

< "$WDIR/t1.csv" "$CSVSORT" -k 1de |  "$CSVAGGR" -a minimum min 0 -a maximum max 0 | diff - "$WDIR/t1_aggr0mmrev.csv" || exit 1

< "$WDIR/t5.csv" "$CSVAGGR" -a cnt count 1 -a sum sum 1 -a min min 1 -a max max 1 | diff - "$WDIR/t5_aggr1smm.csv" || exit 1

echo "## ERROR lines expected">&2
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a string concat 0-1x >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a total sum >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a total sum 0 1 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -aa total sum 0 >/dev/null && exit 1
< "$WDIR/t1.csv" "$CSVSORT" -k 1e |  "$CSVAGGR" -a average avg 0 >/dev/null && exit 1
echo "a,\"b" | "$CSVAGGR" -a cnt count 0 >/dev/null && exit 1
echo "## END (ERROR lines expected)">&2

true
