
/*  A Bison parser, made from ExprPars.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	NEQ	257
#define	LEQ	258
#define	GEQ	259
#define	STARTS_WITH	260
#define	CONTAINS	261
#define	TOK_TRUE	262
#define	TOK_FALSE	263
#define	TOK_DATE	264
#define	DOUBLE	265
#define	INTEGER	266
#define	STRING	267
#define	SYM	268
#define	LEXERRD	269
#define	LEXERRN	270
#define	AND	271
#define	OR	272
#define	XOR	273
#define	NAND	274
#define	NOR	275
#define	DIV	276
#define	MOD	277
#define	NOT	278
#define	DAY	279
#define	MONTH	280
#define	YEAR	281

#line 7 "ExprPars.y"

#ifndef YYBISON
#error Bison required to generate the parser.
#endif
//#define STANDALONE /* When using as a standalone, e.g. for testing */
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "SymTable.h"

static char *mystrcat (void *YYPARSE_PARAM, char *src, char *dst);

/* 
 * Yes, I know it's a dirty hack, but how else can a error routine pass
 * messages out of parser in a thread-safe manner?
 */
#define yyerror(msg) yyerrormy(msg, (struct param *)YYPARSE_PARAM)

#line 29 "ExprPars.y"
typedef union {
  time_t	tivalue;
  struct tm	*tvalue;
  double	dvalue;
  int		ivalue;
  char		*svalue;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		109
#define	YYFLAG		-32768
#define	YYNTBASE	37

#define YYTRANSLATE(x) ((unsigned)(x) <= 281 ? yytranslate[x] : 44)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
     4,    29,    27,     2,    28,     2,    30,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    10,
     5,     9,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     6,     7,     8,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    31,    32,    33,    34,    35,
    36
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     3,     5,     7,    11,    13,    17,    21,    25,
    29,    32,    36,    38,    42,    46,    50,    54,    58,    62,
    66,    70,    74,    78,    82,    86,    90,    94,    98,   102,
   106,   110,   114,   118,   120,   122,   124,   129,   134,   139,
   144,   149,   154,   158,   160,   162,   164,   168,   172,   176,
   180,   184,   188,   192,   194
};

static const short yyrhs[] = {    38,
     0,     0,    13,     0,    14,     0,    38,    22,    38,     0,
    19,     0,    38,    23,    38,     0,    38,    24,    38,     0,
    38,    26,    38,     0,    38,    25,    38,     0,    33,    38,
     0,     3,    38,     4,     0,    39,     0,    40,     5,    40,
     0,    40,     6,    40,     0,    40,     7,    40,     0,    40,
     8,    40,     0,    40,     9,    40,     0,    40,    10,    40,
     0,    42,     5,    42,     0,    42,     6,    42,     0,    42,
     7,    42,     0,    42,     8,    42,     0,    42,     9,    42,
     0,    42,    10,    42,     0,    43,     5,    43,     0,    43,
     6,    43,     0,    43,     7,    43,     0,    43,     8,    43,
     0,    43,     9,    43,     0,    43,    10,    43,     0,    43,
    11,    43,     0,    43,    12,    43,     0,    41,     0,    15,
     0,    20,     0,    41,    27,    34,    42,     0,    41,    27,
    35,    42,     0,    41,    27,    36,    42,     0,    41,    28,
    34,    42,     0,    41,    28,    35,    42,     0,    41,    28,
    36,    42,     0,     3,    41,     4,     0,    16,     0,    17,
     0,    21,     0,    42,    27,    42,     0,    42,    28,    42,
     0,    42,    29,    42,     0,    42,    30,    42,     0,    42,
    32,    42,     0,    42,    31,    42,     0,     3,    42,     4,
     0,    18,     0,    43,    27,    43,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    60,    62,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    78,    79,    80,    81,    82,    83,    84,
    85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
    95,    96,    97,   100,   103,   104,   105,   106,   107,   108,
   109,   110,   111,   114,   115,   116,   117,   118,   119,   120,
   121,   122,   123,   126,   127
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","'('","')'",
"'='","NEQ","LEQ","GEQ","'>'","'<'","STARTS_WITH","CONTAINS","TOK_TRUE","TOK_FALSE",
"TOK_DATE","DOUBLE","INTEGER","STRING","SYM","LEXERRD","LEXERRN","AND","OR",
"XOR","NAND","NOR","'+'","'-'","'*'","'/'","DIV","MOD","NOT","DAY","MONTH","YEAR",
"expr","bool","rel","date","dates","num","str", NULL
};
#endif

static const short yyr1[] = {     0,
    37,    37,    38,    38,    38,    38,    38,    38,    38,    38,
    38,    38,    38,    39,    39,    39,    39,    39,    39,    39,
    39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
    39,    39,    39,    40,    41,    41,    41,    41,    41,    41,
    41,    41,    41,    42,    42,    42,    42,    42,    42,    42,
    42,    42,    42,    43,    43
};

static const short yyr2[] = {     0,
     1,     0,     1,     1,     3,     1,     3,     3,     3,     3,
     2,     3,     1,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     1,     1,     1,     4,     4,     4,     4,
     4,     4,     3,     1,     1,     1,     3,     3,     3,     3,
     3,     3,     3,     1,     3
};

static const short yydefact[] = {     2,
     0,     3,     4,    35,    44,    45,    54,     6,    36,    46,
     0,     1,    13,     0,    34,     0,     0,     0,    34,     0,
    11,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    12,    43,    53,     5,     7,
     8,    10,     9,     0,    14,    15,    16,    17,    18,    19,
     0,     0,     0,     0,     0,     0,     0,    20,    21,    22,
    23,    24,    25,    47,    48,    49,    50,    52,    51,    26,
    27,    28,    29,    30,    31,    32,    33,    55,     0,    37,
    38,    39,    40,    41,    42,     0,     0,     0,     0
};

static const short yydefgoto[] = {   107,
    12,    13,    14,    15,    16,    17
};

static const short yypact[] = {    64,
    64,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    64,    76,-32768,    47,    38,    81,   114,   123,     5,    -2,
-32768,    64,    64,    64,    64,    64,   137,   137,   137,   137,
   137,   137,    60,    70,   134,   134,   134,   134,   134,   134,
   134,   134,   134,   134,   134,   134,     6,     6,     6,     6,
     6,     6,     6,     6,     6,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   137,-32768,-32768,-32768,-32768,-32768,-32768,
   134,   134,   134,   134,   134,   134,   134,    31,    31,    31,
    31,    31,    31,   -10,   -10,-32768,-32768,-32768,-32768,     4,
     4,     4,     4,     4,     4,     4,     4,-32768,     5,   -10,
   -10,   -10,   -10,   -10,   -10,    19,    68,    69,-32768
};

static const short yypgoto[] = {-32768,
    92,-32768,   101,     0,    -1,   -37
};


#define	YYLAST		157


static const short yytable[] = {    20,
    19,    58,    35,    36,    37,    38,    39,    40,    57,    90,
    91,    92,    93,    94,    95,    96,    97,    98,    43,    44,
    45,    46,    58,     7,    41,    42,    43,    44,    45,    46,
    55,    33,    34,    78,    79,    80,    81,    82,    83,    84,
    85,    86,    87,    88,    89,    41,    42,    43,    44,    45,
    46,    27,    28,    29,    30,    31,    32,    41,    42,    43,
    44,    45,    46,    99,    33,    34,     1,   108,   109,   100,
   101,   102,   103,   104,   105,   106,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    35,    36,    37,    38,    39,
    40,     0,    18,    71,    72,    73,    11,    22,    23,    24,
    25,    26,    21,    74,    75,    76,     0,    41,    42,    43,
    44,    45,    46,    59,    60,    61,    62,    63,    47,    48,
    49,    50,    51,    52,    53,    54,    56,    65,    66,    67,
    68,    69,    70,     0,     0,     0,    77,     0,     0,    64,
    55,     0,     0,     0,    22,    23,    24,    25,    26,     5,
     6,     4,     0,     0,    10,     0,     9
};

static const short yycheck[] = {     1,
     1,     4,     5,     6,     7,     8,     9,    10,     4,    47,
    48,    49,    50,    51,    52,    53,    54,    55,    29,    30,
    31,    32,     4,    18,    27,    28,    29,    30,    31,    32,
    27,    27,    28,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    27,    28,    29,    30,    31,
    32,     5,     6,     7,     8,     9,    10,    27,    28,    29,
    30,    31,    32,    64,    27,    28,     3,     0,     0,    71,
    72,    73,    74,    75,    76,    77,    13,    14,    15,    16,
    17,    18,    19,    20,    21,     5,     6,     7,     8,     9,
    10,    -1,     1,    34,    35,    36,    33,    22,    23,    24,
    25,    26,    11,    34,    35,    36,    -1,    27,    28,    29,
    30,    31,    32,    22,    23,    24,    25,    26,     5,     6,
     7,     8,     9,    10,    11,    12,     4,    27,    28,    29,
    30,    31,    32,    -1,    -1,    -1,     3,    -1,    -1,     3,
    27,    -1,    -1,    -1,    22,    23,    24,    25,    26,    16,
    17,    15,    -1,    -1,    21,    -1,    20
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/local/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 60 "ExprPars.y"
{ yyval.ivalue = yyvsp[0].ivalue;
				  yyvsp[0].ivalue==0?yyerror("FALSE"):yyerror("TRUE");;
    break;}
case 2:
#line 62 "ExprPars.y"
{ yyval.ivalue = 0; yyerror("FALSE");		;
    break;}
case 3:
#line 65 "ExprPars.y"
{ yyval.ivalue = 1;				;
    break;}
case 4:
#line 66 "ExprPars.y"
{ yyval.ivalue = 0;				;
    break;}
case 5:
#line 67 "ExprPars.y"
{ yyval.ivalue = yyvsp[-2].ivalue && yyvsp[0].ivalue;			;
    break;}
case 6:
#line 68 "ExprPars.y"
{ yyerror("Variable not found"); YYABORT;;
    break;}
case 7:
#line 69 "ExprPars.y"
{ yyval.ivalue = yyvsp[-2].ivalue || yyvsp[0].ivalue;			;
    break;}
case 8:
#line 70 "ExprPars.y"
{ yyval.ivalue = yyvsp[-2].ivalue ^ yyvsp[0].ivalue;				;
    break;}
case 9:
#line 71 "ExprPars.y"
{ yyval.ivalue = ! (yyvsp[-2].ivalue || yyvsp[0].ivalue);			;
    break;}
case 10:
#line 72 "ExprPars.y"
{ yyval.ivalue = ! (yyvsp[-2].ivalue && yyvsp[0].ivalue);			;
    break;}
case 11:
#line 73 "ExprPars.y"
{ yyval.ivalue = ! yyvsp[0].ivalue;				;
    break;}
case 12:
#line 74 "ExprPars.y"
{ yyval.ivalue = yyvsp[-1].ivalue;				;
    break;}
case 13:
#line 75 "ExprPars.y"
{ yyval.ivalue = yyvsp[0].ivalue;				;
    break;}
case 14:
#line 78 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].tivalue == yyvsp[0].tivalue);			;
    break;}
case 15:
#line 79 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].tivalue != yyvsp[0].tivalue);			;
    break;}
case 16:
#line 80 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].tivalue <= yyvsp[0].tivalue);			;
    break;}
case 17:
#line 81 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].tivalue >= yyvsp[0].tivalue);			;
    break;}
case 18:
#line 82 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].tivalue > yyvsp[0].tivalue);			;
    break;}
case 19:
#line 83 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].tivalue < yyvsp[0].tivalue);			;
    break;}
case 20:
#line 84 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].dvalue == yyvsp[0].dvalue);			;
    break;}
case 21:
#line 85 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].dvalue != yyvsp[0].dvalue);			;
    break;}
case 22:
#line 86 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].dvalue <= yyvsp[0].dvalue);			;
    break;}
case 23:
#line 87 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].dvalue >= yyvsp[0].dvalue);			;
    break;}
case 24:
#line 88 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].dvalue >  yyvsp[0].dvalue);			;
    break;}
case 25:
#line 89 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].dvalue <  yyvsp[0].dvalue);			;
    break;}
case 26:
#line 90 "ExprPars.y"
{ yyval.ivalue = (strcmp(yyvsp[-2].svalue, yyvsp[0].svalue) == 0);		;
    break;}
case 27:
#line 91 "ExprPars.y"
{ yyval.ivalue = (strcmp(yyvsp[-2].svalue, yyvsp[0].svalue) != 0);		;
    break;}
case 28:
#line 92 "ExprPars.y"
{ yyval.ivalue = (strcmp(yyvsp[-2].svalue, yyvsp[0].svalue) <= 0);		;
    break;}
case 29:
#line 93 "ExprPars.y"
{ yyval.ivalue = (strcmp(yyvsp[-2].svalue, yyvsp[0].svalue) >= 0);		;
    break;}
case 30:
#line 94 "ExprPars.y"
{ yyval.ivalue = (strcmp(yyvsp[-2].svalue, yyvsp[0].svalue) > 0);		;
    break;}
case 31:
#line 95 "ExprPars.y"
{ yyval.ivalue = (strcmp(yyvsp[-2].svalue, yyvsp[0].svalue) < 0);		;
    break;}
case 32:
#line 96 "ExprPars.y"
{ yyval.ivalue = (yyvsp[-2].svalue == strstr(yyvsp[-2].svalue, yyvsp[0].svalue));		;
    break;}
case 33:
#line 97 "ExprPars.y"
{ yyval.ivalue = (strstr(yyvsp[-2].svalue, yyvsp[0].svalue) != NULL);	;
    break;}
case 34:
#line 100 "ExprPars.y"
{ yyval.tivalue = mktime(yyvsp[0].tvalue);			;
    break;}
case 35:
#line 103 "ExprPars.y"
{ yyval.tvalue = yyvsp[0].tvalue;				;
    break;}
case 36:
#line 104 "ExprPars.y"
{ yyerror("Date format error"); YYABORT;;
    break;}
case 37:
#line 105 "ExprPars.y"
{ yyvsp[-3].tvalue->tm_mday += (int) yyvsp[0].dvalue;		;
    break;}
case 38:
#line 106 "ExprPars.y"
{ yyvsp[-3].tvalue->tm_mon += (int) yyvsp[0].dvalue;		;
    break;}
case 39:
#line 107 "ExprPars.y"
{ yyvsp[-3].tvalue->tm_year += (int) yyvsp[0].dvalue;		;
    break;}
case 40:
#line 108 "ExprPars.y"
{ yyvsp[-3].tvalue->tm_mday -= (int) yyvsp[0].dvalue;		;
    break;}
case 41:
#line 109 "ExprPars.y"
{ yyvsp[-3].tvalue->tm_mon -= (int) yyvsp[0].dvalue;		;
    break;}
case 42:
#line 110 "ExprPars.y"
{ yyvsp[-3].tvalue->tm_year -= (int) yyvsp[0].dvalue;		;
    break;}
case 43:
#line 111 "ExprPars.y"
{ yyval.tvalue = yyvsp[-1].tvalue;				;
    break;}
case 44:
#line 114 "ExprPars.y"
{ yyval.dvalue = yyvsp[0].dvalue;				;
    break;}
case 45:
#line 115 "ExprPars.y"
{ yyval.dvalue = (double) yyvsp[0].ivalue;			;
    break;}
case 46:
#line 116 "ExprPars.y"
{ yyerror("Float format error");YYABORT;;
    break;}
case 47:
#line 117 "ExprPars.y"
{ yyval.dvalue = yyvsp[-2].dvalue + yyvsp[0].dvalue;				;
    break;}
case 48:
#line 118 "ExprPars.y"
{ yyval.dvalue = yyvsp[-2].dvalue - yyvsp[0].dvalue;				;
    break;}
case 49:
#line 119 "ExprPars.y"
{ yyval.dvalue = yyvsp[-2].dvalue * yyvsp[0].dvalue;				;
    break;}
case 50:
#line 120 "ExprPars.y"
{ yyval.dvalue = yyvsp[-2].dvalue / yyvsp[0].dvalue;				;
    break;}
case 51:
#line 121 "ExprPars.y"
{ yyval.dvalue = (float)((int) ((int) yyvsp[-2].dvalue / (int) yyvsp[0].dvalue));;
    break;}
case 52:
#line 122 "ExprPars.y"
{ yyval.dvalue = (float)((int) ((int) yyvsp[-2].dvalue / (int) yyvsp[0].dvalue));;
    break;}
case 53:
#line 123 "ExprPars.y"
{ yyval.dvalue = yyvsp[-1].dvalue;				;
    break;}
case 54:
#line 126 "ExprPars.y"
{ yyval.svalue = yyvsp[0].svalue; 				;
    break;}
case 55:
#line 127 "ExprPars.y"
{ yyval.svalue = mystrcat(YYPARSE_PARAM, yyvsp[-2].svalue, yyvsp[0].svalue);	;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/local/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 129 "ExprPars.y"


void yyerrormy(char *emsg, struct param *p)
{
#ifdef STANDALONE /* No InterBase */
  p->emsg = (char *) malloc(strlen(emsg)+1);
#else /* Inside InterBase UDF */
  p->emsg = (char *) MALLOC(strlen(emsg)+1);
#endif /* STANDALONE */
  strcpy(p->emsg, emsg);
}

static char *mystrcat(void *t, char *src, char *dst)
{
YYSTYPE p;
p.svalue = (char *)malloc(YYLMAX+1);
*p.svalue = '\0';
strcat(p.svalue, src);
strcat(p.svalue, dst);
addtolist((struct symrec **) &((struct param *)t)->var, NULL, STRING, &p);
return p.svalue;
}

#ifdef STANDALONE /* Stub main() */
int main(int argc, char **argv)
{
char *sym;

if (argc!=3) return 1;
sym=(char *)EvaluateExpression(argv[1], argv[2]);
puts(sym);
free(sym);

return 0;
}
#endif STANDALONE 
