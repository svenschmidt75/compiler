bin_PROGRAMS = sslex


# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.
sslex_LDFLAGS = $(all_libraries) 
sslex_SOURCES = sslex.h _sslexscanner.cpp _sslexscanner.h sslrparser.cpp \
	sslrparser.h sslrtermdef.h main.cpp
AM_CXXFLAGS = -O0 -g3 -march=athlon64
