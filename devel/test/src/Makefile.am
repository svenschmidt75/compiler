bin_PROGRAMS = test
test_SOURCES = test.cpp ctest.cpp

# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.
test_LDFLAGS = $(all_libraries) 

AM_CXXFLAGS = -O0 -Wall -W -save-temps
noinst_HEADERS = ctest.h
