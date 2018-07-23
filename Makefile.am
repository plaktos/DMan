bin_PROGRAMS=dman test
dman_SOURCES=src/main.cpp src/downman.cpp
dman_LDADD = @LIBCURL@
dman_CPPFLAGS = @LIBCURL_CPPFLAGS@

test_SOURCES=src_test/catch_main.cpp src_test/t_downman.cpp src_test/t_dsession.cpp src_test/t_cliparser.cpp src/downman.cpp
test_LDADD = @LIBCURL@
test_CPPFLAGS = @LIBCURL_CPPFLAGS@
