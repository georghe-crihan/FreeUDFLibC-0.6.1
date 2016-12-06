# ---------------------------------------------------------------------
# FreeUDFLibC - copyright (c) 1999 Gregory Deatz
# ---------------------------------------------------------------------

# ---------------------------------------------------------------------
# InterBase Installation Directory
#
# CHANGE this definition to point to your InterBase installation directory
# ---------------------------------------------------------------------
IBASE=/usr/interbase

# MD5 support. See below for objects.
MD_LIB=-lmd # Comment out if system-wide MD5 is absesent
#MD_OBJECT=MD5.o # Uncomment if system-wide MD5 is present

# ---------------------------------------------------------------------
# General InterBase Defines for SOLARIS
# ---------------------------------------------------------------------
GPRE=$(IBASE)/bin/gpre -c -n
GPRE_M=$(IBASE)/bin/gpre -c -n -m
ISQL=$(IBASE)/bin/isql
LEX=flex
YACC=bison
# ---------------------------------------------------------------------
# General Compiler and linker Defines for SOLARIS
# ---------------------------------------------------------------------

#CC=gcc
LINK=ld
LIB_LINK=ld

CFLAGS=-O -c -w -Wall $(DEBUG_FLAGS) -I$(IBASE)/include
LIB_LINK_FLAGS=-O3 -G -Bsymbolic -lgds -lm -lc -lib_util -lcrypt $(MD_LIB) $(CRYPT_LIB) $(SUBSTS)
RM=rm -f
LEXFLAGS=-i
# ---------------------------------------------------------------------
# Generic Compilation Rules
#
# Do NOT change anything below this point.
# ---------------------------------------------------------------------
.SUFFIXES: .o .c .e 

.e.c:
	$(GPRE) $< -d $(DB)

.c.o:
	$(CC) $(CFLAGS)  $*.c

.o:
	$(LINK) -o $@ $< $(LINK_FLAGS)

# ---------------------------------------------------------------------
# The UDF objects and the MD5 Test objects
# ---------------------------------------------------------------------

# Triggers rebuild, needs a working flex
#LEXOUT=lex.yy
LEXOUT = Lexer

# Triggers rebuild, thus needs a working bison
#BISONOUT = ExprPars.tab
BISONOUT = ExprPars

MD5Test_OBJs = MDDriver.o $(MD_OBJECT)
UDF_OBJs = DateFncs.o MathFncs.o StrFncs.o Misc.o Debug.o Encrypt.o BlobFncs.o \
	   $(LEXOUT).o $(BISONOUT).o User.o $(MD_OBJECT)

install: FreeUDFLib.so
	@echo "------------------------------------------------------"
	@echo "You need to copy FreeUDFLib to the interbase lib "
	@echo "directory in order for the server to load it. "
	@echo "------------------------------------------------------"
	@echo "I can do this automagically for you, but you'll need"
	@echo "to type the password for root below. If you don't"
	@echo "trust me, then just bomb out of this and to install"
	@echo "type something like this (while logged in as root):"
	@echo "   cp FreeUDFLib.so /usr/interbase/lib"
	@echo "------------------------------------------------------"
	@echo
	echo 'cp FreeUDFLib.so $(IBASE)/lib' | su

all: FreeUDFLib.dll MD5Test

FreeUDFLib.so: $(UDF_OBJs)
	$(LIB_LINK) $(UDF_OBJs) -o $@ $(LIB_LINK_FLAGS)

MD5Test: $(MD5Test_OBJs)
	$(LINK) $(MD5Test_OBJs) -o $@ $(LINK_FLAGS) $(MD_LIB)

clean:
	rm -f MD5Test FreeUDFLib.so $(LEXOUT).c $(BISONOUT).c $(BISONOUT).h *.o *~

DateFncs.o: DateFncs.h udf_glob.h Debug.h

MathFncs.o: MathFncs.h udf_glob.h Debug.h

StrFncs.o: StrFncs.h udf_glob.h Debug.h

BlobFncs.o: BlobFncs.h udf_glob.h Debug.h

#MD5.o: MD5.h Debug.h # Uncomment if lacking a system-wide MD5 support

MDDriver.o: udf_glob.h Debug.h

Misc.o: Misc.h udf_glob.h Debug.h

Debug.o: Debug.h udf_glob.h

User.o: User.h udf_glob.h

Encrypt.o: Encrypt.h udf_glob.h

# Triggers rebuild, needs a working flex
#$(LEXOUT).c: SymTable.h $(LEXOUT).h
#	$(LEX) $(LEXFLAGS) Lexer.l 

$(LEXOUT).o: SymTable.h udf_glob.h Debug.h

# Triggers rebuild, needs a working bison
#$(BISONOUT).c $(BISONOUT).h: SymTable.h udf_glob.h Debug.h
#	$(YACC) -d ExprPars.y

