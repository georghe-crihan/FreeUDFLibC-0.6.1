#include <windows.h>
#include <stdio.h>

const char *cstr_LogFilename = "c:\\udf_lib.log";

HANDLE Log_Mutex = NULL;
FILE *log_fp = NULL;

int
LogInit(void)
{
  if ( NULL == ( Log_Mutex = CreateMutex(NULL, FALSE, NULL) ) )
    return FALSE;
  if ( NULL == ( log_fp = fopen(cstr_LogFilename, "a") ) )
    return FALSE;
  return TRUE;
}

int
LogDeInit(void)
{
  if ( NULL != log_fp )
  {
    fclose(log_fp);
    log_fp = NULL;
  }
  if ( NULL != Log_Mutex )
  {
    CloseHandle(Log_Mutex);
    Log_Mutex = NULL;
  }
  return TRUE;
}

BOOL WINAPI
DllMain(
 HINSTANCE hinstDll,
 DWORD fdwReason,
 LPVOID lpvReserved
)
{
  switch(fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      if ( !LogInit() )
       return FALSE;
     break;
    case DLL_PROCESS_DETACH:
      LogDeInit();
     break;
  }
  return TRUE;
}
