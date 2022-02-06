#!/bin/sh

WDIR="`dirname $0`"

CSVCONV="../../src/csvconv"

cat "$WDIR/t4.csv" | "$CSVCONV" -ofs '|' | diff - "$WDIR/t4_preserve.csv" || exit 1
cat "$WDIR/t4.csv" | "$CSVCONV" -ofs '|' -esc preserve | diff - "$WDIR/t4_preserve.csv" || exit 1
cat "$WDIR/t4.csv" | "$CSVCONV" -ofs '|' -esc all | diff - "$WDIR/t4_all.csv" || exit 1
cat "$WDIR/t4.csv" | "$CSVCONV" -ofs '|' -esc remove | diff - "$WDIR/t4_remove.csv" || exit 1
cat "$WDIR/t4.csv" | "$CSVCONV" -ofs '|' -esc resolve | diff - "$WDIR/t4_resolve.csv" || exit 1

cat "$WDIR/t4.csv" | "$CSVCONV" -esc remove | diff - "$WDIR/t4_xrm.csv" || exit 1
cat "$WDIR/t4.csv" | "$CSVCONV" -ofs '|' | "$CSVCONV" -ifs '|' -esc remove | diff - "$WDIR/t4_xrm.csv" || exit 1
