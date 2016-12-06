noinst_LTLIBRARIES = liblocal.la

liblocal_la_SOURCES = ppk_buf.c ppk_buf.h \
		      access.c \
		      ppkib1.c dtype.h \
		      phonetiser.c \
		      formatdatetime.c

bin_PROGRAMS = ppkbuftool

ppkbuftool_SOURCES = ppkbuftool.c
ppkbuftool_LDADD = liblocal.la

EXTRA_DIST = *
