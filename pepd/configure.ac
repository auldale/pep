dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(pepd, 1.0)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

PKG_CHECK_MODULES([DEPS], [pep >= 0.0.0 xerces-c >= 0.0.0])

AC_PROG_CXX

AC_LANG([C++])
AC_LANG_CPLUSPLUS
AC_CHECK_HEADER(cstdint)
CXXFLAGS='-Wall -std=gnu++0x'

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

