#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "udf_glob.h"

#define MAX_LOG_BUF 1025

extern HANDLE Log_Mutex;
extern FILE *log_fp;

#define RP_START   if ( WAIT_OBJECT_0 != WaitForSingleObject(Log_Mutex, INFINITE) ) return;
#define RP_END	   fflush(log_fp); ReleaseMutex(Log_Mutex);
#define RP_STDLOG  fprintf(log_fp, "%p (%p): %s\n", esp, *((long *)esp), func_name);
#if (defined DEBUG && !defined HAVE_SYSLOG_H)
/* A quick syslog-like hack for Win32 */
void syslog(int priority, const char *message, ...)
{
va_list vl;
char str[MAX_LOG_BUF];
time_t curtime;
struct tm curtm;
DWORD pid;
STARTUPINFO sinfo;

RP_START
curtime=time(NULL);
gmtime_r(&curtime, &curtm);
strftime(str, MAX_LOG_BUF, "%b %d %H:%M:%S", &curtm); 
GetStartupInfo(&sinfo);
pid = GetCurrentProcessId();
fprintf(log_fp, "%s %d %s: ", str, pid, sinfo.lpTitle);
va_start(vl, message);
vsprintf(str, message, vl);
va_end(vl);
fprintf(log_fp, "%s\n", str);
RP_END
}
#endif /* DEBUG && !HAVE_SYSLOG_H */
