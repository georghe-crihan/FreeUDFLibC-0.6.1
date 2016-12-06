SUBDIRS = doc tests devel local

lib_LTLIBRARIES = libFreeUDFLib.la

#
libFreeUDFLib_la_SOURCES = udf_glob.h \
			   Debug.c Debug.h \
			   Misc.c Misc.h \
			   DateFncs.c DateFncs.h \
			   BlobFncs.c BlobFncs.h \
			   StrFncs.c StrFncs.h \
			   MathFncs.c MathFncs.h \
			   Encrypt.c Encrypt.h \
			   User.c User.h \
			   ExprPars.y ExprPars.h \
			   SymTable.h Lexer.l
AM_LFLAGS = -i
AM_YFLAGS = -d
libFreeUDFLib_la_LIBADD = local/liblocal.la

dist-hook:
	mkdir $(distdir)/common
	cp -p $(srcdir)/common/* $(distdir)/common/

EXTRA_DIST = FreeUDFLibC.def windows.c ext_functions.sql super_ext_functions.sql
AUTOMAKE_OPTIONS = common/ansi2knr
