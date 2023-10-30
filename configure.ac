AC_INIT([utility], [0])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])

OPT_FLAGS="-march=native -O3 -flto"
DEBUG_FLAGS="-ggdb -O0"

CXXFLAGS="-std=gnu++2a $DEBUG_FLAGS $CXXFLAGS"
dnl CXX="g++"

AC_PROG_CXX
AC_LANG(C++)
AX_APPEND_COMPILE_FLAGS([-pthread])
AS_IF([test -z "${WFLAGS+set}"], AX_APPEND_COMPILE_FLAGS([-Wall], WFLAGS))


AC_PROG_RANLIB

CXXFLAGS="$CXXFLAGS $WFLAGS"

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([libsodium], [libsodium])
PKG_CHECK_MODULES([tbb], [tbb]);
PKG_CHECK_MODULES([Catch2], [catch2])
PKG_CHECK_MODULES([lmdb], [lmdb])

PKG_CHECK_MODULES([lmdb], [lmdb])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile utility.pc utility-uninstalled.pc])
AC_OUTPUT
