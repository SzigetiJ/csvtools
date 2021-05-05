bin_PROGRAMS = csvconv csvfilter csvproj csvsort csvjoin csvaggr
csvconv_SOURCES = csvconv.cc CommandLine.cc DefaultCommandLine.cc
csvfilter_SOURCES = csvfilter.cc CommandLine.cc DefaultCommandLine.cc
csvproj_SOURCES = csvproj.cc CommandLine.cc DefaultCommandLine.cc
csvsort_SOURCES = csvsort.cc CommandLine.cc DefaultCommandLine.cc
csvjoin_SOURCES = csvjoin.cc CommandLine.cc DefaultCommandLine.cc
csvaggr_SOURCES = csvaggr.cc CommandLine.cc DefaultCommandLine.cc Numeric.cc
csvconv_LDADD = libcsvtools.a
csvfilter_LDADD = libcsvtools.a
csvproj_LDADD = libcsvtools.a
csvsort_LDADD = libcsvtools.a
csvjoin_LDADD = libcsvtools.a
csvaggr_LDADD = libcsvtools.a

lib_LIBRARIES = libcsvtools.a
libcsvtools_a_SOURCES = \
 ColTypes.cc \
 CsvCell.cc \
 CsvIStream.cc \
 CsvPipe.cc \
 CsvRow.cc \
 Delimiters.cc \
 globals.cc \
 JoinAlgorithm.cc \
 LogConfig.cc \
 MultiSetJoinAlgorithm.cc \
 RowFunc.cc \
 RowOrderKey.cc

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

