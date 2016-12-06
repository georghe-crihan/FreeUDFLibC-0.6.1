/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   Misc.c
 *     Miscellaneous routines:
 *	 ValidateBillingExpression -
 *	 IBTempPath -
 *	 GenerateFormattedName -
 *	 ValidateNameFormat -
 *	 EvaluateCycleExpression -
 *	 ValidateCycleExpression -
 *	 ValidateRegularExpression -
 *	 ValidateStringInRE -
 */

#include "udf_glob.h"
#include <string.h>
#include <ctype.h> 
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#ifdef HAVE_VFORK_H
#include <vfork.h>
#endif

#ifdef _WIN32
#include <process.h>
#endif

#include "Misc.h"
#include "DateFncs.h"

static void appchr(char *, char);
static int internalEvaluateCycleExpression(char **, ISC_QUAD *, ISC_QUAD *,
								double *);
static int internalGenerateFormattedName(char *, char *, char *, char *, 
							char *, char *, char *);

#define IBTMP "C:\\Program Files\\Borland\\IntrBase"

int ValidateBillingExpression(char *szCycleExpression, int *ExprStart)
{
  STARTDEB("ValidateBillingExpression");
  return 1;
}

char *wintransform(char *name, char *dest)
{
#ifndef _WIN32
unsigned int i;
unsigned int j=strlen(dest);
#endif /* !_WIN32 */

STARTDEB("wintransform")
#ifndef _WIN32
for (i=0; i <= strlen(name); i++) {
  if (isalnum(name[i]))
    dest[j]=name[i];
  else
    dest[j]='_';
  j++;
  }
dest[j]='\0';
#else /* _WIN32 */
strcpy(dest, name);
#endif /* !_WIN32 */
ENDDEB("wintransform")
return name;
}

char *IBTempPath(void)
{
  char *a;

  STARTDEB("IBTempPath")
  a=(char *) MALLOC(LOC_STRING_SIZE);
  a[0]='\0';
  strcpy(a, IBTMP);
  ENDDEB("IBTempPath")
  return a;
}


pid_t shell(char *szApp, char **app_argv)
{
pid_t pid;

STARTDEB("shell")

#ifndef _WIN32
if (!(pid = vfork()))
    {
    chdir(IBHome); /* ? gsec won't work otherways? */
    execv (szApp, app_argv);
MSGDEB(1, (LOG_FACILITY, "shell: execv(): %s", strerror(errno)));
    return -1;
    }
#else /* _WIN32 */	
    pid = spawnv(P_NOWAIT, szApp, app_argv);
#endif /* !_WIN32 */

MSGDEB(1, (LOG_FACILITY, "shell: Spawned pid: %d", pid));

ENDDEB("shell")
   return pid;
}

int waitchild(pid_t pid)
{
int Status;

STARTDEB("waitchild")

#ifndef _WIN32
/* wait for the child process with pid to exit */

while (waitpid (pid, &Status, 0) == -1) /*sleep(5)*/;

/* Check for very specific conditions before we assume the child
   exited normally. */

if (WIFEXITED (Status) && 
    (WEXITSTATUS (Status) != 0)) {

MSGDEB(1, (LOG_FACILITY, "Child Exit Status: %d", WEXITSTATUS (Status)));

    Status=-1;
    goto Done;
}

if (
    WIFSIGNALED (Status) ||
    !WIFEXITED (Status) ) {
MSGDEB(1, (LOG_FACILITY, "ERROR")); 
      Status=-1;
      goto Done;
    }


Done:

#else /* _WIN32 */
cwait(&Status, pid, WAIT_CHILD);
Status = (Status & 3) ? -1 : Status;
#endif /* !_WIN32 */

ENDDEB("waitchild")
   return Status;
}

static void appchr(char *String, char ch)
{
int i;

STARTDEB("appchr")
i=strlen(String);
String[i]=ch;
i++;
String[i]='\0';
ENDDEB("appchr")
}

/****************************************************************************
* Some nice "data formatting routines"                                      *
*****************************************************************************

*****************************************************************************
* Formatting names for "pretty" output.                                     *
* ***************************************************************************
* Valid format "fields" are:                                                *
* \<pretext>\<posttext>\P  name prefix                                      *
* \<pretext>\<posttext>\F  first name                                       *
* \<pretext>\<posttext>\M  middle initial                                   *
* \<pretext>\<posttext>\L  last name                                        *
* \<pretext>\<posttext>\S  name suffix                                      *
* @ - escape character; anything following an @ will be printed as a literal*
* --options--                                                               *
* \\\A   print only alpha's                                                 *
* \\\9   print only alpha's and numeric's                                   *
* \\\U   uppercase string                                                   *
* All other strings are just printed as-is.                                 *
* You use "pretext" and "posttext" values when the text preceding or        *
* succeeding a field depends on the presence of a value in the field.       *
* An example:                                                               *
*  "\\ \P\\ \F\\. \M\\\L\, \\S"                                             *
*  yields                                                                   *
*  Mr. John M. Smith, III    - or -                                         *
*  John Smith                - or -                                         *
*  John M. Smith             etc....                                        *
*****************************************************************************
* Just as a small note: This is not complicated, but the strings might      *
* initially seem that way... The intent of this function is so that the     *
* developer can ask the back-end to produce arbitrarily formatted name      *
* strings, but include some intelligent decision-making about how to print  *
* intermediate text.                                                        *
*****************************************************************************/
const char
  FORMAT_CHAR = '\\',
  ESC_CHAR = '@';

#define  STATE_INSERT 1
#define  STATE_PRESTR 2
#define  STATE_POSTSTR 3
#define  STATE_FORMAT 4
#define  STATE_ESC 5
#define  STATE_PRESTR_ESC 6
#define  STATE_POSTSTR_ESC 7

char * GenerateFormattedName(char *szFormatString, char *szNamePrefix, 
  char *szFirstName, char *szMiddleInitial, char *szLastName,
   char *szNameSuffix)
{
  char *strInternalResult=NULL;
  STARTDEB("GenerateFormattedName")
  SETLOCALE
  strInternalResult=(char *) MALLOC(LOC_STRING_SIZE);
  internalGenerateFormattedName(szFormatString, strInternalResult,
                                szNamePrefix, szFirstName, szMiddleInitial,
                                szLastName, szNameSuffix);
  ENDDEB("GenerateFormattedName")
  return strInternalResult;
}

static int internalGenerateFormattedName(char *strFormatString,
  char *strResult, char *strNamePrefix, char *strFirstName, 
  char *strMiddleInitial, char *strLastName, char *strNameSuffix)
{
  unsigned int i, result,
      CurrentState=STATE_INSERT;
  int OptionA=0, 
      Option9=0,
      OptionU=0;
char  PreString[LOC_STRING_SIZE], 
      PostString[LOC_STRING_SIZE],
      AlphaString[LOC_STRING_SIZE];

STARTDEB("internalGenerateFormattedName")
SETLOCALE
  *PreString = '\0';
  *PostString = '\0';
  *strResult = '\0';
  result = 1;
  for (i=0; i < strlen(strFormatString); i++)
    switch (CurrentState) { 
      case STATE_INSERT: 
        if (strFormatString[i] == FORMAT_CHAR)
          CurrentState = STATE_PRESTR;
        else if (strFormatString[i] == ESC_CHAR)
          CurrentState = STATE_ESC;
        else
          appchr(strResult,strFormatString[i]);
      break; 
      case STATE_ESC:
        appchr(strResult, strFormatString[i]);
        CurrentState = STATE_INSERT;
      break;
      case STATE_PRESTR:
        if (strFormatString[i] == FORMAT_CHAR)
          CurrentState = STATE_POSTSTR;
        else if (strFormatString[i] == ESC_CHAR)
          CurrentState = STATE_PRESTR_ESC;
        else
          appchr(PreString, strFormatString[i]);
      break;
      case STATE_PRESTR_ESC:
        appchr(PreString, strFormatString[i]);
        CurrentState = STATE_PRESTR;
      break;
      case STATE_POSTSTR:
        if (strFormatString[i] == FORMAT_CHAR)
          CurrentState = STATE_FORMAT;
        else if (strFormatString[i] == ESC_CHAR)
          CurrentState = STATE_POSTSTR_ESC;
        else
          appchr(PostString, strFormatString[i]);
      break;
      case STATE_POSTSTR_ESC:
        appchr(PostString, strFormatString[i]);
        CurrentState = STATE_POSTSTR;
      break;
      case STATE_FORMAT:
        switch (strFormatString[i]) { 
          case 'P': if (strNamePrefix != NULL) {
              strcat(strResult, PreString);
              strcat(strResult, strNamePrefix);
              strcat(strResult, PostString);
              }
          break;
          case 'F': if (strFirstName != NULL) {
              strcat(strResult, PreString);
              strcat(strResult, strFirstName);
              strcat(strResult, PostString);
              }
          break;
          case 'M': if (strMiddleInitial != NULL) {
              strcat(strResult, PreString);
              strcat(strResult, strMiddleInitial);
              strcat(strResult, PostString);
              }
          break;
          case 'L': if (strLastName != NULL) {
              strcat(strResult, PreString);
              strcat(strResult, strLastName);
              strcat(strResult, PostString);
              }
          break;
          case 'S': if (strNameSuffix != NULL) {
              strcat(strResult, PreString);
              strcat(strResult, strNameSuffix);
              strcat(strResult, PostString);
              }
          break;
          case 'A': if (!Option9) OptionA = 1; else goto OutOfFor; 
	  break;
          case '9': if (!OptionA) Option9 = 1; else goto OutOfFor; 
          break;
          case 'U': OptionU = 1;
          break;
          default: goto OutOfFor; 
          break;
        } /* switch(strFormatString[i]) */
        *PreString = '\0';
        *PostString = '\0';
        CurrentState = STATE_INSERT;
      break; /* STATE_FORMAT */ 
    } /* switch(CurrentState) */
 
OutOfFor:

 if (CurrentState != STATE_INSERT)
    result = 0;
  if (!result)
    sprintf(strResult, "Position: %d  CurrentState: %d", i, CurrentState);
  else { 
    if (Option9 || OptionA) {
      for (i=0; i < strlen(strResult); i++) {
        if (isalpha(strResult[i]))
          appchr(AlphaString, strResult[i]);
        if (isdigit(strResult[i]) && Option9)
          appchr(AlphaString, strResult[i]);
        } /* for () */
      strcpy(strResult, AlphaString);
      } /* if() */
    } /* else */
    if (OptionU)
      for(i=0; i < strlen(strResult); i++) toupper(strResult[i]);
  ENDDEB("internalGenerateFormattedName")

return result;
} /* internalGenerateFormattedName */

int ValidateNameFormat(char *szFormatString)
{
  char strInternalResult[LOC_STRING_SIZE+1];

  STARTDEB("ValidateNameFormat")
SETLOCALE
  if (internalGenerateFormattedName(szFormatString, strInternalResult,
                                           NULL, NULL, NULL, NULL, NULL)) 
    return 1;
  else
    return 0;
}

int ValidateCycleExpression(char *sz, int *ExprStart)
{
char *s=sz+*ExprStart;
ISC_QUAD t;
double d = 0;

t.isc_quad_high = t.isc_quad_low=0; /* Dummy quad */

  STARTDEB("ValidateCycleExpression")
MSGDEB(2, (LOG_FACILITY, "Expr: %s, Start: %d", sz, *ExprStart));
return ((2 & internalEvaluateCycleExpression(&s, &t, &t, &d)) ? 0 : 1);
}

static int internalEvaluateCycleExpression(char **sz, ISC_QUAD *OldDate,
					ISC_QUAD *NewDate, double *Amount)
{
   int len, DateInt, AgeInt;
   double AmountD;

  STARTDEB("internalEvaluateCycleExpression")
MSGDEB(2, (LOG_FACILITY, "Expr: %s", *sz));

for (len=strlen(*sz); isspace(**sz); len--) {
  (*sz)++;
  if (len <= 0)
    return 0;
  }

if (isdigit(**sz))
  if (sscanf(*sz, "%d%n", &DateInt, &len)==1) {	/* Is it BSD-specific? */
    *sz+=len;					/* Eat digits */
    AgeInt = AgeInMonths(NewDate, OldDate);
    if ((AgeInt % DateInt) == 0)
       return 1;				/* Return true */
    return 0;					/* Return false */
    } else return 2;				/* Indicate error */
     
switch (tolower(**sz)) { 
  case '>':
    if ((*sz)[1] == '=') { 
      *sz+=2;
      if (sscanf(*sz, "%d%n", &DateInt, &len)==1) {/* Is it BSD-specific? */
	*sz+=len;				/* Eat digits */
        AgeInt = AgeInMonths(NewDate, OldDate);
MSGDEB(2, (LOG_FACILITY, "New: %ld, Old: %ld", *NewDate, *OldDate));
MSGDEB(2, (LOG_FACILITY, "AgeInt: %d", AgeInt));
        if (AgeInt < DateInt) 
          return 0;				/* Return false */
	return 1;				/* Return true */
	}
      }
  break;					/* '>' */
  case '$': 
    (*sz)++;
    if (sscanf(*sz, "%lf%n", &AmountD, &len)==1) {/* Is it BSD-specific? */
      *sz+=len;					/* Eat digits */
MSGDEB(2, (LOG_FACILITY, "Amount: %lf", AmountD));
      if (*Amount < AmountD)
        return 0;				/* Return false */
      return 1;					/* Return true */
      }
  break;					/* '$' */
  case 'a':
    if ((strlen(*sz) >= 3) && 
        (tolower((*sz)[1]) == 'n') && (tolower((*sz)[2]) == 'd') ) {
      *sz+=3;					/* Eat and */
        
      len = internalEvaluateCycleExpression(sz, OldDate, NewDate, Amount);
      DateInt = internalEvaluateCycleExpression(sz, OldDate, NewDate, Amount);
	   /* T/F      Error bit propagation */	/* Some boolean arithmetic */
      return (len & DateInt) | ((len | DateInt) & 2);
       }
  break;					/* and */
  case 'o':
    if ((strlen(*sz) >= 2) && (tolower((*sz)[1])=='r')) {
      *sz+=2;					/* Eat or */
      return (internalEvaluateCycleExpression(sz, OldDate, NewDate, Amount) |
              internalEvaluateCycleExpression(sz, OldDate, NewDate, Amount) );
     }
  break;					/* or */
  case 'n':
    if ((strlen(*sz) >= 3) &&
        (tolower((*sz)[1]) == 'o') && (tolower((*sz)[2]) == 't') ) {
      *sz+=3;					/* Eat not */
      return internalEvaluateCycleExpression(sz, OldDate, NewDate, Amount) ^ 1;
      }
  break;					/* not */
  default: ;					/* do nothing */
  }
return 2;					/* Indicate error */
}					/* internalEvaluateCycleExpression */


int EvaluateCycleExpression(char *sz, int *ExprStart, 
   ISC_QUAD *OldDate, ISC_QUAD *NewDate, double *Amount)
{
char *s=sz+*ExprStart;
int t;
STARTDEB("EvaluateCycleExpression")
MSGDEB(2, (LOG_FACILITY, "Expr: %s, Start: %d", sz, *ExprStart));

t = internalEvaluateCycleExpression(&s, OldDate, NewDate, Amount);

if (t < 2)
  return t & 1;
else
  return 0;				/* Force false on error */
}

int ValidateRegularExpression(char *sz)
{
  STARTDEB("ValidateRegularExpression")
  return 1;
}

int ValidateStringInRE(char *sz, char *re)
{
  STARTDEB("ValidateStringInRE")
  return 1;
}
