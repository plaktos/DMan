AC_INIT([dman],[0.0.1])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([configure.ac])
AC_PROG_CXX
AC_CONFIG_FILES([Makefile])

LIBCURL_REQUIRED_VERSION=7.61.0
LIBCURL_CHECK_CONFIG(, $LIBCURL_REQUIRED_VERSION, ,
                     [AC_MSG_ERROR([Missing libcurl >= $LIBCURL_REQUIRED_VERSION])
                     ])

AC_OUTPUT
