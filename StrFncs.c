/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   StrFncs.c
 *     String routines:
 *	 ascii - 
 *       Character - return the character based on the passed integer code.
 *       CRLF - return the carriage-return string
 *       EncryptMD5 - hash the passed string using the md5 algorithm
 *       FindWord - return the word in a string starting at some index
 *       FindWordIndex - find the index of a word starting at some index
 *       Left - return the leftmost <n> characters of a string
 *       LineWrap - based on lines <n> characters long, return the ith line
 *       lTrim - trim whitespace from the left side of the string
 *       lrTrim - trim whitespace from both sides of the string
 *       Mid - return a substring of a string
 *       PadLeft - pad the left side of a string with some other string
 *       PadRight - pad the right side of a string with some other string
 *       ProperCase - proper case the passed string (every word capitalized)
 *       Replace - replace character <a> with character <b> in the string
 *       Right - return the rightmost <n> characters of a string
 *       rTrim - Trim the right side of a string of whitespace
 *       StringLength - How long is this string?
 *       StripString - Strip a string of unwanted characters
 *       Substr - Find a substring within a string
 *       GenerateSndxIndex - return the soundex of a string
 *	 FindNthWord -
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "udf_glob.h"

#ifdef HAVE_LIBMD
#include <sys/types.h>
#include <md5.h>
#else
#include "md5.h"
#endif

#include "StrFncs.h"
#define isspacer(s) isspace(s) //|| (s=='\r'))
 
#define iswordchar(sz, i, start)       \
          (isalnum(sz[i]) ||           \
	   ((i > start) &&             \
	    !isspacer(sz[i-1]) &&      \
	    (sz[i] == '\'')))          \

static int FindChar PROTO_LIST ((char, char *));

int ascii(char *c)
{
STARTDEB("ascii")
return (int) *c;
}

char * Character(int *c)
{
char *sz;
STARTDEB("Character")
  sz = (char *) MALLOC (2);
  sz[0] = (char) *c;
  sz[1] = '\0';
ENDDEB("Character");  
  return sz;
}

char * CRLF(void)
{
char *sz;
STARTDEB("CRLF")
  sz = (char *) MALLOC(3);
  sz[0] = '\r';
  sz[1] = '\n';
  sz[2] = '\0';
ENDDEB("CRLF")
  return sz;
}

unsigned char * EncryptMD5(unsigned char *sz)
{
  MD5_CTX context;
  unsigned char digest[16];
  unsigned int len = strlen (sz);
  unsigned int i;
  unsigned char * sz_return = (unsigned char *) MALLOC (33);

STARTDEB("EncryptMD5")

  sz_return[32] = '\0';

  /* Compute the hash */
  MD5Init (&context);
  MD5Update (&context, sz, len);
  MD5Final (digest, &context);

  for (i = 0; i < 16; i++)
    sprintf (&sz_return[i*2], "%02x", digest[i]);
ENDDEB("EncryptMD5")
  return sz_return;
}

int findword(char *sz_res, char *sz, unsigned int *index)
{
  unsigned int i, j;
  int r;
STARTDEB("findword")  
SETLOCALE
  sz_res[0]='\0';
  r = FindWordIndex(sz, index);
  if (r!=-1) {
    j = (unsigned int) r;
    for (i=j; (i < strlen(sz)) && iswordchar(sz, i, *index); i++);

    strlcpy(sz_res, sz+j, i-j+1);
    } else
    i = r;
ENDDEB("findword")
  return i;
}

char *FindWord(char *sz, unsigned int *index)
{
char *sz_res;
STARTDEB("FindWord")
SETLOCALE
sz_res=(char *)MALLOC(strlen(sz));
findword(sz_res, sz, index);
ENDDEB("FindWord")
return sz_res;
}

int FindWordIndex(char *sz, unsigned int *index)
{
  unsigned int i;
  unsigned int len = strlen(sz);
STARTDEB("FindWordIndex")
SETLOCALE 
  for (i = *index; i < len && !iswordchar(sz, i, *index); i++); 
ENDDEB("FindWordIndex")
  return (i >= len) ? -1 : i;
}

char * Left(char *sz, unsigned int *count)
{
  unsigned int len;
  char *sz_result;
STARTDEB("Left")
SETLOCALE
  len = (strlen(sz) < *count) ? strlen(sz) : *count;
  sz_result = (char *) MALLOC (len + 1);
  sz_result[0] = '\0';
  if (*count!=0)
    strlcpy(sz_result, sz, len+1);
ENDDEB("Left")
  return sz_result;
}

/*
 * line_wrap algorithm
 * 1. Get the length of the string.
 * 2. If "the rest" of the string is smaller than col_width, just return
 *    it.
 * 3. Otherwise, let 'j' be the end of the column, and decrement j to
 *    a word boundary.
 *    a. Find a word boundary.
 *    b. If j = i (meaning the word is too long), then the word has
 *       has to be chopped.
 */
char * LineWrap(char *sz, int *index, int *col_width)
{
  int i, j, len;
  char * sz_result;
STARTDEB("LineWrap")
SETLOCALE
  len = strlen(sz); /* (1) */
  i =  *index;
  
  if ((len - i) <= (* col_width)) /* (2) */
    j = len - i;
  else {
    j = i + (* col_width) - 1;
    while (j > i && !isspacer(sz[j]))
      j--; /* (2a) */
    if (j == i)
      j = (* col_width);
    else
      j -= i + 1;
  }

  sz_result = (char *) MALLOC(j + 1);
  sz_result[0]='\0';
    strlcpy(sz_result, sz + i, j+2);
ENDDEB("LineWrap")
  return sz_result;
}

/* Wrapper for ltrim */
char * lTrim(char *sz)
{
  unsigned int i;
  char *sz_result;
STARTDEB("lTrim")
SETLOCALE
  sz_result = (char *) MALLOC(strlen(sz) + 1);
  
  for (i = 0 ; 
       (i < strlen(sz)) && isspacer(sz[i]) ;
       i++) ;
  strcpy(sz_result, sz+i);

ENDDEB("lTrim")
  return sz_result;
}

void lrtrim(char *sz, char *c)
{
  int i, j, len;
STARTDEB("lrtrim")
SETLOCALE
  c[0]='\0';
  len = strlen(sz);
  if (len > 0) {
    for (i = 0 ; 
         (i < len) && isspacer(sz[i]) ;
         i++) ;

    for (j = len - 1;
         (j >= 0) && isspacer(sz[j]) ;
         j--) ;
  
    strlcpy(c, sz+i, j-i+2);
  }
MSGDEB(2, (LOG_FACILITY, "%s, %s", sz, c));
ENDDEB("lrtrim")
}

char * lrTrim(char *sz)
{
char *sz_result;

STARTDEB("LrTrim")
  sz_result = (char *) MALLOC (LOC_STRING_SIZE);
  lrtrim(sz, sz_result); 
ENDDEB("LrTrim")
  return sz_result;
}

char * Mid(char *sz, int *index, int *count)
{
char *sz_result;
STARTDEB("Mid")
SETLOCALE
  sz_result = (char *) MALLOC (*count + 1);
  sz_result[0]='\0';
  strlcpy(sz_result, sz+*index, *count+1);
ENDDEB("MID")
  return sz_result;
}

char * PadLeft(char *sz, char *sz_pad, int *size)
{
  int i, j;
  int len_sz, len_sz_pad, len_sz_result;
  char *sz_result;
STARTDEB("PadLeft")
SETLOCALE
  len_sz = strlen(sz);
  len_sz_pad = strlen(sz_pad);
  len_sz_result = *size > len_sz ? *size : len_sz;
  sz_result = (char *) MALLOC (len_sz_result + 1);
  
  sz_result[len_sz_result] = '\0';
  
  for (i = len_sz - 1 ; i >= 0 ; i--)
    sz_result[len_sz_result - (len_sz - i - 1) - 1] = sz[i];

  for (i = len_sz_result - (len_sz - i - 1) - 1 ;
       i >= 0 ;
       ) {
    for (j = len_sz_pad - 1 ;
	 (j >= 0) && (i >= 0) ;
	 j--, i--)
      sz_result[i] = sz_pad[j];
  }
ENDDEB("PadLeft")  
  return sz_result;
}

char * PadRight(char *sz, char *sz_pad, int *size)
{
  int i, j;
  int len_sz, len_sz_pad, len_sz_result;
  char *sz_result;
STARTDEB("PadRight")
SETLOCALE
  len_sz = strlen(sz);
  len_sz_pad = strlen(sz_pad);
  len_sz_result = *size > len_sz ? *size : len_sz;
  sz_result = (char *) MALLOC (len_sz_result + 1);
  
  sz_result[len_sz_result] = '\0';
  
  for (i = 0 ; i < len_sz ; i++)
    sz_result[i] = sz[i];

  while (i < len_sz_result)
    for (j = 0 ;
	 (j < len_sz_pad) && (i < len_sz_result) ;
	 j++, i++)
      sz_result[i] = sz_pad[j];
ENDDEB("PadRight")
  return sz_result;
}

char * ProperCase(char *sz)
{
  unsigned int i, len, word_begin;
  char *sz_result;
STARTDEB("ProperCase")
SETLOCALE
  len = strlen(sz);
  sz_result = (char *) MALLOC (LOC_STRING_SIZE);
  word_begin = 1;

  for (i = 0 ; i < len ; i++)
    if (isalnum(sz[i]) && word_begin) {
      word_begin = 0;
      sz_result[i] = (char)toupper(sz[i]);
    }
    else if (!isalnum(sz[i])) {
      word_begin = 1;
      sz_result[i] = sz[i];
    } else
      sz_result[i] = sz[i];
ENDDEB("ProperCase")
  return sz_result;
}

char * Replace(char *sz, char *sz_replace, char *sz_with)
{
  char *sz_result;
STARTDEB("Replace")

  sz_result = (char *) MALLOC (LOC_STRING_SIZE);
  internal_replace(sz, sz_replace, sz_with, sz_result);

ENDDEB("Replace")
  return sz_result;
}

/*
 * replace -
 *   I'm sure there is a *much* more efficient way to do this, but
 *   I don't really care that much because this should be fast enough.
 *   If anybody cares to provide me with some faster/more efficient code,
 *   I'd be glad to include it here.
 */
void internal_replace(char *sz, char *sz_replace, char *sz_with, char *sz_result)
{
  int i, j, k=0;
  int len_sz, len_sz_replace, len_sz_with;
  char sz_res[LOC_STRING_SIZE + 1];
STARTDEB("internal_replace")
SETLOCALE
  len_sz = strlen(sz);
  len_sz_replace = strlen(sz_replace);
  len_sz_with = strlen(sz_with);
  
  for (i = 0 ; i < len_sz ; ) {
    if (strncmp(&sz[i], sz_replace, len_sz_replace) == 0) {
      for (j = 0 ; j < len_sz_with ; j++, k++)
	sz_res[k] = sz_with[j];
      i += len_sz_replace;
    }
    else {
      sz_res[k++] = sz[i];
      i++;
    }
  }
  sz_res[k] = '\0';
  strcpy(sz_result, sz_res);
ENDDEB("internal_replace")
}

char * Right(char *sz, int *count)
{
  int len = strlen(sz);
  char * sz_result = (char *) MALLOC (*count + 1);
STARTDEB("Right")
SETLOCALE
  sz_result[0]='\0';
  strlcpy(sz_result, sz+((*count > len) ? 0 : len - *count), *count+2);
ENDDEB("Right")
  return sz_result;
}

char * rTrim(char *sz)
{
  char * sz_result = (char *) MALLOC (strlen(sz) + 1);
STARTDEB("rTrim")
  rtrim(sz, sz_result); 
ENDDEB("rTrim")
  return sz_result;
}

int rtrim(char *sz, char *sz_result)
{
  int i;
  int len = strlen(sz);
STARTDEB("rtrim")
SETLOCALE
  for (i = len - 1 ;
       (i >= 0) && isspacer(sz[i]) ;
       i--) ;
  sz_result[0]='\0';
  strlcpy(sz_result, sz, i+2);
ENDDEB("rtrim")
  return i;
}

char * GenerateSndxIndex(char *sz)
{
  int i, j;
  char code;
  char soundex_tbl[]="01230120022455012623010202";
  /*                  ABCDEFGHIJKLMNOPQRSTUVWXYZ */
  int len = strlen(sz);
  char * sz_result = (char *) MALLOC (6);
STARTDEB("GenerateSndxIndex")
SETLOCALE
  strcpy(sz_result," 0000");

  if (sz != NULL) {
    *sz_result = (char) toupper(*sz);

    j = 1;
    for (i=1; i < len && j < 5; i++) {
      if (!isalpha(code=(char)toupper(sz[i]))) continue; /* Skip this char */
      code=soundex_tbl[code-'A'];
      if (code != '0' && code != sz_result[j - 1])
	sz_result[j++] = code;
    }
  }
ENDDEB("GenerateSndxIndex")
  return sz_result;
}

int StringLength (char *sz)
{
STARTDEB("StringLength")
SETLOCALE
    return strlen(sz);
}

char * StripString (char *sz, char *sz_strip_chars)
{
  int i;
  int j = 0;
  int len = strlen(sz);
  char * sz_result = (char *) MALLOC (len + 1);
STARTDEB("StripString")
SETLOCALE
  for (i = 0 ; i < len ; i++)
    if (!FindChar(sz[i], sz_strip_chars)) {
      sz_result[j] = sz[i];
      j++;
    }
  sz_result[j] = '\0';
ENDDEB("StripString")
  return sz_result;
}
     
/*
 * SubStr -
 *   This is most certainly *not* the most efficient way to search for
 *   a string, but I don't really care.
 */
int SubStr(char *sz, char *sz_search)
{
  int i;
  int result = -1;
  int len_sz = strlen(sz);
  int len_sz_search = strlen(sz_search);
STARTDEB("SubStr")
SETLOCALE
  for (i = 0 ; 
       (i < len_sz - len_sz_search + 1) && (result == -1) ;
       i++)
    if (strncmp(&sz[i], sz_search, len_sz_search) == 0)
      result = i;
ENDDEB("SubStr")  
  return result;
}
     
static int FindChar(char c, char *sz)
{
  int i = 0;
  int len = strlen(sz);
STARTDEB("FindChar")
SETLOCALE
  for (i = 0 ; 
       (i < len) && (c != sz[i]) ;
       i++) ;
ENDDEB("FindChar")  
  if (i == len)
    return 0;
  else
    return -1;
}

unsigned char * FindNthWord(unsigned char *sz, unsigned int *i)
{
  unsigned int k, j, Len;
  unsigned char *res=NULL;
//FIXME
STARTDEB("FindNthWord")
SETLOCALE
  Len = strlen(sz);
  res = (unsigned char *) MALLOC(Len+1);
  res[0]='\0';
#if 0 
  j = 0;
  for (k=0; k < *i; k++) { 
      j=findword(res, sz, &j);
      j++;
      if (j>Len) break;
      }
#endif
ENDDEB("FindNthWord")
  return res; 
}

void strupper(char *s, char *b)
{
unsigned int i;

STARTDEB("strupper")
SETLOCALE

for (i=0; i < strlen(s); i++)
  b[i]=(char) toupper(s[i]);

b[i]='\0';
ENDDEB("strupper")
}

void strlower(char *s, char *b)
{
unsigned int i;

STARTDEB("strlower")
SETLOCALE

for (i=0; i < strlen(s); i++)
  b[i]=(char) tolower(s[i]);

b[i]='\0';
ENDDEB("strlower")
}
