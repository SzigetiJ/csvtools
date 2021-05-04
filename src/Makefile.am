bin_PROGRAMS = csvconv csvfilter csvproj csvsort csvjoin csvaggr
csvconv_SOURCES = csvconv.cc CommandLine.cc DefaultCommandLine.cc
csvfilter_SOURCES = csvfilter.cc CommandLine.cc DefaultCommandLine.cc
csvproj_SOURCES = csvproj.cc CommandLine.cc DefaultCommandLine.cc
csvsort_SOURCES = csvsort.cc CommandLine.cc DefaultCommandLine.cc
csvjoin_SOURCES = csvjoin.cc CommandLine.cc DefaultCommandLine.cc
csvaggr_SOURCES = csvaggr.cc CommandLine.cc DefaultCommandLine.cc Numeric.cc
csvconv_LDADD = libcsvtools.la
csvfilter_LDADD = libcsvtools.la
csvproj_LDADD = libcsvtools.la
csvsort_LDADD = libcsvtools.la
csvjoin_LDADD = libcsvtools.la
csvaggr_LDADD = libcsvtools.la

lib_LTLIBRARIES = libcsvtools.la
libcsvtools_la_SOURCES = CsvCell.cc CsvRow.cc CsvPipe.cc ColTypes.cc RowFunc.cc Delimiters.cc LogConfig.cc RowOrderKey.cc globals.cc CsvIStream.cc JoinAlgorithm.cc MultiSetJoinAlgorithm.cc
libcsvtools_la_LDFLAGS = -version-info 1:0:0

include_HEADERS = \
 ColTypes.h \
 CommandLine.h \
 CsvCell.h \
 CsvClasses.h \
 CsvIStream.h \
 CsvPipe.h \
 CsvRow.h \
 CsvStream.h \
 default_chars.h \
 DefaultCommandLine.h \
 Delimiters.h \
 globals.h \
 JoinAlgorithm.h \
 LogConfig.h \
 log.h \
 MultiSetJoinAlgorithm.h \
 Numeric.h \
 RowFunc.h \
 RowOrderKey.h \
 stl_out.h

