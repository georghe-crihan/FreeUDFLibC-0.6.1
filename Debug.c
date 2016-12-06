/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   Debug.c
 *     Debugging routines declarations:
 *       SetDebuggerOutput -
 *	 CloseDebuggerOutput -
 *	 Debug - 
 */

/*
  It's a stub collection used primarily for compatibility with DELPHI version of
  this library. Under UNIX (FreeBSD),  I use syslog as a debugging facility. 
  If You  need to run it under Win32, then... try  compiling it, but,
  please, don't forget to rewrite and clean it up first.
*/

#include "udf_glob.h"
#include "Debug.h"

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>

HANDLE  hDebugger=NULL;
int Console;
//CRITICAL_SECTION  csDebugger;
HANDLE DebuggerMutex = NULL;
#endif

int CloseDebuggerOutput(void)
{
int result;

  STARTDEB("CloseDebuggerOutput")

#ifdef _WIN32
//  EnterCriticalSection(&csDebugger);
  /* Default to failure. */
  result = UDF_FAILURE;
  if ((DebuggerMutex==NULL) || (hDebugger==NULL))
    return result;
  /* if I've indicated an actual handle, then close it */
    if (Console) /* if I've indicated STDOUT, then Free the console */
      FreeConsole();
    else
      fclose(hDebugger);
    /* Ensure that the debugger handle is 0. */
    hDebugger = NULL;
    CloseHandle(DebuggerMutex);
    DebuggerMutex = NULL;
//  LeaveCriticalSection(&csDebugger);
#endif
    /* Indicate success. */
    result = UDF_SUCCESS;
    ENDDEB("CloseDebuggerOutput")
    return result;
} 

#ifdef _WIN32
const char szConsoleMsg[] = "FreeUDFLib Debugger Console:\n" \
                 "  Press Ctrl-Enter to switch between full-screen and window.\n\n";
#endif

/* Important notes regarding Debug:
   This function is meant only for its intended purpose.
   As you will note, I did not include mechanisms for many users
   to be cleanly dumping data to the debug window. Don't expect this
   function to perform as you might like if you have multiple users
   calling it!
 */
int Debug(char *szDebuggerOutput)
{
   int result;
#ifdef _WIN32
  HANDLE hOutput;
//  unsigned int BytesWritten;
//  char *sOutput[BLOB_STRING_SIZE];
#endif /* _WIN32 */

  STARTDEB("Debug")
  MSGDEB(2, (LOG_FACILITY, "%s", szDebuggerOutput));

#ifdef _WIN32

//  EnterCriticalSection(&csDebugger);
  /* Default to failure. */
  result = UDF_FAILURE;
  if ( WAIT_OBJECT_0 != WaitForSingleObject(DebuggerMutex, INFINITE) )
    return result;
  if (hDebugger != NULL) { 
    /* indicates STDOUT, so let's do just that */
    if (Console) { 
      /* Try to get STDOUT's handle. */
      hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
      /* if I couldn't get STDOUT's handle, then AllocConsole probably needs
       * to be called.
       */
      if (hOutput == INVALID_HANDLE_VALUE) {
        /* Allocate a console. */
        AllocConsole();
        /* Just check that everything went right, and put a friendly
         * message at the top of the console window.
	 */
        hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOutput == INVALID_HANDLE_VALUE) return result;
        fputs(szConsoleMsg, hOutput);
//, strlen(szConsoleMsg), &BytesWritten, NULL);
      } 
    } else
      hOutput = hDebugger;
    /* Now, I can write my debugging stuff! whew. */
//    sprintf(sOutput, "%s\n", szDebuggerOutput);
//    WriteFile(hOutput, sOutput, sizeof(sOutput), &BytesWritten, NULL);
    fprintf(hOutput, "%s\n", szDebuggerOutput);
  }
  fflush(hOutput);
  ReleaseMutex(DebuggerMutex);
//  LeaveCriticalSection(&csDebugger);
#endif    
    result = UDF_SUCCESS;
    ENDDEB("Debug");
    return result;
}


int SetDebuggerOutput(char *szOutputFile)
{
  int result;
  STARTDEB("SetDebuggerOutput")
  MSGDEB(2, (LOG_FACILITY, "%s", szOutputFile));

#ifdef _WIN32
//  EnterCriticalSection(&csDebugger);
  DebuggerMutex = CreateMutex(NULL, FALSE, NULL);
  /* Default to failure. */
  result = UDF_FAILURE;
  if (DebuggerMutex==NULL)
    return result;
  /* If the user asked for STDOUT, then set Console to 1. */
  if (lstrcmpi(szOutputFile, "STDOUT") == 0) { 
    if (hDebugger != NULL)
      CloseDebuggerOutput();
    Console=1;
  } else {
    CloseDebuggerOutput();
    hDebugger = CreateFile(szOutputFile, GENERIC_WRITE, 0, NULL,
                               CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL || FILE_FLAG_WRITE_THROUGH,
                               0);
    if (hDebugger == INVALID_HANDLE_VALUE)
      hDebugger = NULL;
  } 
//  LeaveCriticalSection(&csDebugger);
#endif
    result = UDF_SUCCESS;
    ENDDEB("SetDebuggerOutput")
    return result;
}

