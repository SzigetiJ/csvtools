[![GitHub](https://img.shields.io/github/license/SzigetiJ/csvtools)](LICENSE)
![C/C++ CI](https://github.com/SzigetiJ/csvtools/workflows/C/C++%20CI/badge.svg)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/bfe062e95e6d427ca609e5fb0d04a91e)](https://www.codacy.com/gh/SzigetiJ/csvtools/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=SzigetiJ/csvtools&amp;utm_campaign=Badge_Grade)
[![codecov](https://codecov.io/gh/SzigetiJ/csvtools/branch/master/graph/badge.svg?token=7XZOJJ2DWL)](https://codecov.io/gh/SzigetiJ/csvtools)

csvtools
========

CsvTools provides transformation tools for CSV files. Supported transformation
are the followings:

*   projection (column filtering)
*   selection (row filtering by row condition)
*   sorting (rows ordered by given column(s))
*   aggregation (applying given column function to given columns)
*   extension (appending new column(s) to the CSV as result of given row functions) (TODO)
*   join of two CSV files
    *    natural join
    *    equijoin
    *    outer left join
    *    cross (full) join

Furthermore, CsvTools can be used as C++ library (libcsvtools).

Examples
--------

The input is given as a _well-formed_ CSV file
(cells separated by comma (`,`), records separated by newline (`\n`) and the CSV
file is also terminated by newline, first line is the CSV header,
cells that need to be escaped are escaped by doublequote (`"`),
each line contains as many cells (fields) as the header):

`persons.csv`
```csv
"ID","YoB","Name","Address"
0,1685,"Johann Sebastian Bach","Leipzig"
1,1756,"Wolfgang Amadeus Mozart","Salzburg"
2,1770,"Ludwig van Beethoven",Vienna
3,1810,"Frédéric Chopin",Paris
4,1984,"John ""Johnny"" Smith","London,
Soho"
5,1984,"Peter Carpenter","London"
```

### Projection

For projection use `csvproj`. `csvproj` takes option `-c <expr>`,
where expr defines columns or column interval.

*   `-c 0,1,2` denotes first (0), second (1) and third (2) columns, respectively. Note that column numbering begins with 0. This column sequence definition is equivalent with `-c 0 -c 1 -c 2`.
*   `-c 2,0` denotes third (2) and first (0) columns, respectively.
*   `-c 1,1` denotes the second (1) column twice.
*   `-c 0-2` denotes column interval \[0,2\], i.e., columns {0,1,2}.
*   `-c -3` denotes columns up to interval up to column 3, i.e., columns {0,1,2,3}.
*   `-c 3-` denotes colums 3 and followers, i.e., columns {3,4,...,last}.
*   `-c 1,0-2` denotes columns {1,0,1,2}.
*   Without any column interval option `csvproj` assumes that colums {} have to be projected, hence it produces an empty line for each input record.

`cat persons.csv | csvproj -c 2` produces
```csv
"Name"
"Johann Sebastian Bach"
"Wolfgang Amadeus Mozart"
"Ludwig van Beethoven"
"Frédéric Chopin"
"John ""Johnny"" Smith"
"Peter Carpenter"
```

`cat persons.csv | csvproj -c 3-,-0` produces
```csv
"Address","ID"
"Leipzig",0
"Salzburg",1
Vienna,2
Paris,3
"London,
Soho",4
"London",5
```

### Selection

Use `csvfilter` for selecting rows by condition. The special option interpreted
by `csvfilter` is `-r <cond>`, where cond defines the fitering condition.
The condition is given as a relation between a column (or sequence of columns)
and a string constant.
The syntax of the condition is `cond ::= <relation>:<expr>:str` where relation
is one of {=, !=, <, <=, >, >=}, expr is a column expression (see Projection)
and str is a string constant. If multiple `-r <cond>` conditions are given,
row must fulfill each of them to get selected.

*   `-r =:0:1` select rows where the first cell equals `1`.
*   `-r =:1,3:foo,bar` selects rows where columns 1 and 3 (separated by default output field separator, `,`) give `foo,bar`. Equivalent with
    *    `-r =:3,1:bar,foo`
    *    `-r =:1:foo -r =:3:bar`
*   `-r >=:0,50 -r <:0:100` selects rows where the first column is at least 50 but less than 100.

`cat persons.csv | csvfilter -r "<:1:1800"` produces
```csv
"ID","YoB","Name","Address"
0,1685,"Johann Sebastian Bach","Leipzig"
1,1756,"Wolfgang Amadeus Mozart","Salzburg"
2,1770,"Ludwig van Beethoven",Vienna
```

### Sorting

With `csvsort` the rows of CSV file can be sorted. Features:

*   multi-column sort,
*   ascending / descending sort,
*   numeric / lexicographic sort,
*   special handling of empty cells.

The order of the rows can by contolled with the `-k {num}[d][n][e]` option
where `num` is a column number, `d` is flag for _descending_ order, `n` flag is
for _numeric_ sort and `e` flag denotes that empty cells are greater than not empty cells.
If multiple order columns are defined, the second, third, etc. definitions are
taken into account only if the rows cannot be sorted with the first definition.

Examples:

*   `-k 0` sorts the rows ordered by the first column, lexicographically.
*   `-k 0 -k 1n` sorts the rows ordered by the first column lexicographically, and if rows have the same value in the first column, their order is defined by the second column with numeric order.
*   `-k 0d` rows ordered by first column, descending order.
*   `-k 0e` rows ordered by first column, but rows with empty first cell go to the end.

### Aggregation

With `csvaggr` a column (or a set of columns) can be aggregated.
`csvaggr` supports the following column functions:

*   sum
*   min
*   max
*   count
*   concat

Use `csvaggr` with option `-a <name> <fun> <expr>` where `<name>` will be the
name of the result column, `<fun>` is the applied column function
and `<expr>` is the column expression (see [csvproj](#markdown-header-projection)).
There can be given multiple `-a` options. If a column is not part of any column expression,
it will be used as grouping column.
Thus, every column either will be aggregated or will be a grouping column.

For `csvaggr` to work properly, it is requsite that the input csv data is
sorted by the grouping columns. 

Unnecessary columns cannot be dropped with `csvaggr`.
Use `csvproj` to get rid of unused columns.

Summarized, for aggregating an arbitrary csv file, invoke the
`csvproj | csvsort | csvaggr` process pipeline.

Examples:

*   `csvaggr -a cnt count -` counts the records.
*   `csvaggr -a cnt count 1-` counts the records grouped by the first column.
*   `csvaggr -a cnt count 1,3-` counts the the records grouped by the first and the third columns.
*   `csvproj -c -1 | csvaggr -a sum sum 1` calculates the sum of values in the second column grouped by the first column.
*   `csvproj -c 4 | csvaggr -a min min 0` finds the minimum value of column #4.

### Join CSV files

The command `csvjoin` supports joining of two csv files (left and right table).
The left table is read from stdin, whereas the right table is defined by command
line parameter `-jf <filename>`.
The join type is also a mandatory parameter: `-jt <join_type>`.
The following `join_type` values are available:

*   `natural` recognizes common colums, and performs inner join based on the common columns.
*   `cross` performs full join. Results in Descartes-product of the two input tables.
*   `inner` for inner join.
*   `outer` for outer left join.

In case of inner and outer join, the join columns must be provided: `-jc <expr>`
where `expr` is a column expression (see projection).

### Dealing with Other-than-Comma Separated Values

CSV files are not always comma-separated. Some "CSV" files contain TAB- or
semicolon-separated values. With `csvpipe` these files can be converted into
comma-separated files and CSV files can be converted into Any-character Separated Values file.
Available options: `[-ifs <char>] [-ofs <char>] [-esc {all|preserve|resolve|remove}]`,
where ifs and ofs define the field separator at the input and at the output,
respectively, whereas esc defines the field escaping strategy:

*   `all`: every field will be escaped.
*   `preserve`: already escaped fields remain escaped and escaping is introduced where necessary.
*   `resolve`: same as `preserve`, except for the fields where field escaping was necessary at the input, but is not required at the output -- there fields will not be escaped.
*   `remove`: remove escaping from fields if possible.
