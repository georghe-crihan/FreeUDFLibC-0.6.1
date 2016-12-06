#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>

/* Below is just a lame and lazy attempt to emulate DELPHI FormatDateTime.
 * It is far from being complete, but near to be enough for my particular
 * purpose.
 */
/* strftime(3) format specifiers */
static char myf[]="%e/%m/%y";
//#define LongTimeFormat cf
#define LongTimeFormat myf

static char cf[]="%c", ef[]="%e", df[]="%d", af[]="%a", Af[]="%A", xf[]="%x",
	    fulldate[]="%A %e %B %Y", mf[]="%m", bf[]="%b", Bf[]="%B",
	    yf[]="%y", Yf[]="%Y", kf[]="%k", Hf[]="%H", Mf[]="%M", Sf[]="%S",
	    dyf[]="y", yfd[]="%yy"; /* dummies for year */

/* DELPHI character chains */
static char *cc[]={(char *)1, LongTimeFormat},
	    *dc[]={(char *)6, ef, df, af, Af, xf, fulldate},
            *mc[]={(char *)4, mf, mf, bf, Bf},
	    *yc[]={(char *)4, dyf, yf, yfd, Yf},
	    *hc[]={(char *)2, kf, Hf},
	    *nc[]={(char *)2, Mf, Mf},
	    *sc[]={(char *)2, Sf, Sf};

/* DELPHI chains synopsis */
/*
 * blank: %c // correct time
 * c: %c // correct time
 * d: %e
 * dd: %d
 * ddd: %a
 * dddd: %A
 * ddddd: %x
 * dddddd: %A %d %B %Y // zeros
 * m: %m // zeros, minute
 * mm: %m // minute
 * mmm: %b
 * mmmm: %B
 * yy: %y
 * yyyy: %Y
 * h: %k
 * hh: %H
 * n: %M // zeros
 * nn: %M
 * s: %S // zeros
 * ss: %S
 * z: // zeros
 * zzz:
 */ 

static int subst(unsigned int *, char *, unsigned int, char *, char **);

char *formatdatetime(char *src, char *dst)
{
unsigned int i=0, j=0;
int t;

*dst='\0';
setlocale(LC_TIME, "fr_FR.ISO_8859-1");
if (strlen(src)==0) {
  subst(&i, "c", j, dst, cc);
  return dst;
  }

for (i=0; i < strlen(src); i++)
  switch(tolower(src[i])) {
    case '\'':
    case '\"':
	if (src[i+1]==src[i]) { /* Quotes requested */
	  dst[j]=src[i]; j++; i+=2;
          }
        else { /* Protect against expansion */
          t=src[i];
          for (i++; (src[i]!=t) && (i < strlen(src)); i++) { /* Copy through */
            dst[j]=src[i];
            j++;
            }
          }
    	break; /* Quotes */
    case 'c':
	j=subst(&i, src, j, dst, cc);
	break;
    case 'd':
	j=subst(&i, src, j, dst, dc);
        break;
    case 'm':
	j=subst(&i, src, j, dst, mc);
	break;
    case 'y':
	j=subst(&i, src, j, dst, yc);
	break;
    case 'h':
	j=subst(&i, src, j, dst, hc);
	break;
    case 'n':
	j=subst(&i, src, j, dst, nc);
	break;
    case 's':
	j=subst(&i, src, j, dst, sc);
	break;
    case '%':
	dst[j]='%'; j++;
	dst[j]='%'; j++;
	break; /* Percent */
    default:
	dst[j]=src[i];
	j++;
  }

dst[j]='\0';
return dst;
}

static int subst(unsigned int *i, char *src, unsigned int j, char *dst, char **chain)
{
int items;
int t;

items = (int) chain[0];
dst[j]='\0';
for (t=0; tolower(src[*i])==tolower(src[*i+t]); t++) /* nothing */;
if (t > items)
  t = items; /* Character chain too long? */

strcat(dst, chain[t]);
t--;
(*i)+=t;

return strlen(dst);
}
