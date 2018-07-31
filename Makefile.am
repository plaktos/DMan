bin_PROGRAMS=dman
EXTRA_PROGRAMS=test

dman_SOURCES=src/main.cpp\
			 src/downman.cpp

dman_LDFLAGS = -pthread
dman_LDADD = @LIBCURL@
dman_CPPFLAGS = @LIBCURL_CPPFLAGS@

test_SOURCES=src_test/catch_main.cpp\
			 src_test/t_downman.cpp\
			 src_test/t_cliparser.cpp\
			 src/downman.cpp

test_LDFLAGS = -pthread
test_LDADD = @LIBCURL@
test_CPPFLAGS = @LIBCURL_CPPFLAGS@
