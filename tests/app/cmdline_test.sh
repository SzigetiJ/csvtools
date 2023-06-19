#!/bin/sh

WDIR=`dirname "$0"`

CMD="../../src/csvproj"

echo -n "" | "$CMD" -v -v 2>&1 | diff - "$WDIR/csvproj_ignore.txt" || exit 1

echo "## ERROR lines expected">&2
"$CMD" - >/dev/null && exit 1
"$CMD" -xxx >/dev/null && exit 1
"$CMD" -- >/dev/null && exit 1
"$CMD" --xxx >/dev/null && exit 1
"$CMD" --columns >/dev/null && exit 1
"$CMD" --help=0 >/dev/null && exit 1
echo "## END (ERROR lines expected)">&2

true
