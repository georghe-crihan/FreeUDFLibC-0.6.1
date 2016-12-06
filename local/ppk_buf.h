#ifndef PPK_BUF_H_INCLUDED
#define PPK_BUF_H_INCLUDED

#include <time.h>
#include <sys/types.h>
#include "../udf_glob.h"
#include "../Misc.h"

#define CONNECTION_LIMIT 100	/* Originaly 99 */

#define MAXBUF CONNECTION_LIMIT 
#define MAXSBUF CONNECTION_LIMIT
 
#define PPK_MAGIC 0x50704B00		/* "PpK\0" */

/* Field types */
#define TYPE_FREE 0x0000
#define TYPE_USED 0x8000 
#define TYPE_LOCK 0x8001
#define TYPE_SIGN 0x8002
#define TYPE_CONN 0x8003
#define TYPE_CNTR 0x8004

/* Flags */
#define FPPKDEBUG 0x0001

/* IPC identification */
#define IPCFILE IBHome"/ibdon.gdb"
#define SHARED_MEMORY_ID 0x70506B4D	/* pPkM */
#define PPK_SEM_SET_ID   0x70506B53	/* pPkS */

struct elem
{
unsigned short type;
int station;
int session;
time_t time;
};

struct selem 
{
unsigned short type;
int station;
int session;
time_t time;
char typestr[LOC_STRING_SIZE];
};

/* For dumping purposes */
union sdump
{
struct elem i;
struct selem s;
};

struct smem
{
int magic;
unsigned short flags;
unsigned char res1;
unsigned char res2;
struct elem elems[MAXBUF];
struct selem selems[MAXSBUF]; 
};

#ifdef __cplusplus
 extern "C"
 {
#endif
/* The shared memory mechanism */
struct smem *trylockres(void);
struct smem *lockres(void);
int unlockres(struct smem *);

int accountconnections(struct elem *, int);
struct elem *addtointbuf(struct elem *, int, int, unsigned short);
struct elem *searchintbuf(struct elem *, int, int, unsigned short);
int freeintbufelem(struct elem *, int, int, unsigned short);
struct elem *searchstationcounter(struct elem *, int);
struct selem *searchstrbuf(struct selem *, int, int, char *, unsigned short);
int searchlockstring(char *);
int freestrbufelem(struct selem *, int, int, char *, unsigned short);
struct selem *addtostrbuf(struct selem *, int, int, char *, unsigned short);
int OutputElementDebugInfo(FILE *, union sdump *, char *);
void dumpbuf(struct smem *, FILE *, unsigned short);
#ifdef __cplusplus
 }
#endif
#endif /* PPK_BUF_H_INCLUDED */
