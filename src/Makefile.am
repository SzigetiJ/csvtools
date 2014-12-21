bin_PROGRAMS = csvpipe csvfilter csvproj
csvpipe_SOURCES = csvpipe.cc CommandLine.cc DefaultCommandLine.cc
csvfilter_SOURCES = csvfilter.cc CommandLine.cc DefaultCommandLine.cc
csvproj_SOURCES = csvproj.cc CommandLine.cc DefaultCommandLine.cc
csvpipe_LDADD = libcsvtools.la
csvfilter_LDADD = libcsvtools.la
csvproj_LDADD = libcsvtools.la

lib_LTLIBRARIES = libcsvtools.la
libcsvtools_la_SOURCES = CsvCell.cc CsvRow.cc CsvPipe.cc ColTypes.cc RowFunc.cc Delimiters.cc LogConfig.cc
libcsvtools_la_LDFLAGS = -version-info 1:0:0

include_HEADERS = CsvClasses.h CsvCell.h CsvRow.h CsvPipe.h ColTypes.h log.h RowFunc.h default_chars.h Delimiters.h LogConfig.h

