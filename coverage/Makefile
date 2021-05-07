SRCDIR=../src
TESTDIR=../tests
include $(SRCDIR)/Makefile.am

CC=g++
CXXFLAGS=-Wall -Wpedantic -O0 -std=c++11 -fprofile-arcs -ftest-coverage -I$(SRCDIR)
CXXFLAGS_X=-Wall -Wpedantic -O0 -std=c++11 -I$(SRCDIR)
LDFLAGS=-fprofile-arcs -ftest-coverage

TEST_X0 = \
 CsvCell_print \
 CsvCell_parse

INFO_FILE=csvtools.info
INFO_FILE_REDUCED=csvtools.x.info

TEST_X0_PROGS = $(TEST_X0:%=%_test)
TEST_X0_OBJS = $(TEST_X0_PROGS:%=%.o)
TEST_X0_SOURCES = $(TEST_X0_PROGS:%=$(TESTDIR)/%.cc)
TEST_X0_COVS = $(TEST_X0_PROGS:%=%.gcda)
TEST_X0_GCNO = $(TEST_X0_PROGS:%=%.gcno)

all: $(INFO_FILE)

$(INFO_FILE): $(TEST_X0_COVS)
	lcov --rc lcov_branch_coverage=1 -c --directory . --output-file $@

%.gcda: %
	./$<

LIB_OBJS= $(libcsvtools_a_SOURCES:%.cc=l_%.o)

%_test: %_test.o $(LIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%_test.o: $(TESTDIR)/%_test.cc
	$(CC) -c $(CXXFLAGS_X) -o $@ $<

l_%.o: $(SRCDIR)/%.cc
	$(CC) -c $(CXXFLAGS) -o $@ $<

$(INFO_FILE_REDUCED): $(INFO_FILE)
	lcov --rc lcov_branch_coverage=1 -r $< "/usr*" > $@

html: $(INFO_FILE_REDUCED)
	genhtml -s --branch-coverage $< --output-directory html

clean:
	lcov -z --directory .
	rm -f $(INFO_FILE_REDUCED)
	rm -f $(INFO_FILE)
	rm -f $(LIB_OBJS)
	rm -f $(LIB_OBJS:%.o=%.gcda)
	rm -f $(LIB_OBJS:%.o=%.gcno)
	rm -f $(TEST_X0_PROGS)
	rm -f $(TEST_X0_OBJS)
	rm -f $(TEST_X0_COVS)
	rm -f $(TEST_X0_GCNO)
	rm -rf html

.PHONY: clean html

