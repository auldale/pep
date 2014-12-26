dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(mod_pep, 1.0.0)


AC_CONFIG_MACRO_DIR([m4/])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
LT_INIT

PKG_CHECK_MODULES([DEPS], [apr-1 >= 1.4.5])

dnl AM_PROG_CC_C_O

AC_PROG_CC

# this will look for apxs command - put it in $APXS, fail on failure
AX_WITH_APXS()


# find apr-config binary
AC_ARG_WITH(apr_config, AC_HELP_STRING,
[ apr_config="$withval" ],
[AC_PATH_PROGS(apr_config,
[apr-config apr-0-config apr-1-config],
[apr-1-config],
[$PATH:/usr/sbin/:/usr/local/apache2/bin]
)]
)

apr_config="/usr/bin/apr-1-config"



if test "$apr_config" = "no"; then
   AC_MSG_ERROR(Could not find the apr-config program. You can specify a location with the --with-apr-config=FILE option. It may be named apr-0-config or apr-1-config and can be found in your apache2 bin directory.)
fi

$apr_config --cppflags &> /dev/null
if test "$?" != "0"; then
  AC_MSG_ERROR($apr_config is not a valid apr-config program)
fi




APR_LDFLAGS="`${apr_config} --link-ld --libs`"
AC_SUBST(APR_LDFLAGS)

APACHE_CFLAGS="-I`${APXS} -q INCLUDEDIR` -I`${apr_config} --includedir`"
AC_SUBST(APACHE_CFLAGS)

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

