AC_INIT([dman],[0.1.0])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([configure.ac])

AC_PROG_CXX

LIBCURL_REQUIRED_VERSION=7.61.0
LIBCURL_CHECK_CONFIG(, $LIBCURL_REQUIRED_VERSION, ,
                     [AC_MSG_ERROR([Missing libcurl >= $LIBCURL_REQUIRED_VERSION])
                     ])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
