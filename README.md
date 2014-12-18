csvtools
========

CsvTools provides transformation tools for CSV files. Supported transformation are the followings:
* projection (column filtering)
* selection (row filtering by row condition)
* sorting (rows ordered by given column(s)) (TODO)
* aggregation (applying given column function to given columns) (TODO)
* extension (appending new column(s) to the CSV as result of given row functions) (TODO)
* join of two CSV files (TODO)
  * natural join
  * equijoin
  * outer left join

Furthermore, CsvTools can be used as C++ library (libcsvtools).

Examples
--------

The input is given as a _well-formed_ CSV file
(cells separated by comma (','), records separated by newline ('\n') and the CSV file is also terminated by newline,
first line is the CSV header, cells that need to be escaped are escaped by doublequote ('"'),
each line contains as many cells (fields) as the header):

`persons.csv`
```
"ID","YoB","Name","Address"
0,1685,"Johann Sebastian Back","Leipzig"
1,1756,"Wolfgang Amadeus Mozart","Salzburg"
2,1770,"Ludwig van Beethoven",Vienna
3,1810,"Frédéric Chopin",Paris
4,1984,"John ""Johnny"" Smith","London,
Soho"
5,1984,"Peter Carpenter","London"
```

#### Projection

For projection use `csvproj`. `csvproj` takes option `-c <expr>`, where expr defined columns or column interval.
* `-c 0,1,2` denotes first (0), second (1) and third (2) columns, respectively. Note that column numbering begins with 0.
This column sequence definition is equivalent with `-c 0 -c 1 -c 2`.
* `-c 2,0` denotes third (2) and first (0) columns, respectively.
* `-c 1,1` denotes the second (1) column twice.
* `-c 0-2` denotes column interval [0,2], i.e., columns {0,1,2}.
* `-c -3` denotes columns up to interval up to column 3, i.e., columns {0,1,2,3}.
* `-c 3-` denotes colums 3 and followers, i.e., columns {3,4,...,last}.
* `-c 1,0-2` denotes columns {1,0,1,2}.
* Without any column interval option `csvproj` assumes that colums {} have to be projected, hence it produces an empty line for each input record.

`cat persons.csv | csvproj -c 2` produces
```
"Name"
"Johann Sebastian Back"
"Wolfgang Amadeus Mozart"
"Ludwig van Beethoven"
"Frédéric Chopin"
"John ""Johnny"" Smith"
"Peter Carpenter"
```

`cat persons.csv | csvproj -c 3-,-0` produces
```
"Address","ID"
"Leipzig",0
"Salzburg",1
Vienna,2
Paris,3
"London,
Soho",4
"London",5
```
