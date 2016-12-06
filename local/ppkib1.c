#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dtype.h"
#include "../udf_glob.h"
#include "../StrFncs.h"

#define LOCSTR  254
#define LOCSTR2 250

extern byte *formatdatetime(byte *, byte *);

/*
 * Note: UDFs are so loosing, that they are using their own non-ISO
 * char tables, so I am forced to supply them.
 */

char vowels[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0,
1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1
};

char diacriticals[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1
};

char notvowels[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0,
0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

byte upchars[] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x43,
0x45, 0x45, 0x45, 0x45, 0x49, 0x49, 0x49, 0x49,
0xD0, 0x4E, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x58,
0xD8, 0x55, 0x55, 0x55, 0x55, 0x59, 0xDE, 0xDF,
0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x43,
0x45, 0x45, 0x45, 0x45, 0x49, 0x49, 0x49, 0x49,
0xF0, 0x4E, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0xF7,
0xF8, 0x55, 0x55, 0x55, 0x55, 0x59, 0xFE, 0x59
};

byte lowchars[] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x63,
0x65, 0x65, 0x65, 0x65, 0x69, 0x69, 0x69, 0x69,
0xD0, 0x6E, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x78,
0xD8, 0x75, 0x75, 0x75, 0x75, 0x79, 0xDE, 0xDF,
0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x63,
0x65, 0x65, 0x65, 0x65, 0x69, 0x69, 0x69, 0x69,
0x6F, 0x6E, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0xF7,
0xF8, 0x75, 0x75, 0x75, 0x75, 0x79, 0xFE, 0x79
};

void dstrupper(byte *s, byte *d)
{
unsigned int i;

for (i=0; i < strlen(s); i++)
  d[i]=dtoupper(s[i]);
d[i]='\0';
}

void dstrlower(byte *s, byte *d)
{
unsigned int i;

for (i=0; i < strlen(s); i++)
  d[i]=dtolower(s[i]);
d[i]='\0';
}

int lgnom(void)
{
return 220;
}

int lgprenom(void)
{
return 30;
}

byte *caractere(int *c)
{
byte *a;

STARTDEB("caractere")
a=(byte *) MALLOC(LOCSTR2);
a[0]=(byte) *c;
a[1]='\0';
ENDDEB("caractere")
return a;
}

byte *inverseascii(byte *c)
{
byte *a;
unsigned int i;

STARTDEB("inverseascii")
a=(byte *) MALLOC(LOCSTR2);
for(i=0; i < strlen(c); i++)
  a[i]= (byte) (0xFF - c[i]);

a[i]='\0';
ENDDEB("inverseascii")
return a;
}

byte *strzero(int *c, int *p)
{
byte t[32], *a;
int i;

t[0]='\0';
STARTDEB("strzero")
a=(byte *)MALLOC(LOCSTR2);
sprintf(t, "%d", *c);
i=*p-strlen(t);
a[i]='\0';
if (i > 0)
  for (i--; i >= 0; i--)
    a[i]='0';
strcat(a, t);
MSGDEB(2, (LOG_FACILITY, "%d, %d, %s", *c, *p, a));
ENDDEB("strzero")
return a;
}

byte *substr(byte *s, int *i, int *c)
{
byte *a;
STARTDEB("substr")
a=(byte *)MALLOC(LOCSTR);
a[0]='\0';
strlcpy(a, s+*i-1, *c+1);
MSGDEB(2, (LOG_FACILITY, "\"%s\",\n \"%d\",\n \"%d\",\n \"%s\"", s, *i, *c, a));
ENDDEB("substr")
return a;
}

byte *sommechar(byte *s1, byte *s2)
{
byte *a;

STARTDEB("sommechar")
a=(byte *)MALLOC(LOCSTR);

*a='\0';
strcpy(a, s1);
strcat(a, s2);
ENDDEB("sommechar")
return a;
}

int somme(int *a, int *b)
{
STARTDEB("somme")
return *a+*b;
}

int extraitchiffres(byte *c)
{
unsigned int i, j=0;
byte tmp[48];

STARTDEB("extraitchiffres")
SETLOCALE

for (i=0; i < strlen(c); i++)
  if (isdigit(c[i])) {
    tmp[j]=c[i];
    j++;
    }

tmp[j]='\0';
i=atoi(tmp);
ENDDEB("extraitchiffres")
if (errno!=0)
  return -1;
return i;
}

byte *maj(byte *s)
{
byte *a;
STARTDEB("maj")
a=(byte *)MALLOC(LOCSTR);
dstrupper(s, a);
ENDDEB("maj")
return a;
}

byte *minu(byte *s)
{
byte *a;
STARTDEB("minu")
a=(byte *)MALLOC(LOCSTR);
dstrlower(s, a);
ENDDEB("minu")
return a;
}

byte *extraitnom(byte *s)
{
byte *a;
unsigned int i;
STARTDEB("extraitnom")
a=(byte *)MALLOC(LOCSTR);

for (i=0; i < strlen(s); i++) {
  a[i]=s[i];
  if (s[i]==',')
    break;
  }
a[i]='\0';   
ENDDEB("extraitnom");
return a;
}

byte *padr(byte *s, int *c)
{
SETLOCALE
STARTDEB("padr")
return PadRight(s, " ", c);
}

byte *padl(byte *s, int *c)
{
SETLOCALE
STARTDEB("padr")
return PadLeft(s, " ", c);
}

int lpos(byte *s2, byte *s1)
{
byte *c;

STARTDEB("lpos")
SETLOCALE
if (*s2=='\0')
  return 0;

c=strstr(s1, s2);

if (c==NULL)
  return 0;
MSGDEB(2, (LOG_FACILITY, "%s, %s, %d", s1, s2, c - s1 + 1));
ENDDEB("lpos")
return c - s1 + 1;
}

int combien(byte *s2, byte *s1)
{
int c=0, p=0, r;
STARTDEB("combien")
for (p=0; (r=lpos(s2, s1+p))!=0; p+=r)
  c++;
ENDDEB("combien")
return c;
}

byte *makenp(byte *s1, byte *s2)
{
byte *a, *b;
STARTDEB("makenp")
MSGDEB(2, (LOG_FACILITY, "makenp: s1: %s, s2: %s", s1, s2));
a = (byte *) MALLOC (LOCSTR);
a[0]='\0';
lrtrim(s1, a);
if (s1!=NULL) {
MSGDEB(2, (LOG_FACILITY, "makenp: a: %s", a));
  if ((s2!=NULL) && (s2[0]!='\0')) {
    b=(byte *)malloc (LOCSTR);
    b[0]='\0';
    lrtrim(s2,b);
MSGDEB(2, (LOG_FACILITY, "makenp: b: %s", b));
    strcat(a, ", ");
    strcat(a, b);
    free(b);
    }
  }
MSGDEB(2, (LOG_FACILITY, "makenp: res: %s", a));
ENDDEB("makenp")
return a; 
}

byte *compresse(byte *s)
{
unsigned int i, j;
byte *a;

STARTDEB("compresse")
SETLOCALE
a=(byte *)MALLOC(LOCSTR);
j=0;
for (i=0; i < strlen(s); i++)
  if (isalnum(s[i])) {
    a[j]=s[i];
    j++;
    }
a[j]='\0';
ENDDEB("compresse")
return a;
}


byte *compmaj(byte *s)
{
byte *a;
STARTDEB("compmaj")
a=compresse(s);
dstrupper(a, a);
ENDDEB("compmaj")
return a;
}

byte *pluriel(byte *s)
{
byte *a;
int i;

STARTDEB("pluriel")
SETLOCALE
a=(byte *)MALLOC(LOCSTR);
*a='\0';

lrtrim(s, a);
i=strlen(a);
if (i == 0) return a;
if ( (i > 1) && (tolower(a[i-1])=='l') && (tolower(a[i-2])=='a') ) {
  a[i-1]='\0';
  strcat(a, "ux");
  return a;
  }
if (tolower(a[i-1])=='s') { /* trim */
  a[i-1]='\0';
  return a;
  }
if (!((tolower(a[i-1])=='x') || (tolower(a[i-1])=='z')))
  strcat(a, "s"); 
ENDDEB("pluriel")
return a;
}

static void last_delim(byte *n, byte **a)
{
byte *b=n, *c=n;
int i,l;

STARTDEB("last_delim")
l=strlen(n)-1;
for(i=l; i >= 0; i--)
  if (n[i]=='\\') {
    b=n+i;
    break;
    } else
  if (n[i]==':') {
    c=n+i;
    break;
    }
if ((b < c) && (c > n)) b=c;
*a=b;
ENDDEB("last_delim")
}

byte *extraitfilename(byte *n)
{
byte *a, *c;
STARTDEB("extraitfilename")
a = (byte *) MALLOC(LOCSTR);
*a='\0';
last_delim(n, &c);
strcpy(a, c+1);
ENDDEB("extraitfilename")
return a;
}

byte *extraitfilepath(byte *s)
{
byte *a, *c;

STARTDEB("extraitfilepath")
a = (byte *) MALLOC(LOCSTR);
*a='\0';
last_delim(s, &c);
strlcpy(a, s, c-s+2);
ENDDEB("extraitfilepath")
return a;
}

byte *majnp(byte *s)
{
int c, d=220;
byte *a, *b;

STARTDEB("majnp")
c = lpos(",", s);
if (c!=0) {
  a = (byte *) malloc(LOCSTR);
  a[0] = '\0';
  strlcpy(a, s, c - 1);
  b = padr(a, &d);
  free(a);
} else {
  b = (byte *) MALLOC(LOCSTR);
  b[0] = '\0';
  }
strcat(b, s+c);
dstrupper(b, b); 
ENDDEB("majnp")
return b;
}

byte *majespace(byte *s)
{
byte *a;
STARTDEB("majespace")
SETLOCALE
a = compresse(s);
dstrupper(a, a);
ENDDEB("majespace")
return a;
}

byte *maketit(byte *s1, byte *s2, byte *opt)
{
byte *to, *t1, *t2, *r;
unsigned int i;

STARTDEB("maketit")
MSGDEB(2, (LOG_FACILITY, "s1=%s s2=%s opt=%s", s1, s2, opt));

r = (byte *) MALLOC(LOCSTR);
r[0] = '\0';

to = (byte *) malloc(LOC_STRING_SIZE);
lrtrim(opt, to);

t1 = (byte *) malloc(LOC_STRING_SIZE);
lrtrim(s1, t1);

t2 = (byte *) malloc(LOC_STRING_SIZE); 
lrtrim(s2, t2);

if ((to[0]=='N') && (to[1]=='\0')) {
 for (i = 0; i < strlen(t1); i++)
   if (isalnum(t1[i])) {
     t1[i]=dtoupper(t1[i]);
     break;
     }
for (i = 0; i < strlen(t2); i++)
  if (isalnum(t2[i])) {
    t2[i]=dtoupper(t2[i]);
    break;
    }
}
free(to);

strcat(r, t1);
if (t1[strlen(t1)-1]!='\'')
  strcat(r, " ");
strcat(r, t2);

  free(t1);
  free(t2);
ENDDEB("maketit")
  return r;
  
}

byte *extraitvalparam(byte *OptionString, byte *ParamName)
{
byte *result, *a;
int NewFlag=1;
unsigned int i, p, j=0;

SETLOCALE
STARTDEB("extraitvalparam")
MSGDEB(2, (LOG_FACILITY, "extraitvalparam: OptString: %s\n ParamName: %s\n", OptionString, ParamName));

result = (byte *) MALLOC(LOCSTR);
result[0] = '\0'; /* Terminate the string */
a = (byte *) malloc(LOCSTR);

for (i = 0; i < strlen(OptionString); i++) {
  if ( OptionString[i] != 0xA6) {
    if (NewFlag)
      j = 0;

    a[j]=OptionString[i]; /* Copy till delimiter */
    NewFlag=0;
    j++;
    a[j] = '\0'; /* Terminate the string */
    continue;
    }
  if (!NewFlag) {
    NewFlag=1;
/* Searching for equal sign */
    p = lpos("=", a);
    if (strncmp(a, ParamName, p-1)==0) { /* Found some */
    strcpy(result, a+p);
    free(a);
MSGDEB(2, (LOG_FACILITY, "extraitvalparam: res=%s", result));
      return result; 
      }
    }
  }

free(a);
return result;
}

byte* extmotjoker(byte *c, int *word)
{
int space=1;
int counter=1;
unsigned int i, j=0;
byte *result;

SETLOCALE
STARTDEB("extmotjoker")
MSGDEB(2, (LOG_FACILITY, "%s %d", c, *word));

result = (byte *) MALLOC(LOCSTR);

for(i = 0; i < strlen(c); i++)
  if ( isalnum(c[i]) || (c[i]==':') || (c[i]=='%') ) {
    space=0;
    if (counter==*word) {
      result[j]=c[i];
      j++;
      }
  } else
    if (!space) {
      space=1;
      if (counter!=*word)
        counter++;
      else break;
    }

result[j]='\0';
ENDDEB("extraitmotjoker")
return result;    
}

byte *souschaine(byte *Src, byte *delim, unsigned int *Which)
{
byte *dest;
unsigned int i, Count=1, j=0;

STARTDEB("souschaine")		
dest = (byte *) MALLOC(LOCSTR);

if (*delim=='\0')
  *delim='\\';

if (*Which==0)
  *Which=1;

if (*Src!='\0')
  for (i = 0; i < strlen(Src); i++)
    if (strncmp(Src+i, delim, strlen(delim))==0) {
      Count++;
      if (Count > *Which)
        break;           /* Let's be speedy */
      i+=strlen(delim)-1; /* Where to start copy from */
      } else
    if (*Which==Count) {
      dest[j]=Src[i];
      j++;
      }

dest[j]='\0';
ENDDEB("souschaine")
return dest;
}

byte *strdatetimefrtoib(byte *src)
{
byte *result;
unsigned int d, m, y;

STARTDEB("strdatetimefrtoib")
result = (byte *) MALLOC(LOCSTR);
result[0] = '\0';
/* check that it contains '/' in right palaces - 1 to 2 digits
 * also swap day and month.
 */
if (sscanf(src, "%d/%d/%d", &m, &d, &y) == 3)
  sprintf(result, "%2d/%2d/%4d", d, m, y);
ENDDEB("strdatetimefrtoib")
return result;
}

byte *formatdate(ISC_QUAD *date, byte *format)
{
struct tm t;
byte *result, *format2;

SETLOCALE
STARTDEB("formatdate")
result = (byte *) MALLOC(LOCSTR);
format2 = (byte *) MALLOC(LOCSTR);
MSGDEB(2, (LOG_FACILITY, "%s", format));
isc_decode_date(date, &t);

formatdatetime(format, format2);
t.tm_sec=t.tm_min=t.tm_hour=0;
strftime(result, LOCSTR, format2, &t);
free(format2);
ENDDEB("formatdate")
return result;
}

