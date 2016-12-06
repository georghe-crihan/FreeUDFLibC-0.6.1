#ifndef DTYPE_H_INCLUDED
#define DTYPE_H_INCLUDED

typedef unsigned char byte;

extern char vowels[]; 
extern char diacriticals[];
extern char notvowels[];
extern byte upchars[];
extern byte lowchars[];

#define notvowel(c) notvowels[c]
#define isvowel(c) vowels[c]
#define isdiacr(c) diacriticals[c]
#define dtoupper(c) upchars[c]
#define dtolower(c) lowchars[c]

void dstrupper(byte *, byte *);
void dstrlower(byte *, byte *);

#endif /* DTYPE_H_INCLUDED */
