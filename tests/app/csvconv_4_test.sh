#!/bin/sh

WDIR=`dirname "$0"`

CSVCONV="../../src/csvconv"

< "$WDIR/t4.csv" "$CSVCONV" -ofs '|' -esc preserve | diff - "$WDIR/t4_preserve.csv" || exit 1
## default esc is preserve
< "$WDIR/t4.csv" "$CSVCONV" -ofs '|' | diff - "$WDIR/t4_preserve.csv" || exit 1
< "$WDIR/t4.csv" "$CSVCONV" -ofs '|' -esc all | diff - "$WDIR/t4_all.csv" || exit 1
< "$WDIR/t4.csv" "$CSVCONV" -ofs '|' -esc remove | diff - "$WDIR/t4_remove.csv" || exit 1
< "$WDIR/t4.csv" "$CSVCONV" -ofs '|' -esc resolve | diff - "$WDIR/t4_resolve.csv" || exit 1

< "$WDIR/t4.csv" "$CSVCONV" -esc remove | diff - "$WDIR/t4_xrm.csv" || exit 1
< "$WDIR/t4.csv" "$CSVCONV" -ofs '|' | "$CSVCONV" -ifs '|' -esc remove | diff - "$WDIR/t4_xrm.csv" || exit 1

## parameter override
< "$WDIR/t4.csv" "$CSVCONV" -ofs ';' -ofs '|' | diff - "$WDIR/t4_preserve.csv" || exit 1
< "$WDIR/t4.csv" "$CSVCONV" -ofs '|' -esc remove -esc preserve | diff - "$WDIR/t4_preserve.csv" || exit 1
< "$WDIR/t4.csv" "$CSVCONV" -ofs '|' | "$CSVCONV" -ifs ';' -ifs '|' -esc remove | diff - "$WDIR/t4_xrm.csv" || exit 1

echo "## ERROR lines expected">&2
< "$WDIR/t4.csv" "$CSVCONV" -ofs >/dev/null && exit 1
< "$WDIR/t4.csv" "$CSVCONV" -ofs 'x' 'y' >/dev/null && exit 1
< "$WDIR/t4.csv" "$CSVCONV" -esc >/dev/null && exit 1
< "$WDIR/t4.csv" "$CSVCONV" -esc null >/dev/null && exit 1
< "$WDIR/t4.csv" "$CSVCONV" -ifs >/dev/null && exit 1
< "$WDIR/t4.csv" "$CSVCONV" -fs '|' >/dev/null && exit 1
echo "## END (ERROR lines expected)">&2

true
