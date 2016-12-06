#include <stdio.h>
#include <string.h>
#include "../udf_glob.h"
#include <ctype.h>
#include <sys/types.h>
#include "../Misc.h"
#include "ppk_buf.h"


char tmp[16];
char *argvc[3]={"server", tmp, NULL};

int StartIb_ServerSocket(int *port)
{
if (*port==-1)
  *port=600;
sprintf(tmp, "%d", *port);
MSGDEB(0, (LOG_FACILITY, "Socket booting at %d", *port));
shell(IBHome"/UDF/server", argvc);
return 0;
}

/* Counters are session-independent -
 * they 1 in session field and Compteur in type field.
 */

int initcompteur(int *StatNum)
{
int r=0;
struct elem *res;
struct smem *t;
STARTDEB("initcompteur")
if ((t=trylockres())==NULL) {
  r=-1;
  goto unlock;
  }

if ((res=searchstationcounter(t->elems, *StatNum))==NULL)
  if ((res=addtointbuf(t->elems, *StatNum, 1, TYPE_CNTR))==NULL)
    r=-1; 
  
res->session=1;

unlock:
  unlockres(t);
ENDDEB("initcompteur")
return r;
}

int valeurcompteur(int *StatNum, int *Val)
{
int r=0;
struct elem *res;
struct smem *t;
STARTDEB("valeurcompteur")
if ((t=trylockres())==NULL) {
  r=-1;
  goto unlock;
  }

  if ((res=searchstationcounter(t->elems, *StatNum))==NULL)
    if ((res=addtointbuf(t->elems, *StatNum, 1, TYPE_CNTR))==NULL)
       r=-1;
res->session+=*Val;
r=res->session;

unlock:
unlockres(t);
ENDDEB("valeurcompteur")
return r;
}

int debutsignal(int *StatNum, int *SessNum, char *SigTypeString)
{
int r=-1;
struct smem *t;
STARTDEB("debutsignal")
if ((t=lockres())==NULL)
  goto unlock;

if (addtostrbuf(t->selems, *StatNum, *SessNum, SigTypeString, TYPE_SIGN)!=NULL)
  r=0;

unlock:
unlockres(t);
ENDDEB("debutsignal")
return r;
}

int finsignal(int *StatNum, int *SessNum, char *SigTypeString)
{
int r=-1;
struct smem *t;
STARTDEB("finsignal")
if ((t=lockres())==NULL)
  goto unlock;

r=freestrbufelem(t->selems, *StatNum, *SessNum, SigTypeString, TYPE_SIGN);

unlock:
unlockres(t);
ENDDEB("finsignal")
return r;
}

int cherchesignal(int *StatNum, int *SessNum, char *SigTypeString)
{
int r=-1;
struct smem *t;
STARTDEB("cherchesignal")
if ((t=lockres())==NULL)
  goto unlock;

if (searchstrbuf(t->selems, *StatNum, *SessNum, SigTypeString, TYPE_SIGN)!=NULL)
  r=0;

unlock:
unlockres(t);
ENDDEB("cherchesignal")
return r;
}

int creerverrou(int *StatNum, int *SessNum, char *LockString)
{
int r=0;
int i;
struct smem *t;
STARTDEB("creerberrou")
if ((t=trylockres())==NULL) {
  r=-1;
  goto unlock;
  }

for (i=0; i < MAXSBUF; i++)
  if (t->selems[i].type==TYPE_LOCK)
    if (strcasecmp(t->selems[i].typestr, LockString)==0) {
      if (t->flags & FPPKDEBUG)
        OutputElementDebugInfo(NULL, (union sdump *) (t->selems + i), "Erreur V --> ");
      r=-1;
    }

if (addtostrbuf(t->selems, *StatNum, *SessNum, LockString, TYPE_LOCK)==NULL)
  r=-1;

unlock:
unlockres(t);
ENDDEB("creerverrou")
return r;
}

int libererverrou(int *StatNum, int *SessNum, char *LockString)
{
int r=-1;
struct smem *t;
STARTDEB("libererverrou")
if ((t=lockres())==NULL)
  goto unlock;

r=freestrbufelem(t->selems, *StatNum, *SessNum, LockString, TYPE_LOCK);

unlock:
unlockres(t);
ENDDEB("libererverrou")
return r;
}

int liberertousverrous(void)
{
int r=-1;
int i;
struct smem *t;
STARTDEB("liberertousverrous")
if ((t=lockres())==NULL)
  goto unlock;

for (i=0; i < MAXSBUF; i++)
  if (t->selems[i].type==TYPE_LOCK)
    t->selems[i].type^=TYPE_USED;
r=0;

unlock:
unlockres(t);
ENDDEB("liberertousverrous")
return r;
}

/*
 * Connections have a 'Connexion' in their ElemTypeString.
 */

int essaiconnexion(int *StatNum, int *SessNum, char *Action)
{
int r=-1;
int i;
struct smem *t;
STARTDEB("essaiconnexion")
if ((t=trylockres())==NULL)
  goto unlock;

if ( (tolower(*Action)=='p') && (Action[1]=='\0') ) {
/* Do cleanup */
  for (i=0; i < MAXSBUF; i++)
    if (t->selems[i].station==*StatNum)
      if ((t->selems[i].type==TYPE_LOCK) || (t->selems[i].type==TYPE_SIGN))
        t->selems[i].type^=TYPE_USED;
  for (i=0; i < MAXBUF; i++)
    if (t->elems[i].station==*StatNum)
      if (t->elems[i].type==TYPE_CONN)
        t->elems[i].type^=TYPE_USED;
  } 
if (accountconnections(t->elems, 0) != CONNECTION_LIMIT) {
  addtointbuf(t->elems, *StatNum, *SessNum, TYPE_CONN);
  r=accountconnections(t->elems, -1);
  }

unlock:
unlockres(t);
ENDDEB("essaiconnexion")
return r; 
}

int finconnexion(int *StatNum, int *SessNum)
{
int i;
int r=-1;
struct smem *t;
STARTDEB("finconnexion")
if ((t=lockres())==NULL)
  goto unlock;

for (i=0; i < MAXSBUF; i++)
  if (t->selems[i].station==*StatNum)
    if (t->selems[i].session==*SessNum)
      if ( (t->selems[i].type==TYPE_SIGN) || (t->selems[i].type==TYPE_LOCK) )
        t->selems[i].type^=TYPE_USED;

for (i=0; i < MAXBUF; i++)
  if (t->elems[i].station==*StatNum)
    if (t->elems[i].session==*SessNum)
      if (t->elems[i].type==TYPE_CONN)
        t->elems[i].type^=TYPE_USED;

r = accountconnections(t->elems, -1);

unlock:
unlockres(t);
ENDDEB("finconnexion")
return r;
}

int compteconnexions(int *station)
{
int r=0;
struct smem *t;
STARTDEB("compteconnexions")
if ((t=lockres())==NULL)
  goto unlock;

r=accountconnections(t->elems, *station);

unlock:
unlockres(t);
ENDDEB("compteconnexions")
return r;
}

/*
 * Debugging stuff
 */

int debutecriture(void)
{
int r=-1;
struct smem *t;
STARTDEB("debutecriture")
if ((t=lockres())==NULL)
  goto unlock;

if (t->flags & FPPKDEBUG)
  goto unlock;
//debugfilehandle=fopen(wintransform("C:\\SUIVI.TXT"), "rw+"); /* append to it */
t->flags|=FPPKDEBUG;
r=0;

unlock:
unlockres(t);
ENDDEB("debutecriture")
return r;
}

int finecriture(void)
{
int r=-1;
struct smem *t;
STARTDEB("finecriture")
if ((t=lockres())==NULL)
  goto unlock;

if (!(t->flags & FPPKDEBUG))
  goto unlock;
//fclose(debugfilehandle);
t->flags^=FPPKDEBUG;
r=0;

unlock:
unlockres(t);
ENDDEB("finecriture")
return r;
}

int forcerecriture(char *What)
{
int r=-1;
FILE *aud;
char c;
struct smem *t;
STARTDEB("forcerecriture")
if ((t=trylockres())==NULL)
  goto unlock;

aud=fopen(IBHome"/log/C__AUDIT.TXT", "a+"); /* Create/append */

if (aud==NULL)
MSGDEB(0, (LOG_FACILITY, "forcerecriture"));


if (What[1]!='\0') {
  fclose(aud);
  r=0;
  goto unlock;
  }

c= (char) tolower(What[0]);

switch(c) {
  case 's':
    dumpbuf(t, aud, TYPE_SIGN);
    break;

  case 'v':
    dumpbuf(t, aud, TYPE_LOCK);
    break;

  case 'c':
    dumpbuf(t, aud, TYPE_CONN);
    break;

  case 't':
    dumpbuf(t, aud, TYPE_CONN);
    dumpbuf(t, aud, TYPE_LOCK);
    dumpbuf(t, aud, TYPE_SIGN);
    break;

  default:
    MSGDEB(2, (LOG_FACILITY, "Unknown dump"));
    break;
}
fclose(aud);
r=0;

unlock:
unlockres(t);
ENDDEB("forcerecriture")
return r;
}
