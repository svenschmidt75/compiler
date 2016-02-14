bin_PROGRAMS = sslr


# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.
sslr_LDFLAGS = $(all_libraries)
sslr_SOURCES = hashclass.cpp hashclass.h main.cpp sslexscanner.cpp\
	sslexscanner.h sslr.h _sslrparser.cpp _sslrparser.h sslrtermdef.h\
	terminal.cpp terminal.h
AM_CXXFLAGS = -O0 -Wall -g3 -march=athlon64
