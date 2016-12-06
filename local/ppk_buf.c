#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef BSD
#include <machine/param.h>
#endif

#include <sys/types.h>
#ifndef _WIN32
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#endif

#ifdef linux
#include <sys/stat.h>
#define SEM_A S_IWGRP 666
#define SEM_R S_IRGRP
#endif

#include "ppk_buf.h"
#include "../udf_glob.h"

static char *formattime(time_t, char *);

/* #define TRACE_HISTORY */
/*
 *  The following is an (probably buggy and lame) implementation of a global 
 *  memory buffer through the UNIX System V(R) compatible IPC shared memory and
 *  semaphore facilities. To my best knowledge, it should not cause serious
 *  mutual locks or waste of system resources.
 *  CAVEAT: There is no function, to release the resources, so you should use
 *  ipcs(1) to remove the semaphore set and the shared memory segment, if
 *  necessary.
 */

/* Below is used to initialize the resource for the first time */
struct smem *trylockres(void)
{
#ifndef _WIN32
key_t ipckey, key;
int semid, shmid;
struct smem *shmadr;

if ( ( shmadr=lockres() )!=NULL )
  return shmadr;
else 
  if (errno!=ENOENT)
    return NULL;

/* Generate IPC key for the semaphore set */
key=ftok(IPCFILE, PPK_SEM_SET_ID);
if (key==-1) {
MSGDEB(0,(LOG_FACILITY, "trylockres() ftok() : %s", strerror(errno)));
  return NULL;
  }

/* Create and lock resource */
semid=semget(key, 1, IPC_CREAT | IPC_EXCL | SEM_R | SEM_A );
if (semid==-1) {
MSGDEB(0,(LOG_FACILITY, "semget() : %s", strerror(errno)));
  return NULL;
  }

/* Generate IPC key for the shared memory segment */
ipckey=ftok(IPCFILE, SHARED_MEMORY_ID);
if (ipckey==-1) {
MSGDEB(0, (LOG_FACILITY, "ftok() : %s", strerror(errno)));
  shmadr=NULL;
  goto semclear;
  }

shmid=shmget(ipckey, sizeof(struct smem), IPC_CREAT | SHM_R | SHM_W );
if (shmid==-1) {
MSGDEB(0, (LOG_FACILITY, "shmget() : %s", strerror(errno)));
  shmadr=NULL;
  goto semclear;
  }

/* Attach the shared memory segment */
shmadr = (struct smem *) shmat(shmid, 0, 0);
if ((int)shmadr==-1) {
MSGDEB(0, (LOG_FACILITY, "shmat() : %s", strerror(errno)));
  shmadr=NULL;
  goto semclear;
  }

MSGDEB(2,(LOG_FACILITY, "Resource allocated"));

/* do initialization stuff */
memset(shmadr, '\0', sizeof(struct smem));
shmadr->magic=PPK_MAGIC;

#ifdef TRACE_HISTORY
shmadr->res1=MAXBUF;
shmadr->res2=MAXSBUF;
#endif /* TRACE_HISTORY */

MSGDEB(2, (LOG_FACILITY, "Buffer initialized."));

semclear:
return shmadr;
#else /* _WIN32 stub */
return NULL;
#endif /* !_WIN32 */
}

struct smem *lockres(void)
{
#ifndef _WIN32
struct sembuf a;
key_t memipckey, semipckey;
int shmid, semid;
struct smem *shmadr;

/* Generate IPC key for the requested resource */
semipckey=ftok(IPCFILE, PPK_SEM_SET_ID);
if (semipckey==-1) {
MSGDEB(0, (LOG_FACILITY, "ftok() : %s", strerror(errno)));
  return NULL;
  }

semid=semget(semipckey, 1, SEM_R | SEM_A);
if (semid==-1) {
  if (errno!=ENOENT)
MSGDEB(0, (LOG_FACILITY, "lockres() semget() : %s", strerror(errno)));
  return NULL;		/* Need init */
  }
/* Lock resource */
a.sem_flg=SEM_UNDO;	/* restore semaphore on process exit */
a.sem_num=0;
a.sem_op=-1; 
if (semop(semid, &a, 1)==-1) {
MSGDEB(0,(LOG_FACILITY, "semop() : %s", strerror(errno)));
  return NULL;
  }
MSGDEB(2, (LOG_FACILITY, "Resource locked."));

/* Generate IPC key for the shared memory segment */
memipckey=ftok(IPCFILE, SHARED_MEMORY_ID);
if (memipckey==-1) {
MSGDEB(0, (LOG_FACILITY, "ftok() : %s", strerror(errno)));
  return NULL;
  }

shmid=shmget(memipckey, sizeof(struct smem), SHM_R | SHM_W );
if (shmid==-1) {
MSGDEB(0, (LOG_FACILITY, "shmget() : %s", strerror(errno)));
  return NULL;
  }

/* Attach the shared memory segment */
shmadr = (struct smem *) shmat(shmid, 0, 0);
if ((int)shmadr==-1) {
MSGDEB(0, (LOG_FACILITY, "shmat() : %s", strerror(errno)));
  return NULL;
  }
return shmadr;
#else /* _WIN32 stub */
return NULL;
#endif /* !_WIN32 */
}

int unlockres(struct smem *shmadr)
{
#ifndef _WIN32
//struct sembuf mysembuf;
key_t semipckey;
int semid;

/* Detach the shared memory segment */
if (shmadr!=NULL)
  if (shmdt(shmadr)==-1) {
MSGDEB(0,(LOG_FACILITY, "shmdt() : %s", strerror(errno)));
    }
/* First, get the semaphore for the specified resource */
semipckey=ftok(IPCFILE, PPK_SEM_SET_ID);
if (semipckey==-1) {
MSGDEB(0, (LOG_FACILITY, "ftok() : %s", strerror(errno)));
  return -1;
  }

semid=semget(semipckey, 1, SEM_R | SEM_A );
if (semid==-1) {
MSGDEB(0, (LOG_FACILITY, "semget() : %s", strerror(errno)));
  return -1;
  }

/* Release resourse */
/* Explicitly
 * mysembuf.sem_flg=0;
 * mysembuf.sem_num=0;
 * mysembuf.sem_op=1;
 * if (semop(semid, &mysembuf, 1)==-1) {
 * MSGDEB(0, (LOG_FACILITY, "semop() : %s", strerror(errno)));
 */
if (semctl(semid, 0, SETVAL, 1)==-1) {
MSGDEB(0, (LOG_FACILITY, "semop() : %s", strerror(errno)));
  return -1;
  }
MSGDEB(2, (LOG_FACILITY, "Resource released."));
return 0;
#else /* _WIN32 stub! */
return -1;
#endif /* !_WIN32 */
}

/*
 * Below follows some kind of buffer stuff.
 * I tried to find a compromise between speed and size here.
 */

/*
 * Integer buffers
 */
struct elem *addtointbuf(struct elem *buf, int station, int session, unsigned short type)
{
int i;

#ifdef TRACE_HISTORY

#endif /* TRACE_HISTORY */

for (i=0; i < MAXBUF; i++)
  if (!(buf[i].type & TYPE_USED))
    break;
if (i==MAXBUF)
  return NULL;

buf[i].type = type;
buf[i].time = time(NULL);
buf[i].station = station;
buf[i].session = session;
return buf+i;
}

struct elem *searchintbuf(struct elem *buf, int station, int session, unsigned short type)
{
int i;

for (i=0; i < MAXBUF; i++)
 if (buf[i].type==type)
   if (buf[i].station==station)
     if (buf[i].session==session)
       break;

if (i==MAXBUF)
  return NULL;

return buf+i;
}

int freeintbufelem(struct elem *buf, int station, int session, unsigned short type)
{
struct elem *p;

if ((p=searchintbuf(buf, station, session, type))==NULL)
  return -1;

p->type^=TYPE_USED;
return 0;
}

/*
 * Specific to counters
 */
struct elem *searchstationcounter(struct elem *t, int station)
{
int i;

for (i=0; i < MAXBUF; i++)
  if (t[i].type==TYPE_CNTR)
    if (t[i].station==station)
      return t+i;
return NULL;
}


/*
 * Locks and signals
 *
 */

struct selem *addtostrbuf(struct selem *buf, int station, int session, char *str, unsigned short type)
{
int i;

for (i=0; i < MAXSBUF; i++)
  if (!(buf[i].type & TYPE_USED))
    break;
if (i==MAXSBUF)
  return NULL;

buf[i].type = type;
buf[i].time = time(NULL);
buf[i].station = station;
buf[i].session = session;
buf[i].typestr[0] = '\0';
strcpy(buf[i].typestr, str);
 
return buf+i;
}

int freestrbufelem(struct selem *buf, int station, int session, char *str, unsigned short type)
{
struct selem *p;

if ((p=searchstrbuf(buf, station, session, str, type))==NULL)
  return -1;

p->type^=TYPE_USED;
return 0;
}


struct selem *searchstrbuf(struct selem *buf, int station, int session, char *str, unsigned short type)
{
int i;

for (i=0; i < MAXSBUF; i++)
 if (buf[i].type==type)
   if (buf[i].station==station)
     if (buf[i].session==session)
       if (strcasecmp(buf[i].typestr, str)==0)
         break;

if (i==MAXSBUF)
  return NULL;

return buf+i;
}

/*
 * Unguarded version of compteconnexions() to prevent self-locking.
 */
int accountconnections(struct elem *t, int station)
{
int i, c=0;

for (i=0; i < MAXBUF; i++)
  if (t[i].type==TYPE_CONN) {
    if (station > 0)
      if (t[i].station==station)
        c++;
    if (station==0) /* Clients only */
      if (t[i].station!=0)
        c++;
    if (station==-1)
      c++;
    }
return c;
}

/*
 * Debugging stuff
 */

char *formattime(time_t time, char *dest)
{
struct tm t;

gmtime_r(&time, &t);
sprintf(dest, "%d h %d m %d s", t.tm_hour, t.tm_min, t.tm_sec);
return dest;
}

int OutputElementDebugInfo(FILE *o, union sdump *e, char *str)
{
char tmp[16];
char compstr[]="Compteur";
char connstr[]="Connexion";
char *ptr;
FILE *out;

if ((e==NULL) || (!(e->i.type & TYPE_USED)))
  return 0;

if (o==NULL) 
  out=fopen(IBHome"/log/C__SUIVI.TXT","a+");
else
 out=o;  

switch(e->i.type) {
  case TYPE_LOCK:
  case TYPE_SIGN:
	ptr=e->s.typestr;
	break;
  case TYPE_CONN:
	ptr=connstr;
	break;
  case TYPE_CNTR:
	ptr=compstr;
  default:
	ptr=NULL;
	break;
}

fprintf(out, "%s%d - %d : %s %s\n", str, e->i.station, e->i.session, formattime(e->i.time, tmp), ptr);

if (o==NULL)
  fclose(out);

return 0;
}

void dumpbuf(struct smem *sm, FILE *o, unsigned short type)
{
int i;

if ( (type==TYPE_LOCK) || (type==TYPE_SIGN)) {
  for(i=0; i < MAXSBUF; i++)
    if (sm->selems[i].type==type)
      OutputElementDebugInfo(o, (union sdump *) (sm->selems+i), NULL);
  return;
  }
if ( (type==TYPE_CONN) ) {
  for(i=0; i < MAXBUF; i++)
   if (sm->elems[i].type==type)
     OutputElementDebugInfo(o, (union sdump *)(sm->elems+i), NULL);
 }
}
