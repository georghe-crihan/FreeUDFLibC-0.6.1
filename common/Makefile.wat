# Makefile for WATCOM. You may need to edit config.h
DATOBJ=gds32dat.lib
IB_ROOT=C:\PROGRA~1\INTERB~1\INTERB~1
TIB_ROOT=D:\pap\orig\client~1
INC=$(TIB_ROOT)\include
ILIB=$(TIB_ROOT)\lib
CC=wcc386
AR=wlib
LD=wlink
LFLAGS=LIBPATH $(ILIB) RUNTIME console LIB CLIB3R LIB NT
CFLAGS=-W4 -ox -i=$(INC) -i=common -i=. -D_WIN32

OBJECTS=MD5.obj setenv.obj syslog.obj strlcpy.obj gmtime_r.obj windows.obj Debug.obj User.obj Misc.obj DateFncs.obj BlobFncs.obj Encrypt.obj MathFncs.obj StrFncs.obj ExprPars.obj Lexer.obj ppk_buf.obj access.obj ppkib1.obj formatdatetime.obj phonetiser.obj
OBJECTSl=MD5.obj,setenv.obj,syslog.obj,strlcpy.obj,gmtime_r.obj,windows.obj,Debug.obj,User.obj,Misc.obj,DateFncs.obj,BlobFncs.obj,Encrypt.obj,MathFncs.obj,StrFncs.obj,ExprPars.obj,Lexer.obj,ppk_buf.obj,access.obj,ppkib1.obj,formatdatetime.obj,phonetiser.obj

FreeUDFLib.dll: $(OBJECTS) $(DATOBJ)
	$(LD) $(LFLAGS) system nt_dll EXP=FreeUDFLib.def lib ib_util lib gds32.lib $(DATOBJ) F $(OBJECTSl)>>build.log

Debug.obj: Debug.c
	$(CC) $(CFLAGS) Debug.c >> build.log

User.obj: User.c
	$(CC) $(CFLAGS) User.c >> build.log

Misc.obj: Misc.c
	$(CC) $(CFLAGS) Misc.c >> build.log

DateFncs.obj: DateFncs.c
	$(CC) $(CFLAGS) DateFncs.c >> build.log

BlobFncs.obj: BlobFncs.c
	$(CC) $(CFLAGS) BlobFncs.c >> build.log

Encrypt.obj: Encrypt.c
	$(CC) $(CFLAGS) Encrypt.c >> build.log

MathFncs.obj: MathFncs.c
	$(CC) $(CFLAGS) MathFncs.c >> build.log

StrFncs.obj: StrFncs.c
	$(CC) $(CFLAGS) StrFncs.c >> build.log

# The two below should not be modified directly, but only through bison & flex
ExprPars.obj: ExprPars.c
	$(CC) $(CFLAGS) ExprPars.c >> build.log

Lexer.obj: Lexer.c
	$(CC) $(CFLAGS) Lexer.c >> build.log

MD5.obj: common\MD5.c
	$(CC) $(CFLAGS) common\MD5.c >> build.log

setenv.obj: common\setenv.c
	$(CC) $(CFLAGS) common\setenv.c >> build.log

strlcpy.obj: common\strlcpy.c
	$(CC) $(CFLAGS) common\strlcpy.c >> build.log

gmtime_r.obj: common\gmtime_r.c
	$(CC) $(CFLAGS) common\gmtime_r.c >> build.log

syslog.obj: common\syslog.c
	$(CC) $(CFLAGS) common\syslog.c >> build.log

windows.obj: windows.c
	$(CC) $(CFLAGS) windows.c >> build.log

# Local stuff

access.obj: local\access.c
	$(CC) $(CFLAGS) local\access.c >> build.log

ppkib1.obj: local\ppkib1.c
	$(CC) $(CFLAGS) local\ppkib1.c >> build.log

ppk_buf.obj: local\ppk_buf.c
	$(CC) $(CFLAGS) local\ppk_buf.c >> build.log

formatdatetime.obj: local\formatdatetime.c
	$(CC) $(CFLAGS) local\formatdatetime.c >> build.log

phonetiser.obj: local\phonetiser.c
	$(CC) $(CFLAGS) local\phonetiser.c >> build.log

# If You have a broken gds32_ms.lib
gds32dat.lib:
	$(AR) gds32dat +gds32.dll >> build.log
	                

clean:
	del $(OBJECTS) gds32_ms.lib gds32_ms.exp FreeUDFLib.exp FreeUDFLib.lib build.log

distclean: clean
	del FreeUDFLib.dll 

inst:
	copy FreeUDFLib.dll $(IB_ROOT)\lib\ppkib1.dll 
	copy FreeUDFLib.dll $(IB_ROOT)\lib\ppkIbacces.dll
        copy FreeUDFLib.dll $(IB_ROOT)\lib\PpkUDFLib.dll
