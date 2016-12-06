/*
 * This is intended for Bison parser generator as it supports generation
 * of reentrant parsers. Only %pure-parser is Bison-specific. But Bison
 * is available on so many platforms, that there should be no problems.
 */

%{
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
%}

%pure_parser		/* Thread-safe parser, please */

%union {
  time_t	tivalue;
  struct tm	*tvalue;
  double	dvalue;
  int		ivalue;
  char		*svalue;
}

%token '(' ')'
%token '=' NEQ LEQ GEQ '>' '<' STARTS_WITH CONTAINS
%token <ivalue> TOK_TRUE TOK_FALSE
%token <tvalue> TOK_DATE
%token <dvalue> DOUBLE
%token <ivalue> INTEGER 
%token <svalue> STRING
%token SYM LEXERRD LEXERRN

%left  AND OR XOR NAND NOR
%left '+' '-'
%left '*' '/' DIV MOD
%right NOT DAY MONTH YEAR

%left STARTS_WITH CONTAINS

%type <ivalue> expr bool rel
%type <tvalue> dates
%type <tivalue> date
%type <dvalue> num 
%type <svalue> str

%%
expr:	  bool			{ $$ = $1;
				  $1==0?yyerror("FALSE"):yyerror("TRUE");}
	| /* empty */		{ $$ = 0; yyerror("FALSE");		}
	;

bool:	  TOK_TRUE		{ $$ = 1;				}
	| TOK_FALSE		{ $$ = 0;				}
	| bool AND bool		{ $$ = $1 && $3;			}
	| SYM			{ yyerror("Variable not found"); YYABORT;}
	| bool OR bool		{ $$ = $1 || $3;			}
	| bool XOR bool		{ $$ = $1 ^ $3;				}
	| bool NOR bool		{ $$ = ! ($1 || $3);			}
	| bool NAND bool	{ $$ = ! ($1 && $3);			}
	| NOT bool		{ $$ = ! $2;				}
	| '(' bool ')'		{ $$ = $2;				}
	| rel			{ $$ = $1;				}
	;

rel:	  date '=' date		{ $$ = ($1 == $3);			}
	| date NEQ date		{ $$ = ($1 != $3);			}
	| date LEQ date		{ $$ = ($1 <= $3);			}
	| date GEQ date		{ $$ = ($1 >= $3);			}
	| date '>' date		{ $$ = ($1 > $3);			}
	| date '<' date		{ $$ = ($1 < $3);			}
	| num '=' num		{ $$ = ($1 == $3);			}
	| num NEQ num		{ $$ = ($1 != $3);			}
	| num LEQ num		{ $$ = ($1 <= $3);			}
	| num GEQ num		{ $$ = ($1 >= $3);			}
	| num '>' num		{ $$ = ($1 >  $3);			}
	| num '<' num		{ $$ = ($1 <  $3);			}
	| str '=' str		{ $$ = (strcmp($1, $3) == 0);		}
	| str NEQ str		{ $$ = (strcmp($1, $3) != 0);		}
	| str LEQ str		{ $$ = (strcmp($1, $3) <= 0);		}
	| str GEQ str		{ $$ = (strcmp($1, $3) >= 0);		}
	| str '>' str		{ $$ = (strcmp($1, $3) > 0);		}
	| str '<' str		{ $$ = (strcmp($1, $3) < 0);		}
	| str STARTS_WITH str	{ $$ = ($1 == strstr($1, $3));		}
	| str CONTAINS str	{ $$ = (strstr($1, $3) != NULL);	}
	;

date:	  dates			{ $$ = mktime($1);			}
	;

dates: 	  TOK_DATE		{ $$ = $1;				}
	| LEXERRD		{ yyerror("Date format error"); YYABORT;}
	| dates '+' DAY num	{ $1->tm_mday += (int) $4;		}
	| dates '+' MONTH num	{ $1->tm_mon += (int) $4;		}
	| dates '+' YEAR num	{ $1->tm_year += (int) $4;		}
	| dates '-' DAY num	{ $1->tm_mday -= (int) $4;		}
	| dates '-' MONTH num	{ $1->tm_mon -= (int) $4;		}
	| dates '-' YEAR num	{ $1->tm_year -= (int) $4;		}
	| '(' dates ')'		{ $$ = $2;				}
	;

num:	  DOUBLE		{ $$ = $1;				}
	| INTEGER		{ $$ = (double) $1;			}
	| LEXERRN		{ yyerror("Float format error");YYABORT;}
	| num '+' num		{ $$ = $1 + $3;				}
	| num '-' num		{ $$ = $1 - $3;				}
	| num '*' num		{ $$ = $1 * $3;				}
	| num '/' num		{ $$ = $1 / $3;				}
	| num MOD num		{ $$ = (float)((int) ((int) $1 / (int) $3));}
	| num DIV num		{ $$ = (float)((int) ((int) $1 / (int) $3));}
	| '(' num ')'		{ $$ = $2;				}
	;

str:	  STRING		{ $$ = $1; 				}
	| str '+' str		{ $$ = mystrcat(YYPARSE_PARAM, $1, $3);	}
	;
%%

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
