typedef union {
  time_t	tivalue;
  struct tm	*tvalue;
  double	dvalue;
  int		ivalue;
  char		*svalue;
} YYSTYPE;
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

