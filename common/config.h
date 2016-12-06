/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have <vfork.h>.  */
/* #undef HAVE_VFORK_H */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if your <sys/time.h> declares struct tm.  */
/* #undef TM_IN_SYS_TIME */

/* Define vfork as fork if vfork does not work.  */
/* #undef vfork */

/* Define if lex declares yytext as a char * by default, not a char[].  */
#define YYTEXT_POINTER 1

/* Earlier InterBase patches */
#define HAVE_BLOBCALLBACK 1

/* Define strcasecmp as stricmp if we do not have it */
/* #undef strcasecmp */

/* Multi-threaded version of malloc */
#define MALLOC ib_util_malloc

/* InterBase home directory */
#define IBHome "/usr/interbase"

/* InterBase gsec */
#define GSEC "gsec"

/* Version stuff */
#define RELEASE "1999-Jul-12"

/* Define if you have the gmtime_r function.  */
#define HAVE_GMTIME_R 1

/* Define if you have the mktime function.  */
#define HAVE_MKTIME 1

/* Define if you have the setenv function.  */
#define HAVE_SETENV 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strlcpy function.  */
#define HAVE_STRLCPY 1

/* Define if you have the strstr function.  */
#define HAVE_STRSTR 1

/* Define if you have the strtol function.  */
#define HAVE_STRTOL 1

/* Define if you have the <malloc.h> header file.  */
/* #undef HAVE_MALLOC_H */

/* Define if you have the <string.h> header file.  */
/* #undef HAVE_STRING_H */

/* Define if you have the <syslog.h> header file.  */
#define HAVE_SYSLOG_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the crypt library (-lcrypt).  */
#define HAVE_LIBCRYPT 1

/* Define if you have the gds library (-lgds).  */
#define HAVE_LIBGDS 1

/* Define if you have the md library (-lmd).  */
#define HAVE_LIBMD 1

/* Name of package */
#define PACKAGE "FreeUDFLibC"

/* Version number of package */
#define VERSION "0.6.1"

/* Define if compiler has function prototypes */
#define PROTOTYPES 1

/* ISO Locale, see locale(3) */
/* #undef LOCALE_SUPPORT */

/* Debug level, 2 - most verbose */
/* #undef DEBUG */

/* Desired syslog facility - see syslog(3) */
#define LOG_FACILITY LOG_INFO

