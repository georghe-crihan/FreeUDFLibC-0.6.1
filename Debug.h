/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   Debug.h
 *     Debugging routines declarations:
 *       SetDebuggerOutput -
 *	 CloseDebuggerOutput -
 *	 Debug - 
 */

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#define MSGDEB(a,b) SYSDEB##a(b)


/* log critical events */
#ifdef DEBUG 
#ifndef HAVE_SYSLOG_H /* e.g. _WIN32 */
#include <stdio.h>
#include <stdarg.h>
#define LOG_LOCAL1 0
#define LOG_INFO 1
#ifdef __cplusplus
 extern "C"
 {
#endif
void syslog(int, const char *, ...); /* Replacement! */
#ifdef __cplusplus
 }
#endif
#else /* UNIX */
#include <syslog.h>
#endif /* !HAVE_SYSLOG_H */ 

#include <errno.h>
#define SYSDEB0(syslog_args) syslog syslog_args
#if (DEBUG >=1)
#define SYSDEB1(syslog_args) syslog syslog_args
#endif

#if (DEBUG >=2)
#define SYSDEB2(syslog_args) syslog syslog_args
#endif

#if (DEBUG >=3)
#define SYSDEB3(syslog_args) syslog syslog_args
#endif

#endif /* DEBUG */

#ifndef SYSDEB0
#define SYSDEB0(a)
#endif

#ifndef SYSDEB1
#define SYSDEB1(a)
#endif

#ifndef SYSDEB2
#define SYSDEB2(a)
#endif

#ifndef SYSDEB3
#define SYSDEB3(a)
#endif

#define STARTDEB(EntryName) MSGDEB(1, (LOG_FACILITY, EntryName"() - Enter"));
#define ENDDEB(EntryName) MSGDEB(1, (LOG_FACILITY, EntryName"() - Exit"));

#ifdef __cplusplus
 extern "C"
 {
#endif

int CloseDebuggerOutput PROTO_LIST ((void));
int Debug PROTO_LIST ((char *));
int SetDebuggerOutput PROTO_LIST ((char *));

#ifdef __cplusplus
 }
#endif
#endif /* DEBUG_H_INCLUDED */
