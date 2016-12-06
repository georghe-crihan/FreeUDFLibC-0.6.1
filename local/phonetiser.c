/*
 * Hopefully it is reentrant now - thus suits multithreaded model and a bit
 * faster.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "dtype.h"

#ifndef STANDALONE
#include "../udf_glob.h"
#include "../StrFncs.h"
#else
//#define DEBUG 3
#include "phonetiser.h"
#endif


/* French diacritical letters */
#define CETwoDotsSmall 0xEB
#define CITwoDotsSmall 0xEF
#define CCedillaCap 0xC7
#define CCedillaSmall 0xE7
#define EAcoute  0xE8 /* Small   */
#define EGrave   0xE9 /* Small   */

/*
 * Strings
 */

static byte	ANCap[]="AN", OUCap[]="OU", ONCap[]="ON", CHCap[]="CH",
		NICap[]="NI", INCap[]="IN", UNCap[]="UN", WACap[]="WA",
		MMCap[]="MM", NNCap[]="NN", MNCap[]="MN", IRCap[]="IR",
		NHCap[]="NH", HNCap[]="HN", MPCap[]="MP", MBCap[]="MB",
		GACap[]="GA", GECap[]="GE", GICap[]="GI", GOCap[]="GO",
		GUCap[]="GU", GYCap[]="GY", UECap[]="UE", UICap[]="UI",
		UYCap[]="UY", CECap[]="CE", ISCap[]="IS", ISSCap[]="ISS",
		SSCap[]="SS", SCCap[]="SC", STCap[]="ST", MMICap[]="MMI",
		LLCap[]="LL", MHCap[]="MH", CICap[]="CI", WINCap[]="WIN",
		KSSmall[]="ks", 

		     UTwoDotsSmall[]="\xFC", UCircumflexSmall[]="\xFB",
		     OCircumflexSmall[]="\xF4", ICircumflexSmall[]="\xEE",
		     EAcouteSmall[]="\xE8", EGraveSmall[]="\xE9",

		     ASmall[]="a", BSmall[]="b", DSmall[]="d", ESmall[]="e",
		     GSmall[]="g", ISmall[]="i", JSmall[]="j", KSmall[]="k",
                     LSmall[]="l", MSmall[]="m", NSmall[]="n", OSmall[]="o",
                     PSmall[]="p", RSmall[]="r", SSmall[]="s", TSmall[]="t",
                     USmall[]="u", VSmall[]="v", YSmall[]="y", ZSmall[]="z",

                     ACap[]="A", BCap[]="B", CCap[]="C", DCap[]="D",
                     ECap[]="E", FCap[]="F", GCap[]="G", HCap[]="H",
		     ICap[]="I", JCap[]="J", KCap[]="K", LCap[]="L",
		     MCap[]="M", NCap[]="N", PCap[]="P", RCap[]="R",
		     SCap[]="S", TCap[]="T", UCap[]="U", XCap[]="X",
                     YCap[]="Y", ZCap[]="Z",

		     SSmallCCedillaCapCCedilla[]="\xE7\xC7\0",
                     SSmallEAcouteSmallECircumflex[]="\xE8\xEA\0";

struct param {
  byte *src;
  unsigned int isrc;

  byte *tsrc;

  byte *dst;

  byte current;
  byte next;
  byte next_up;
  byte prev;
  byte prev_up;
};

static int strcmp_op_case(struct param *p, byte *s)
{
unsigned int i;
int c;

if ((strlen(p->tsrc)==0) ||
    (strlen(s)==0))
  return 0;

for (i=0; i < strlen(s); i++) {
  if (i >= strlen(p->tsrc))
    return 0;
  c=p->tsrc[i];
  if (isupper(s[i]))
    c=dtoupper(c);

  if (c!=s[i])
    return 0;
}

return 1;
}

/* Check for S or X on end of src. */
static int SOrXOnEnd(struct param *p, unsigned int Arg)
{
if (Arg==strlen(p->src)) /* Reached end of string */
  return 1;

if (Arg==strlen(p->src)-1) /* Reached end of string */
  if ( (dtoupper(p->src[Arg])=='S') ||
       (dtoupper(p->src[Arg])=='X'))
    return 1;
return 0;
}


/* If src contains 'S' or 'X' on end advance isrc by 15 chars, return true. */
static int CheckSOrXAdvancePtr(struct param *p)
{
if (SOrXOnEnd(p, p->isrc)) {
  p->isrc+=15;
  return 1;
  }
return 0;
}

#ifdef SIZE_OPT
void AppendArgToResult(struct param *p, byte *s)
{
strcat(p->dst, s);
p->isrc++;
}
#else
#define AppendArgToResult(p, s) { \
	strcat(p->dst, s); \
	p->isrc++; \
	}
#endif

/* If tsrc contains any of the specified chunks, append specified phonema
 * to dst. Advance isrc by the specified ChunkLen.
 */
static int ExpandToPhonema(struct param *p, byte *phonema,
					int ChunkLen, ...)
{
va_list vl;
int res=0;
byte *b;


va_start(vl, ChunkLen);
for ( b=va_arg(vl, byte*); b!=NULL; b=va_arg(vl, byte*)) 
  if (strcmp_op_case(p, b)) {
  if (phonema!=NULL)
    strcat(p->dst, phonema);
    p->isrc+=ChunkLen;
    res=1;
    break;
  }
va_end(vl);
return res;
}

static int ExpandToOnePhonema(struct param *p, byte *AppString, byte *MatchChars)
{
int i;

for (i = 0; MatchChars[i]!='\0'; i++) 
  if (MatchChars[i]==(p->current)) {
    AppendArgToResult(p, AppString);
    return 1;
    }

return 0;
}

static int AppendIfCurCharDiacrVowel(struct param *p, byte *s)
{
if (isdiacr(p->current)) {
  AppendArgToResult(p, s);
  return 1;
  }

return 0;
}

/* Acts upon single chunk/phonema */
static int ExpandFirstPhonema(struct param *p, byte *Phonema,
					int ChunkLen, byte *Chunk)
{
if ((strcmp_op_case(p, Chunk)) && (p->isrc==1)) {
  strcat(p->dst, Phonema);
  p->isrc+=ChunkLen;
  return 1;
  }
return 0;
}

static int ExpandIfSOrX(struct param *p, byte *phonema,
					int PtrOffset, ...)
{
va_list vl;
int res=0;
byte *b;

va_start(vl, PtrOffset);
for ( b=va_arg(vl, byte*); b!=NULL; b=va_arg(vl, byte*)) 
  if (strcmp_op_case(p, b) &&
      SOrXOnEnd(p, (p->isrc)+PtrOffset)) {
  if (phonema!=NULL)
    strcat(p->dst, phonema);
  p->isrc+=15;
  res=1;
  break;
  }

va_end(vl);
return res;
}

static int ExpandPhonemaCheckVowel(struct param *p, byte *phonema,
			char Type, int Pos, int ChunkLen, ...)
{
int res=0;
va_list vl;
byte *b;

va_start(vl, ChunkLen);

for ( b=va_arg(vl, byte*); b!=NULL; b=va_arg(vl, byte*)) 
  if (strcmp_op_case(p, b))
    if (((Type=='V') && isvowel(p->src[(p->isrc)+Pos-1])) ||
        ((Type=='C') && notvowel(p->src[(p->isrc)+Pos-1])) ) {
      if (phonema!=NULL)
        strcat(p->dst, phonema);
      p->isrc+=ChunkLen;
      res = 1;
      break;
      }

va_end(vl);
return res;
}

/*
 * Handle individual letters
 */

static void LetterA(struct param *p)
{
if (AppendIfCurCharDiacrVowel(p, ASmall) ||
    ExpandToPhonema(p, ASmall, 1, MMCap, NNCap, MNCap, NULL))
  return;
if (strcmp_op_case(p, NCap) ||
    strcmp_op_case(p, MCap)) {
  if (isvowel(p->src[(p->isrc)+1])) {
    AppendArgToResult(p, ASmall);
    return;
    } else {
    strcat(p->dst, ANCap);
    p->isrc+=2;
    return;
    }
  }
if (ExpandToPhonema(p, OSmall, 2, OCircumflexSmall, NULL) ||
    ExpandToPhonema(p, ASmall, 2, "oNN", NULL) ||
    ExpandToPhonema(p, OUCap, 3, "oU", NULL) ||
    ExpandToPhonema(p, ANCap, 3, "oN", "eN", NULL) ||
    ExpandToPhonema(p, EAcouteSmall, 2, ESmall, ICircumflexSmall, IRCap, ISSCap, ISCap, NULL))
  return;
if (strcmp_op_case(p, "iM") ||
    strcmp_op_case(p, "iN")) {
  if (isvowel(p->src[(p->isrc)+2]))
    AppendArgToResult(p, EAcouteSmall);
  p->isrc++;
  }

if (ExpandIfSOrX(p, "ay", 2, "iL", NULL) ||
    ExpandToPhonema(p, ASmall, 1, "iLL", NULL) ||
    ExpandIfSOrX(p, EGraveSmall, 4, "ient", NULL) ||
    ExpandToPhonema(p, EGraveSmall, 2, ISmall, NULL) ||
    ExpandFirstPhonema(p, EGraveSmall, 1, YCap) ||
    ExpandPhonemaCheckVowel(p, EAcouteSmall, 'V', 2, 1, YCap, NULL) ||
    ExpandToPhonema(p, OSmall, 2, USmall, UTwoDotsSmall, NULL))
  return;

if (strcmp_op_case(p, ACap) &&
    !isdiacr(p->next)) {
  strcat(p->dst, ASmall);
  p->isrc+=2;
  }
AppendArgToResult(p, ASmall);
}

static void LetterB(struct param *p)
{
if (!ExpandToPhonema(p, BSmall, 2, BCap, NULL))
  AppendArgToResult(p, BSmall);
}

static void LetterC(struct param *p)
{
if (!ExpandToOnePhonema(p, SSmall, SSmallCCedillaCapCCedilla) &&
    !ExpandToPhonema(p, KSmall, 3, CHCap, "Qu", NULL) &&
    !ExpandToPhonema(p, KSSmall, 2, "Ci", CECap, NULL) &&
    !ExpandToPhonema(p, KSmall, 2, CCap, HNCap, KCap, NULL) &&
    !ExpandIfSOrX(p, KSmall, 1, HCap, NULL) &&
    !ExpandPhonemaCheckVowel(p, CHCap, 'V', 2, 2, HCap, NULL) &&
    !ExpandPhonemaCheckVowel(p, KSmall, 'C', 2, 2, HCap, NULL) &&
    !ExpandToPhonema(p, SSmall, 1, ECap, ICap, YCap, NULL))
  AppendArgToResult(p, KSmall);
}

static void LetterD(struct param *p)
{
if (!ExpandToPhonema(p, DSmall, 2, DCap, NULL) &&
  !CheckSOrXAdvancePtr(p)) 
  AppendArgToResult(p, DSmall);
}

static void LetterE(struct param *p)
{
if (ExpandToOnePhonema(p, EGraveSmall, EGraveSmall) ||
    ExpandToOnePhonema(p, EAcouteSmall, SSmallEAcouteSmallECircumflex))
  return;
if (p->current==CETwoDotsSmall) {
  if (CheckSOrXAdvancePtr(p))
    return;
  AppendArgToResult(p, EAcouteSmall);
  return;
  }

if (SOrXOnEnd(p, p->isrc) &&
    (p->isrc > 1) &&
    (strlen(p->dst)!=0) &&
    ((p->dst[strlen(p->dst)-2]==EAcoute) ||
     (p->dst[strlen(p->dst)-2]==EGrave))) {
  p->isrc+=15;
  return;
  }

if (SOrXOnEnd(p, p->isrc) && 
    !isdiacr(p->current)) {
  if (strlen(p->src) > 3) {
    p->isrc+=15;
    return;
    }

  strcat(p->dst, ESmall);
  p->isrc+=15;
  return;
  }

if (ExpandToPhonema(p, EAcouteSmall, 2, "iNE", "iGN", "iCH", "iGE",
				"IGLE", "iZE", NULL) ||
    ExpandToPhonema(p, NULL, 1, "iN", NULL) ||
    ExpandIfSOrX(p, "Ey", 2, "iL", NULL) ||
    ExpandToPhonema(p, EAcouteSmall, 1, ISmall, SSCap, NULL) ||
    ExpandToPhonema(p, EGraveSmall, 1, SCCap, NULL) ||
    ExpandIfSOrX(p, EGraveSmall, 1, RCap, NULL) ||
    ExpandToPhonema(p, ECap, 2, USmall, NULL))
  return;
if ((p->prev_up=='H') && 
    (p->isrc==1)) {
  AppendArgToResult(p, EGraveSmall);
  return;
  }

if ((p->prev_up=='I') &&
    strcmp_op_case(p, NNCap)) {
  AppendArgToResult(p, EAcouteSmall);
  return;
  }
  
if ((p->prev=='i') &&
    strcmp_op_case(p, NHCap) &&
    isvowel(p->src[(p->isrc)+2])) {
  AppendArgToResult(p, INCap);
  return;
  }

if (ExpandPhonemaCheckVowel(p, ANCap, 'C', 2, 2, NCap, NULL) ||
    ExpandToPhonema(p, ASmall, 1, "MMeNT", NULL) ||
    ExpandToPhonema(p, ANCap, 2, MMCap, MPCap, MBCap, NULL) ||
    ExpandPhonemaCheckVowel(p, ESmall, 'V', 2, 1, NCap, MCap, NULL) ||
    ExpandToPhonema(p, NULL, 1, "au", UCircumflexSmall, NULL) ||
    ExpandToPhonema(p, ISmall, 2, ESmall, NULL))
  return;

if (isvowel(p->next)) {
  p->isrc++;
  return;
  }

if ((p->prev_up=='G') &&
    (isvowel(p->next))) {
  p->isrc++;
  return;
  }

if (notvowel(p->next) &&
    (dtoupper(p->src[(p->isrc)+1])==p->next_up)) {
  AppendArgToResult(p, EAcouteSmall);
  return;
  }

if (((p->next_up=='Z') ||
     (p->next_up=='T')) &&
    (SOrXOnEnd(p, (p->isrc)+1))) {
  strcat(p->dst, EGraveSmall);
  p->isrc+=2;
  return;
  }

if ((p->next_up=='L') &&
    ((SOrXOnEnd(p, (p->isrc)+1) ||
     notvowel(p->src[(p->isrc)+1])))) {
  AppendArgToResult(p, EAcouteSmall);
  return;
  }

if (strcmp_op_case(p, STCap) &&
    (strlen(p->src)-3==p->isrc)) {
  strcat(p->dst, EAcouteSmall);
  p->isrc+=15;
  return;
  }

if ((p->next_up=='N') &&
    SOrXOnEnd(p, (p->isrc)+1) &&
    ((p->prev_up=='E') ||
     (p->prev_up=='I') ||
     (p->prev_up=='Y'))) {
  strcat(p->dst, INCap);
  p->isrc+=15;
  return;
  }

if (notvowel(p->next) &&
    notvowel(p->src[(p->isrc)+1])) {
  AppendArgToResult(p, EAcouteSmall);
  return;
  }

if (!ExpandToPhonema(p, EGraveSmall, 1, XCap, NULL))
  AppendArgToResult(p, ESmall);
}

static void LetterG(struct param *p)
{
if (!CheckSOrXAdvancePtr(p) &&
    !ExpandPhonemaCheckVowel(p, NICap, 'V', 2, 2, NCap, NULL) &&
    !ExpandToPhonema(p, "gj", 2, GECap, GICap, GYCap, NULL) &&
    !ExpandToPhonema(p, GSmall, 2,GACap,GOCap, GUCap, UECap, UICap, UYCap, NULL) &&
    !ExpandPhonemaCheckVowel(p, GSmall, 'C', 2, 2, GCap, NULL) &&
    !ExpandToPhonema(p, JSmall, 1, ECap, ICap, YCap, NULL))
  AppendArgToResult(p, GSmall);
}

static void LetterF(struct param *p)
{
if (!ExpandToPhonema(p, FCap, 2, FCap, NULL)) 
  AppendArgToResult(p, FCap);
}

/* There is no sound of H */
static void LetterH(struct param *p) { p->isrc++; }

static void LetterI(struct param *p)
{
if ((p->current==CITwoDotsSmall) &&
    (p->prev=='a') &&
    isvowel(p->next)) {
  AppendArgToResult(p, YSmall);
  return;
  }

if (AppendIfCurCharDiacrVowel(p, ISmall))
  return;

if (strcmp_op_case(p, LLCap)) {
  if (!isvowel(p->prev))
    strcat(p->dst, ISmall);
  strcat(p->dst, YSmall);
  p->isrc+=3;
  return;
}

if (ExpandIfSOrX(p, "iIN", 2, "eN", NULL) ||
    ExpandToPhonema(p, ISmall, 1, NNCap, MMICap, NULL) ||
    ExpandPhonemaCheckVowel(p, INCap, 'V', 3, 2, MMCap, NULL) ||
    ExpandIfSOrX(p, INCap, 1, NCap, NULL) ||
    ExpandPhonemaCheckVowel(p, INCap, 'C', 2, 2, NCap, MCap, NULL) ||
    ExpandPhonemaCheckVowel(p, ISmall, 'V', 2, 1, MCap, NCap, NULL))
  return;

if (isvowel(p->next) &&
    isvowel(p->prev)) {
  AppendArgToResult(p, YSmall);
  return;
  }

if (notvowel(p->prev)) {
  AppendArgToResult(p, ISmall);
  return;
  }

AppendArgToResult(p, ISmall);
}

static void LetterJ(struct param *p)
{
if (!ExpandToPhonema(p, JSmall, 2, JCap, NULL))
  AppendArgToResult(p, JSmall);
}

static void LetterK(struct param *p)
{
if (!ExpandToPhonema(p, KSmall, 2, KCap, NULL))
  AppendArgToResult(p, KSmall);
}

static void LetterL(struct param *p)
{
if (!ExpandToPhonema(p, LSmall, 2, LCap, NULL))
  AppendArgToResult(p, LSmall);
}

static void LetterM(struct param *p)
{
if (!ExpandToPhonema(p, MSmall, 2, MCap, NULL) &&
    !ExpandToPhonema(p, NULL, 1, NCap, NULL)) 
   AppendArgToResult(p, MSmall);
}

static void LetterN(struct param *p)
{
if (!ExpandToPhonema(p, NSmall, 2, NCap, NULL))
  AppendArgToResult(p, NSmall);
}

static void LetterO(struct param *p)
{
if (ExpandToPhonema(p, NULL, 1, "eil", NULL) ||
    ExpandToPhonema(p, OUCap, 2, OSmall, UCap, NULL) ||
    ExpandPhonemaCheckVowel(p, NULL, 'C', 3, 1, "eu", NULL) ||
    ExpandPhonemaCheckVowel(p, NULL, 'C', 2, 1, ESmall, NULL) ||
    ExpandToPhonema(p, OSmall, 1, NNCap, MMCap, MNCap, NULL) ||
    ExpandPhonemaCheckVowel(p, OSmall, 'V', 3, 1, MHCap, NHCap, NULL) ||
    ExpandIfSOrX(p, ONCap, 1, NCap, MCap, NULL) ||
    ExpandPhonemaCheckVowel(p, ONCap, 'C', 2, 2, NCap, MCap, NULL))
  return;

if (notvowel(p->next)) {
  AppendArgToResult(p, OSmall);
  return;
  }

if (strcmp_op_case(p, "iN")) {
  if (SOrXOnEnd(p, (p->isrc)+2)) {
    strcat(p->dst, WINCap);
    p->isrc+=15;
    return;
    }

  if (notvowel(p->src[(p->isrc)+2])) {
    strcat(p->dst, WINCap);
    p->isrc+=3;
    return;
    }
  }

if (!ExpandToPhonema(p, WACap, 2, ISmall, NULL) &&
    !ExpandIfSOrX(p, WACap, 1, ISmall, NULL) &&
    !ExpandToPhonema(p, WACap, 1, YSmall, NULL))
  AppendArgToResult(p, OSmall);
}

static void LetterP(struct param *p)
{
if (!ExpandToPhonema(p, PSmall, 2, PCap, NULL) &&
    !ExpandToPhonema(p, NULL, 1, TCap, NULL) &&
    !ExpandToPhonema(p, FCap, 2, HCap, NULL) &&
    !CheckSOrXAdvancePtr(p))
   AppendArgToResult(p, PSmall);
}

static void LetterQ(struct param *p)
{
if ((p->isrc==1) &&
    (p->prev=='a') &&
    strcmp_op_case(p, "ua") &&
    notvowel(p->src[(p->isrc)+2])) {
  strcat(p->dst, "kWA");
  p->isrc+=3;
  return;
  }

if (!ExpandToPhonema(p, KSmall, 2, USmall, NULL)) 
  AppendArgToResult(p, KSmall);
}

static void LetterR(struct param *p)
{
if (!ExpandToPhonema(p, RSmall, 2, RCap, NULL)) {
  if (SOrXOnEnd(p, p->isrc)) {
    strcat(p->dst, RSmall);
    p->isrc+=15;
    } else
    AppendArgToResult(p, RSmall);
  }
}

static void LetterS(struct param *p)
{
if ((p->next_up=='H') &&
    isvowel(p->prev) &&
    isvowel(p->src[(p->isrc)+1])) {
  strcat(p->dst, ZSmall);
  p->isrc+=2;
  return;
  }

if (ExpandFirstPhonema(p, CHCap, 2, HCap) ||
    ExpandIfSOrX(p, CHCap, 1, HCap, NULL) ||
    ExpandPhonemaCheckVowel(p, CHCap, 'V', 3, 3, CHCap, NULL) ||
    ExpandToPhonema(p, SSmall, 2, SCap, NULL))
  return;

if (SOrXOnEnd(p, p->isrc))
  if ((p->prev_up=='O') ||
      (p->prev_up=='Y')) {
    strcat(p->dst, SSmall);
    p->isrc+=15;
    return;
    }

if (CheckSOrXAdvancePtr(p) ||
    ExpandToPhonema(p, SSmall, 2, CECap, CICap, NULL))
  return;

if (isvowel(p->prev) &&
    isvowel(p->next)) {
  AppendArgToResult(p, ZSmall);
  return;
  }


AppendArgToResult(p, SSmall);
}

static void LetterT(struct param *p)
{
if (CheckSOrXAdvancePtr(p) ||
    ExpandToPhonema(p, TSmall, 2, TCap, HCap, NULL))
  return;

if (p->prev_up=='S') {
  AppendArgToResult(p, TSmall);
  return;
  }

if (strcmp_op_case(p, "ioNS") &&
    SOrXOnEnd(p, (p->isrc)+4)) {
  AppendArgToResult(p, TSmall);
  return;
  }

if (!ExpandPhonemaCheckVowel(p, SSmall, 'V', 2, 1, ISmall, NULL)) 
  AppendArgToResult(p, TSmall);
}

static void LetterU(struct param *p)
{
if (AppendIfCurCharDiacrVowel(p, USmall) ||
    ExpandToPhonema(p, USmall, 2, UCap, NULL) ||
    ExpandToPhonema(p, NULL, 1, "ei", NULL) ||
    ExpandToPhonema(p, USmall, 1, NNCap, MMCap, MNCap, NULL))
  return;

if ((p->next_up=='M') &&
    isvowel(p->prev) &&
    SOrXOnEnd(p, (p->isrc)+1)) {
  AppendArgToResult(p, ECap);
  return;
  }

if (!ExpandIfSOrX(p, UNCap, 1, NCap, MCap, NULL) &&
    !ExpandPhonemaCheckVowel(p, ECap, 'C', 2, 2, NCap, MCap, NULL))
  AppendArgToResult(p, USmall);
}

/* V & W */
static void LetterV(struct param *p)
{
AppendArgToResult(p, VSmall);
}

static void LetterX(struct param *p)
{
if ((p->prev_up=='E') &&
    ((p->isrc==1) ||
      ((p->isrc==2) &&
       (dtoupper(p->src[0])=='H')))) {
  AppendArgToResult(p, "gz");
  return;
  }

if (ExpandIfSOrX(p, KSSmall, 1, ECap, NULL) ||
   CheckSOrXAdvancePtr(p))
  return;

if ((p->prev_up=='E') && 
    notvowel(p->next)) {
  AppendArgToResult(p, KSSmall);
  return;
  }

if (isvowel(p->prev) &&
    isvowel(p->next)) {
  AppendArgToResult(p, SSmall);
  return;
  }

AppendArgToResult(p, SSmall);
}

static void LetterY(struct param *p)
{
if ((p->isrc==1) &&
    (p->prev_up=='S') &&
    notvowel(p->src[(p->isrc)+1]) && 
    ((p->next_up=='N') || (p->next_up=='M')) ) { /* Syntaxe or Symmetrie */ 
     strcat(p->dst, INCap);
     p->isrc+=2;
     return;
     }

if (isvowel(p->prev) && isvowel(p->next)) {
  AppendArgToResult(p, YSmall);
 } else
  AppendArgToResult(p, ISmall);
}

static void LetterZ(struct param *p)
{
if (!ExpandToPhonema(p, ZSmall, 2, ZCap, NULL) &&
    !CheckSOrXAdvancePtr(p))
  AppendArgToResult(p, ZSmall);
}

/* It's a bit faster and clea[rn]er then a 'switch', in this particular case. */
typedef void (*funcp)(struct param *);

static
funcp phonetics[] = { LetterA, LetterB, LetterC, LetterD, LetterE, LetterF,
	      	      LetterG, LetterH, LetterI, LetterJ, LetterK, LetterL,
		      LetterM, LetterN, LetterO, LetterP, LetterQ, LetterR,
		      LetterS, LetterT, LetterU, LetterV, LetterV, LetterX,
		      LetterY, LetterZ };
/*
 * Main routine
 */

byte *phonetiser(byte *src)
{
int c;
unsigned int i;
struct param p;

SETLOCALE
STARTDEB("phonetiser")
/* Prepare source */
p.src = (byte *) malloc(254);
p.src[0] = '\0';
lrtrim(src, p.src);
strlower(p.src, p.src);

p.dst = (byte *) MALLOC(254);
p.dst[0] = '\0';

/* Main stuff */
for (p.isrc=1; p.isrc <= strlen(p.src); ) {
  p.current=p.src[p.isrc-1];
  p.next=p.src[p.isrc];
  p.next_up=(byte) dtoupper(p.next);
  p.tsrc = p.src + (p.isrc < strlen(p.src) ? p.isrc : strlen(p.src));

  if (p.isrc <= 1) {
    p.prev=p.src[0];
    p.prev_up=p.src[0];
    }
  else {
    p.prev=p.src[p.isrc-2];
    p.prev_up=dtoupper(p.prev);
    }
  c = dtoupper(p.current);
  if (isalpha(c)) {
    if (c == CCedillaCap) {
      LetterC(&p);
      continue;
      }
    if (c <= 'Z') {
      phonetics[c-'A'](&p);
      continue;
      }
    } /* isalpha */
      p.isrc++;
  } /* for */

/* Final stuff */
free(p.src);
for (i = 0; i < strlen(p.dst); i++)
  if ((p.dst[i]==EAcoute) ||
      (p.dst[i]==EGrave) ||
      (p.dst[i]=='E')) p.dst[i]='e';
ENDDEB("phonetiser")
return p.dst;
}
