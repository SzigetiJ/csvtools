#!/bin/sh

WDIR="`dirname $0`"

CSVSORT="../../src/csvsort"
CSVAGGR="../../src/csvaggr"

cat "$WDIR/t1.csv" | "$CSVSORT" -k 1e |  "$CSVAGGR" -a total sum 0 | diff - "$WDIR/t1_aggr0s.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVSORT" -k 1e |  "$CSVAGGR" -a maximum max 0 | diff - "$WDIR/t1_aggr0mx.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVSORT" -k 1e |  "$CSVAGGR" -a minimum min 0 | diff - "$WDIR/t1_aggr0mn.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVSORT" -k 1e |  "$CSVAGGR" -a count count 0 | diff - "$WDIR/t1_aggr0cnt.csv" || exit 1
cat "$WDIR/t1.csv" | "$CSVSORT" -k 1e |  "$CSVAGGR" -a string concat 0 | diff - "$WDIR/t1_aggr0cc.csv" || exit 1

cat "$WDIR/t1.csv" | "$CSVSORT" -k 1de |  "$CSVAGGR" -a minimum min 0 -a maximum max 0 | diff - "$WDIR/t1_aggr0mmrev.csv" || exit 1

cat "$WDIR/t5.csv" | "$CSVAGGR" -a cnt count 1 -a sum sum 1 -a min min 1 -a max max 1 | diff - "$WDIR/t5_aggr1smm.csv" || exit 1
