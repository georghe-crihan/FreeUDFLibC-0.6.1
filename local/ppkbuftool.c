#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef BSD
#include <machine/param.h>
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <err.h>
#include <unistd.h>
#include "ppk_buf.h"

#ifdef linux
#include <sys/stat.h>
#define SEM_A S_IWGRP 666
#define SEM_R S_IRGRP
#endif

#define FFORCE  0x00000001
#define FREMOVE 0x00000002
#define FSILENT 0x00000004
#define FMORTEM 0x00000008
#define FBDUMP  0x0000000F
#define FPUTIN  0x00000020

void printfreecells(int *);
void printtype(union sdump *);
void analyse(struct smem *);
int  ipcresstat(int, int *, int *);

int main(int argc, char **argv)
{
FILE *out=NULL;
struct smem *t;
struct sembuf sbuf;
int flags=0, i;
int semid, shmid;

while ((i = getopt(argc, argv, "rfsb:i:p:")) != -1)
     switch (i) {
     case 'f':
	     flags |= FFORCE;
	     break;
     case 'b':
	     if ((out = fopen(optarg, "w+"))==NULL)
		     err(1, "%s\n", optarg);
	     flags |= FBDUMP;
	     break;
     case 'p':
             if ((out = fopen(optarg, "r"))==NULL)
                     err(1, "%s\n", optarg);
	     flags |= FPUTIN;
     case 'i':
             if ((out = fopen(optarg, "r"))==NULL)
		      err(1, "%s\n", optarg);
	     flags |= FMORTEM;
	     break;
     case 'r':
	     flags |= FREMOVE;
	     break;
     case 's':
	     flags |= FSILENT;
	     break;
     case '?':
     default:
	     printf("%s -frs -b outfile -i infile -p datafile\n",argv[0]);
	     return -1;
     }
argc -= optind;
argv += optind;

if (!(flags & FMORTEM)) 
  if (ipcresstat(flags & FSILENT, &semid, &shmid)==-1)
    return -1;

if (!(flags & FMORTEM)) {
  if (!(flags & FFORCE)) {
    if ((t=lockres())==NULL) {
      err(1,"lockres()\n");
      return -1;
      }
    }
    else {
      sbuf.sem_num=0;
      sbuf.sem_op=-1;
      sbuf.sem_flg=SEM_UNDO | IPC_NOWAIT;
      i=semop(semid, &sbuf, 1);
      if (i==-1)
        if (errno==EAGAIN)
          puts("Warning: working on a locked segment!");
      t = (struct smem *) shmat (shmid, 0, 0);
    }
}

if (!(flags & FMORTEM) && (out!=NULL) && (flags & FBDUMP)) 
  fwrite(t, 1, sizeof(struct smem), out);

if ((flags & FMORTEM) && !(flags & FPUTIN))
  t=(struct smem *) malloc(sizeof(struct smem));

if ((flags & FMORTEM) || (flags & FPUTIN)) 
  fread(t, 1, sizeof(struct smem), out);

if (!(flags & FSILENT))
  analyse(t);

if ((flags & FMORTEM) && !(flags & FPUTIN))
  free(t);

if (flags & (FMORTEM | FPUTIN | FBDUMP))
  fclose(out);

if (!(flags & FMORTEM))
  unlockres(t);

if (!(flags & FMORTEM) && (flags & FREMOVE)) {
  shmctl(shmid, IPC_RMID, NULL);
  semctl(semid, 0, IPC_RMID, NULL);
  }

return 0;
}

void printfreecells(int *t)
{
if (*t!=0) {
  printf("%d free elements\n", *t);
  *t=0;
  }
}

void printtype(union sdump *t)
{
unsigned short i=t->i.type;

if (! (i & TYPE_USED) ) {
   printf("erased ");
   i^=TYPE_USED; 
   }

switch (i) {
  case TYPE_LOCK:
	printf("lock: %s station %d, session %d, lock %s\n", ctime(&(t->s.time)), t->s.station, t->s.session, t->s.typestr);
	break;
  case TYPE_SIGN:
	printf("signal: %s station %d, session %d, signal %s\n", ctime(&(t->s.time)), t->s.station, t->s.session, t->s.typestr);
	break;
  case TYPE_CONN:
	printf("connection: %s station %d, session %d\n", ctime(&(t->i.time)), t->i.station, t->i.session);
	break;
  case TYPE_CNTR:
	printf("counter: %s station %d, value %d\n", ctime(&(t->i.time)), t->i.station, t->i.session);
	break;
  default:
	printf("unknown(0x%X): %s station %d, session %d\n", t->i.type, ctime(&(t->i.time)), t->i.station, t->i.session);
    }
}

/* Generate report */
void analyse(struct smem *t)
{
int i, freecells;

printf("Magic 0x%X, expected 0x%X\n", t->magic, PPK_MAGIC);
if (t->magic!=PPK_MAGIC)
  puts("Warning: magics doesn't match!");

printf("Flags: 0x%X\t", t->flags);
if (t->flags & FPPKDEBUG)
  puts("(DEBUG)");
else
  puts("");

printf("Reserved1: 0x%X\n", t->res1);
printf("Reserved2: 0x%X\n", t->res2);

printf("Buffer size: %d bytes\n", sizeof(struct smem));
printf("Number of allocated records: full=%d, light=%d\n", MAXSBUF, MAXBUF);

puts("Dumping full buffer");

freecells=0;

for (i=0; i < MAXSBUF; i++) {
  if (t->selems[i].type==TYPE_FREE)
    freecells++;
  else {
     printfreecells(&freecells);
     printtype((union sdump *) (t->selems+i)); 
  }
}
printfreecells(&freecells); 

puts("\nDumping light buffer");
for (i=0; i < MAXBUF; i++) {
  if (t->elems[i].type==TYPE_FREE)
    freecells++;
  else {
    printfreecells(&freecells);
    printtype((union sdump *) (t->elems+i));
   }
}
printfreecells(&freecells);
}


int ipcresstat(int silent, int *semid, int *shmid)
{
struct semid_ds ipcstat;
struct shmid_ds mstat;
key_t key;
int i;

key=ftok(IPCFILE, PPK_SEM_SET_ID);
if (key==-1)
  return -1;

*semid=semget(key, 1, SEM_A | SEM_R);
if (*semid==-1)
  return -1;

i = semctl(*semid, 0, IPC_STAT, &ipcstat);
if (i==-1)
  return -1;

if (!silent) {
  printf("Number of semaphores: %d\n", ipcstat.sem_nsems);
  printf("Last semaphore operation: %s", ctime(&ipcstat.sem_otime));
  printf("Last semaphore change: %s", ctime(&ipcstat.sem_ctime));
  }

key=ftok(IPCFILE, SHARED_MEMORY_ID);
if (key==-1)
  return -1;

*shmid=shmget(key, sizeof(struct smem), SHM_R | SHM_W); 
if (*shmid==-1)
  return -1;

shmctl(*shmid, IPC_STAT, &mstat);  
if (!silent) {
  printf("Shared memory size: %d bytes\n", mstat.shm_segsz);
  printf("Last operation's PID: %d\n", mstat.shm_lpid);
  printf("Creator's PID: %d\n", mstat.shm_cpid);
  printf("Current attachments: %d\n", mstat.shm_nattch);
  printf("Last attachment: %s", ctime(&mstat.shm_atime));
  printf("Last detachment: %s", ctime(&mstat.shm_dtime));
  printf("Last change: %s", ctime(&mstat.shm_ctime));
  }
return 0;
}
