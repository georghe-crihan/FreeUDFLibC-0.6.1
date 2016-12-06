/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   User.c
 *     User maintenance routines declarations:
 *	UserMaintenanance -
 */

/* Obsolete! *DON'T USE THIS FUNCTION. IT WILL SOON NOT EXIST!* */
/* This is a function for adding users, deleting users and
  changing a user's password.
  It worked with IB 4.1, but I don't use it anymore (I use
  IBPassword in Encrypt.pas in conjunction with the included
  chgpasswd script to maintain passwords from my application).
  Since I don't use it anymore, it might not work now, and I
  probably won't fix it either.
  If anyone out there prefers this to IBPassword, and takes it
  upon themselves to fix this, let me know, and I'll add your fixes... 
 */

/* #define USER_MAINTENANCE_STUBBED */
/* This call is spawning another copy of the process. It is way too expensive
   for it. Plus, it's a security concern. Therefore IT SHOULD BE STUBBED.
   If Your application doesn't absolutely require it, please, stub it out.
*/

#ifndef USER_MAINTENANCE_STUBBED
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#endif /* USER_MAINTENANCE_STUBBED */
#include "udf_glob.h"
#include "User.h"
#include "StrFncs.h"
#include "Misc.h"

int UserMaintenance(char *username, char *password, char *sysuname,
                                   char *syspword, char *action)
{
  int result;
#ifndef USER_MAINTENANCE_STUBBED
pid_t  pid;
char   opadd[]="-add",
       opdel[]="-delete",
       opmod[]="-modify",
       oppas[]="-pw";
  char szApp[LOC_STRING_SIZE]; /* IB_BIN + '\gsec.exe' */
  char *sysu, *sysp,	/* System username and password (sysuname, syspword) */
       *uname, *pword;	/* username and password */
  char *app_argv[6];

  STARTDEB("UserMaintenance")
  /* Default the result to 100000 (i.e. failure). */
  result = UM_ERROR;

  /* Cleanup username, password, sysuname, syspword */
  uname=(char *) malloc(strlen(username)+1);
  rtrim(username, uname);
  pword=(char *) malloc(strlen(password)+1);
  rtrim(password, pword);
  sysu=(char *) malloc(strlen(sysuname)+1);
  rtrim(sysuname, sysu);
  sysp=(char *) malloc(strlen(syspword)+1);
  rtrim(syspword, sysp);

MSGDEB(1, (LOG_FACILITY, "UserName: %s, SysUName: %s, Action: %s", uname, sysu, action));

  /* Get the path of GSEC. */
  if (getenv("IB_BIN")!=NULL)
    sprintf(szApp, "%s"GSEC, getenv("IB_BIN"));
  else
    strcpy(szApp, IBHome"/bin/"GSEC);

  /* Running this program requires the following environment variables
    to be present as entered above: ISC_USER and ISC_PASSWORD. It is
    considered a security breach if the ISC_USER name and ISC_PASSWORD
    environment variables are set in the server, since just about anybody could
    get to them, thus users cannot change passwords by themselves. Rather, they
    must change their password in the presence of a DBA. 
  */

  if (strcmp(sysu, "SYSDBA")!=0) 
    return UM_SYSUSER_NOT_VALID;
  else
    if (setenv("ISC_USER", sysu, 0)==-1)
    return UM_CANNOT_SET_ENV_ISC_USERNAME;

  if (sysp == '\0')
    return UM_SYSPASSWORD_NOT_VALID;
  else 
    if (setenv("ISC_PASSWORD", sysp, 0)==-1)
    return UM_CANNOT_SET_ENV_ISC_PASSWORD;

  app_argv[0]=GSEC;
  app_argv[2]=uname;
  app_argv[3]=oppas;
  app_argv[5]=NULL;

  switch(*action) { 
    case 'A': app_argv[1]=opadd;
              app_argv[4]=pword;
    break;
    case 'D': app_argv[1]=opdel;
	      app_argv[3]=NULL;
    break;
    case 'M': app_argv[1]=opmod; 
	      app_argv[4]=pword;
    break;
    default: 
      return UM_CANNOT_DETERMINE_ACTION;
  }

    pid=shell(szApp, app_argv);
    if(waitchild(pid)==-1) result=UM_ERROR;

    free(uname);
    free(pword);
    free(sysu);
    free(sysp);
#else
  result=UM_ERROR;
  MSGDEB(1, (LOG_FACILITY, "STUB"));
#endif /* USER_MAINTENANCE_STUBBED */
MSGDEB(1, (LOG_FACILITY, "result: %d", result));
  ENDDEB("UserMaintenance")
  return result;
}
