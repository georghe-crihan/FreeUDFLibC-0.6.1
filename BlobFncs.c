/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   BlobFncs.c
 *     Blob routines declarations:
 *	 BlobMaxSegmentLength -
 *	 BlobSegmentCount -
 *	 BlobSize -
 *	 BlobAsPChar -
 *	 BlobLeft -
 *	 BlobLine -
 *	 BlobMid -
 *	 BlobRight -
 *	 StrBlob -
 *	 BlobBinCmp -
 *	 BlobUpper -
 *	 BlobReplace -
 *	 BlobCat -
 *	 BlobCatStr -
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "udf_glob.h"
#include "StrFncs.h"
#include "BlobFncs.h"

int BlobMaxSegmentLength(BLOBCALLBACK Blob)
{
  STARTDEB("BlobMaxSegmentLength")
  if ((Blob==NULL) || (Blob->blob_handle==NULL)) return 0;
  ENDDEB("BlobMaxSegmentLength")
  return Blob->blob_max_segment;
}

int BlobSegmentCount(BLOBCALLBACK Blob)
{
  STARTDEB("BlobSegmentCount")
  if ((Blob==NULL) || (Blob->blob_handle==NULL)) return 0;
  ENDDEB("BlobSegmentCount")
  return Blob->blob_number_segments;
}

int BlobSize(BLOBCALLBACK Blob)
{
  STARTDEB("BlobSize")
  if ((Blob==NULL) || (Blob->blob_handle==NULL)) return 0;
  ENDDEB("BlobSize")
  return Blob->blob_total_length;
}

unsigned char * BlobAsPChar(BLOBCALLBACK Blob)
{
unsigned char *st=NULL;

  STARTDEB("BlobAsPChar")
SETLOCALE

  st = (unsigned char *) MALLOC(BLOB_STRING_SIZE);
  internal_blob2str(Blob, st, BLOB_STRING_SIZE);
  ENDDEB("BlobAsPChar")
  return st;
}

void internal_blob2str(BLOBCALLBACK Blob, unsigned char *st, ISC_USHORT st_size)
{
  ISC_USHORT bytes_read=0;
  ISC_LONG bytes_left, total_bytes_read;

STARTDEB("internal_blob2str")
  if ((Blob==NULL) || (Blob->blob_handle==NULL)) {
    st[0] = '\0';
    return;
    }

  total_bytes_read = 0;                   /* total bytes read is 0. */
  for (bytes_left=Blob->blob_total_length; /* I have TotalSize bytes to read. */
        bytes_left > 0; bytes_left-=bytes_read) { /* Decrement bytes_left by the number of bytes actually read. */
    /* Using blob_handle, store at most "bytes_left" bytes in
     * the buffer starting where I last left off
     */
    (*Blob->blob_get_segment)(Blob->blob_handle, st+total_bytes_read,
						st_size, &bytes_read);
    st_size-=bytes_read;
MSGDEB(2, (LOG_FACILITY, "internal_blob2str: %d %d\n", bytes_left, bytes_read));
    /* Increment total_bytes_read by the number of bytes actually read. */
    total_bytes_read+=bytes_read;
  }
st[total_bytes_read]='\0';
ENDDEB("internal_blob2str")
}

unsigned char *BlobLeft(BLOBCALLBACK Blob, unsigned int *Number)
{
  ISC_USHORT bytes_read=0, st_size=BLOB_STRING_SIZE;
  ISC_LONG bytes_left, total_bytes_read;
  unsigned char *st;
  STARTDEB("BlobLeft")
SETLOCALE
  st = (unsigned char *) MALLOC (BLOB_STRING_SIZE);
  st[0] = '\0';
  if ((Blob==NULL) || (Blob->blob_handle==NULL) || (*Number < 0)) return st;
  bytes_left =
      Blob->blob_total_length < *Number ? Blob->blob_total_length : *Number;
  total_bytes_read = 0;                  /* total bytes read is 0. */
  for( ; bytes_left > 0; bytes_left-=bytes_read) { /* Decrement bytes_left by the number of bytes actually read. */
    /* Using blob_handle, store at most "bytes_left" bytes in
     * the buffer starting where I last left off
     */
    (*Blob->blob_get_segment)(Blob->blob_handle, st+total_bytes_read, st_size, &bytes_read);
    st_size-=bytes_read;
MSGDEB(2, (LOG_FACILITY, "%d", bytes_read));
    /* Increment total_bytes_read by the number of bytes actually read. */
    total_bytes_read+=bytes_read;
  }
  st[total_bytes_read]='\0';
  ENDDEB("BlobLeft")
  return st;
}

/* Lines are zero based. */
unsigned char * BlobLine(BLOBCALLBACK Blob, int *Number)
{
  int i, j;
  unsigned char *result;
 /* First read the whole blob into a character buffer, then
    scan through the entire blob, reusing the first part of the
    buffer to write the Number-th line of the Blob.
 */
  STARTDEB("BlobLine")
SETLOCALE
  result = BlobAsPChar(Blob);
  if (*result == '\0') return result;
  j = -1;
  while ((*Number >= 0) && ((j == -1) || (result[j] != '\0'))) {
    i = -1;
    do {
      i++; j++;
      /* Here only write to the return string if j > i. */
      if (j > i)
        result[i] = result[j];
    } while ( (result[j]!='\0') && (result[j]!='\n') && (result[j]!='\r') );
    /* if current character is cr then check to see if there
     * is a line feed following it.
     */
    if ((result[j] == '\n') || (result[j]=='\r')) {
      result[i] = '\0'; /* "End" the current line. */
      if (result[j+1] == '\n') j++;
    }
    (*Number)--;
  } /* while */ 
  if (*Number != -1) *result = '\0';
  ENDDEB("BlobLine")
  return result;
}

unsigned char *BlobMid(BLOBCALLBACK Blob, unsigned int *Start, unsigned int *Number)
{
  unsigned char *result;
  unsigned int t;

  STARTDEB("BlobMid")
SETLOCALE
  /* result = BlobAsPChar(Blob); */
  t = *Number+*Start;
  result = BlobLeft(Blob, &t); /* avoid reading more */
MSGDEB(2, (LOG_FACILITY, "%s", result));
  if (*Start > strlen(result)) {
    result[0] = '\0';
    return result;
    }
  strlcpy(result, result + *Start, *Number+1);
  ENDDEB("BlobMid")
  return result;
}

unsigned char *BlobRight(BLOBCALLBACK Blob, int *Number)
{
  unsigned char *result;

  STARTDEB("BlobRight")
SETLOCALE
  result = BlobAsPChar(Blob);
  if (*Number < 0) result[0]='\0';
  strlcpy(result, result + strlen(result) - *Number, *Number+1);
  ENDDEB("BlobRight")
  return result;
}

/* According to IB docs, this is how you do it for blobs.
 * First pass the "first" argument
 * Second, pass the "return" argument (A Blob) as the last argument.
 * then you would just declare the external function in
 * IB as shown below
 */
BLOBCALLBACK StrBlob(unsigned char *sz, BLOBCALLBACK Blob)
{
  STARTDEB("StrBlob")
  if ((Blob==NULL) || (Blob->blob_handle==NULL)) return Blob;
  (*Blob->blob_put_segment)(Blob->blob_handle, sz, (ISC_USHORT) strlen(sz));
  ENDDEB("StrBlob")
  return Blob;
}

BLOBCALLBACK BlobUpper(BLOBCALLBACK SrcBlob, BLOBCALLBACK DstBlob)
{
unsigned char *c;
BLOBCALLBACK b;

  SETLOCALE
  STARTDEB("BlobUpper")
  if ((SrcBlob==NULL) || (SrcBlob->blob_handle==NULL))
    return SrcBlob;
  c = (unsigned char *) malloc(BLOB_STRING_SIZE);
  internal_blob2str(SrcBlob, c, BLOB_STRING_SIZE);
  strupper(c,c);
  b = StrBlob(c, DstBlob);
  free(c);
  ENDDEB("BlobUpper")
  return b;
}

BLOBCALLBACK BlobCatStr(BLOBCALLBACK SrcBlob, unsigned char *s, BLOBCALLBACK DstBlob)
{
unsigned char *c;
BLOBCALLBACK b;

  SETLOCALE
  STARTDEB("BlobCatStr")
  if ((SrcBlob==NULL) || (SrcBlob->blob_handle==NULL))
    return SrcBlob;
  c = (unsigned char *) malloc(BLOB_STRING_SIZE);
  internal_blob2str(SrcBlob, c, BLOB_STRING_SIZE);
  strcat(c, s);
  b = StrBlob(c, DstBlob);
  free(c);
  ENDDEB("BlobCatStr")
  return b;
}

BLOBCALLBACK BlobCat(BLOBCALLBACK SrcBlob, BLOBCALLBACK SrcBlob2, BLOBCALLBACK DstBlob)
{
unsigned char *c, *d;
BLOBCALLBACK b;

  SETLOCALE
  STARTDEB("BlobCat")
  if ((SrcBlob2==NULL) || (SrcBlob2->blob_handle==NULL))
    return SrcBlob2;
  if ((SrcBlob==NULL) || (SrcBlob->blob_handle==NULL))
    return SrcBlob;
  c = (unsigned char *) malloc(BLOB_STRING_SIZE);
  d = (unsigned char *) malloc(BLOB_STRING_SIZE);
  internal_blob2str(SrcBlob, c, BLOB_STRING_SIZE);
  internal_blob2str(SrcBlob2, d, BLOB_STRING_SIZE);
  strcat(c, d);
  b = StrBlob(c, DstBlob);
  free(c);
  free(d);
  ENDDEB("BlobCat")
  return b;
}

BLOBCALLBACK BlobReplace(unsigned char *srch, unsigned char *repl, BLOBCALLBACK SrcBlob, BLOBCALLBACK DstBlob)
{
unsigned char *c, *d;
BLOBCALLBACK b;

  SETLOCALE
  STARTDEB("BlobReplace")
  if ((SrcBlob==NULL) || (SrcBlob->blob_handle==NULL))
    return SrcBlob;
  c = (unsigned char *) malloc(BLOB_STRING_SIZE);
  d = (unsigned char *) malloc(BLOB_STRING_SIZE);
  internal_blob2str(SrcBlob, c, BLOB_STRING_SIZE);
  internal_replace(c, srch, repl, d);
  b = StrBlob(d, DstBlob);
  free(c);
  free(d);
  ENDDEB("BlobReplace")
  return b;
}

int BlobBinCmp(BLOBCALLBACK BlobA, BLOBCALLBACK BlobB)
{
  ISC_USHORT bytes_readA=0;
  ISC_LONG bytes_leftA, total_bytes_readA, segmA;
  ISC_USHORT bytes_readB=0;
  ISC_LONG bytes_leftB, total_bytes_readB, segmB;
  ISC_LONG iterator;
  unsigned char stA[BLOB_STRING_SIZE], stB[BLOB_STRING_SIZE];
  int result;

  STARTDEB("BlobBinCmp")
SETLOCALE
  result = -1;
  if ((BlobA==NULL) || (BlobA->blob_handle==NULL) ||
      (BlobB==NULL) || (BlobB->blob_handle==NULL)) return -1; 
  if (BlobA->blob_total_length != BlobB->blob_total_length)
  {
 
MSGDEB(2,(LOG_FACILITY, "[ASize-BSize %d - %d]", BlobA->blob_total_length, BlobB->blob_total_length));
     ENDDEB("BlobBinCmp")
     return (BlobA->blob_total_length < BlobB->blob_total_length ? BlobA->blob_total_length: BlobB->blob_total_length) + 1;
  } 
  result = 0;
  if ((BlobA->blob_total_length == 0) && (BlobB->blob_total_length == 0))
  { 
    ENDDEB("BlobBinCmp")
    return 0;
  }
  bytes_leftA = BlobA->blob_total_length;  /* I have TotalSize bytes to read. */
  total_bytes_readA = 0;
  segmA = BlobA->blob_max_segment;

MSGDEB(2, (LOG_FACILITY, "[ASizeMaxSegmentLength %d - %d]", BlobA->blob_total_length, BlobA->blob_max_segment));

  bytes_leftB = BlobB->blob_total_length;  /* I have TotalSize bytes to read. */
  total_bytes_readB = 0;
  segmB = BlobB->blob_max_segment;

MSGDEB(2, (LOG_FACILITY, "[BSizeMaxSegmentLength %d - %d]", BlobB->blob_total_length, BlobB->blob_max_segment));

  iterator = 0;
  do {
    /* Using blob_handle, store at most "bytes_left" bytes in
     * the buffer starting where I last left off
     */
    if (iterator == total_bytes_readA)
    {
 
MSGDEB(2, (LOG_FACILITY, "[iterator=totalReadA %d]", total_bytes_readA));

      (*BlobA->blob_get_segment)(BlobA->blob_handle, stA, BLOB_STRING_SIZE, &bytes_readA);
      /* Increment total_bytes_read by the number of bytes actually read. */

MSGDEB(2, (LOG_FACILITY, "[iterator=BytesReadA %d]", bytes_readA));

      total_bytes_readA+=bytes_readA;
      /* Decrement bytes_left by the number of bytes actually read. */
      bytes_leftA-=bytes_readA;
    } 
    if (iterator == total_bytes_readB)
    {

MSGDEB(2, (LOG_FACILITY, "[iterator=totalReadB %d]", total_bytes_readB));

      (*BlobB->blob_get_segment)(BlobB->blob_handle, stB, BLOB_STRING_SIZE, &bytes_readB);
      /* Increment total_bytes_read by the number of bytes actually read. */
      total_bytes_readB+=bytes_readB;
      /* Decrement bytes_left by the number of bytes actually read. */
      bytes_leftB-=bytes_readB;
    }

MSGDEB(2, (LOG_FACILITY, "[itA,itB]=%c, %c", stA[iterator % segmA], stB[iterator % segmB]));

    if (stA[iterator % segmA] != stB[iterator % segmB])
    { 
    	result = iterator + 1;
      break;
    } 
    iterator++;

MSGDEB(2, (LOG_FACILITY, "[Iterator=%d]", iterator));

  } while ((iterator != BlobA->blob_total_length) && (iterator != BlobB->blob_total_length));
  ENDDEB("BlobBinCmp")
  return result;
}

int BlobIComp(BLOBCALLBACK BlobA, BLOBCALLBACK BlobB)
{
  ISC_USHORT bytes_readA=0;
  ISC_LONG bytes_leftA, total_bytes_readA, segmA;
  ISC_USHORT bytes_readB=0;
  ISC_LONG bytes_leftB, total_bytes_readB, segmB;
  ISC_LONG iterator;
  unsigned char stA[BLOB_STRING_SIZE], stB[BLOB_STRING_SIZE];
  int result;

  STARTDEB("BlobIComp")
SETLOCALE
  result = -1;
  if ((BlobA==NULL) || (BlobA->blob_handle==NULL) ||
      (BlobB==NULL) || (BlobB->blob_handle==NULL)) return -1; 
  if (BlobA->blob_total_length != BlobB->blob_total_length)
  {
 
MSGDEB(2, (LOG_FACILITY, "[ASize-BSize %d - %d]", BlobA->blob_total_length, BlobB->blob_total_length));

     ENDDEB("BlobIComp")
     return (BlobA->blob_total_length < BlobB->blob_total_length ? BlobA->blob_total_length: BlobB->blob_total_length) + 1;
  } 
  result = 0;
  if ((BlobA->blob_total_length == 0) && (BlobB->blob_total_length == 0))
  { 
    ENDDEB("BlobIComp")
    return 0;
  }
  bytes_leftA = BlobA->blob_total_length;  /* I have TotalSize bytes to read. */
  total_bytes_readA = 0;
  segmA = BlobA->blob_max_segment;

MSGDEB(2, (LOG_FACILITY, "[ASizeMaxSegmentLength %d - %d]", BlobA->blob_total_length, BlobA->blob_max_segment));

  bytes_leftB = BlobB->blob_total_length;  /* I have TotalSize bytes to read. */
  total_bytes_readB = 0;
  segmB = BlobB->blob_max_segment;

MSGDEB(2, (LOG_FACILITY, "[BSizeMaxSegmentLength %d - %d]", BlobB->blob_total_length, BlobB->blob_max_segment));

  iterator = 0;
  do {
    /* Using blob_handle, store at most "bytes_left" bytes in
     * the buffer starting where I last left off
     */
    if (iterator == total_bytes_readA)
    {
 
MSGDEB(2, (LOG_FACILITY, "[iterator=totalReadA %d]", total_bytes_readA));

      (*BlobA->blob_get_segment)(BlobA->blob_handle, stA, BLOB_STRING_SIZE, &bytes_readA);
      /* Increment total_bytes_read by the number of bytes actually read. */
      total_bytes_readA+=bytes_readA;
      /* Decrement bytes_left by the number of bytes actually read. */
      bytes_leftA-=bytes_readA;
    } 
    if (iterator == total_bytes_readB)
    {
MSGDEB(2, (LOG_FACILITY, "[iterator=totalReadB %d]", total_bytes_readB));

      (*BlobB->blob_get_segment)(BlobB->blob_handle, stB, BLOB_STRING_SIZE, &bytes_readB);
      /* Increment total_bytes_read by the number of bytes actually read. */
      total_bytes_readB+=bytes_readB;
      /* Decrement bytes_left by the number of bytes actually read. */
      bytes_leftB-=bytes_readB;
    }

MSGDEB(2, (LOG_FACILITY, "[itA,itB]=%c, %c", stA[iterator % segmA], stB[iterator % segmB]));

    if (tolower(stA[iterator % segmA]) != tolower(stB[iterator % segmB]))
    { 
    	result = iterator + 1;
      break;
    } 
    iterator++;

MSGDEB(2, (LOG_FACILITY, "[Iterator=%d]", iterator));

  } while ((iterator != BlobA->blob_total_length) && (iterator != BlobB->blob_total_length));
  ENDDEB("BlobIComp")
  return result;
}

unsigned char *FindNthWordBlob(BLOBCALLBACK Blob, unsigned int *n)
{
unsigned char *a, *c;

STARTDEB("FindNthWordBlob")
c = (unsigned char *) malloc (32000);
internal_blob2str(Blob, c, 32000);
a=FindNthWord(c, n);
free(c);
ENDDEB("FindNthWordBlob")
return a;
}

int DumpBlobToFile(char *msg, char *fname, BLOBCALLBACK Blob)
{
char *f;
unsigned char *t;
int res=1;
FILE *log;
time_t timestamp;

STARTDEB("DumpBlobToFile")
f =(char *) malloc(MAX_PATH_LEN);
t =(unsigned char *) malloc(BLOB_STRING_SIZE);
internal_blob2str(Blob, t, BLOB_STRING_SIZE);
strcpy(f, IBHome"/log/");
wintransform(fname, f);
if ((log=fopen(f, "a+"))==NULL)
  return -1;
if (*msg=='\0')
  res=0;
timestamp=time(NULL);
fprintf(log, "%s\n%s\n%s\n", ctime(&timestamp),msg,t);
fclose(log);
free(t);
free(f);
ENDDEB("DumpBlobToFile")
return res;
}
