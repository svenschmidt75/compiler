bin_PROGRAMS = ssass
ssass_SOURCES = ssass.cpp ssass.h sslexscanner.cpp sslexscanner.h \
			sslrparser.cpp sslrparser.h sslrtermdef.h copcode.cpp cnooperandopcode.cpp \
			coneoperandopcode.cpp ctwooperandopcode.cpp cadressingmode.cpp clabel.cpp cdcdata.cpp \
	cdcdatabyte.cpp cdcdataword.cpp cdcdatalong.cpp cdcdatasingle.cpp cdcdatadouble.cpp

# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.
ssass_LDFLAGS = $(all_libraries) 
noinst_HEADERS = copcode.h cnooperandopcode.h coneoperandopcode.h \
					ctwooperandopcode.h cadressingmode.h clabel.h cdcdata.h cdcdataword.h cdcdatalong.h \
	cdcdatasingle.h cdcdatadouble.h
AM_CXXFLAGS = -O0 -g3 -march=athlon-xp -short-enums
METASOURCES = AUTO
