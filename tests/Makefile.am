AM_DEFAULT_SOURCE_EXT = .cc

TESTS = \
	ColIval_ctor_test \
	ColIvalV_ctor_test \
	ColIvalV_extract_ival_test \
	CsvCell_ctor_test \
	CsvCell_decimal_test \
	CsvCell_less_test \
	CsvCell_parse_test \
	CsvCell_print_test \
	CsvRow_io_test \
	CsvRow_concat_test \
	CsvIStream_test \
	stdrowfuncs_test \
	strsplit_test

check_PROGRAMS = \
	ColIval_ctor_test \
	ColIvalV_ctor_test \
	ColIvalV_extract_ival_test \
	CsvCell_ctor_test \
	CsvCell_decimal_test \
	CsvCell_less_test \
	CsvCell_parse_test \
	CsvCell_print_test \
	CsvRow_io_test \
	CsvRow_concat_test \
	CsvIStream_test \
	stdrowfuncs_test \
	strsplit_test

CLEANFILES = 

BUILT_SOURCES = 

AM_CXXFLAGS = -I${top_srcdir}/src
LDADD = ../src/libcsvtools.a

#strsplit_test_SOURCES=strsplit_test.cc ../src/CommandLine.cc
#stdrowfuncs_test_SOURCES=stdrowfuncs_test.cc ../src/Numeric.cc ../src/RowFunc.cc
#stdrowfuncs_test_OBJS=stdrowfuncs_test.cc src/Numeric.o src/RowFunc.o
strsplit_test_LDADD=../src/CommandLine.o ../src/libcsvtools.a
stdrowfuncs_test_LDADD= ../src/Numeric.o ../src/RowFunc.o

SUBDIRS=app