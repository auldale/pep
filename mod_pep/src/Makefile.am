
noinst_LTLIBRARIES = libmodpep.la
noinst_DATA = mod_pep.la

AM_CPPFLAGS =  $(DEPS_CFLAGS) ${APACHE_CFLAGS} -I/usr/local/include/pep
AM_LDFLAGS = ${DEPS_LIBS} ${APR_LDFLAGS} -lpep-lite
LDADD = ${DEPS_LIBS}

libmodpep_la_SOURCES = \
	mod_pep.c


AM_CFLAGS = -Wall



install-exec-local:
	$(APXS) -i -n 'pep' mod_pep.la

mod_pep.la: libmodpep.la
	$(APXS) -c -o $@ $< ${APACHE_CFLAGS} 
