dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libpep, 1.0)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
LT_INIT

PKG_CHECK_MODULES([DEPS], [log4shib >= 1.0])



AC_PROG_CXX

AC_LANG([C++])
AC_LANG_CPLUSPLUS
AC_CHECK_HEADER(cstdint)
CXXFLAGS='-Wall -std=gnu++0x'

AC_CONFIG_FILES(Makefile src/pep/Makefile src/pep/pep.pc)
AC_OUTPUT

