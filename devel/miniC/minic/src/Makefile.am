bin_PROGRAMS = minic
minic_SOURCES = minic.cpp sslexscanner.cpp sslexscanner.h sslrparser.cpp \
	sslrparser.h sslrtermdef.h csymboltable.cpp csymboltable.h csyntaxtree.cpp \
	ccodebase.cpp

# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.
minic_LDFLAGS = $(all_libraries) 

noinst_HEADERS = csyntaxtree.h ccodebase.h
AM_CXXFLAGS = -O0 -g3 -march=athlon64
