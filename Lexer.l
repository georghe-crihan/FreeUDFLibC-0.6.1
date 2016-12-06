/*
 * This is prmarily for flex(1), with a strong emphasis is on POSIX,
 * (in fact, only %option and buffers are flex-specific), but flex
 * is available on so many platforms, that there should be no problems.
 */

%{
#ifndef FLEX_SCANNER
#error Flex required to generate the lexer.
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include "SymTable.h"

#define PREP_STRING yr(svalue) = (char *) malloc(YYLMAX)
%}

%option nostdinit
%option caseless
%option noyywrap
%option noinput
%option nounput
%option never-interactive

%x EQUAL_SIGN
%x SYMBOL
%x COMMENT
%x STR
%x STRD
%x DT

ID [[:alpha:]_\.]([[:alnum:]_\.])*
NUM -?[[:digit:]]+
DAT [[:digit:]]{1,2}\/[[:digit:]]{1,2}\/[[:digit:]]{1,2}
%%
%{
	int c=YY_START;
	if (pr(pass)==SYMBOL_LOAD)
	  BEGIN(SYMBOL);	/* Load symbol table */
	else BEGIN(INITIAL);
%}
<SYMBOL,INITIAL,EQUAL_SIGN>\/\*	{ c = YY_START;
				  BEGIN(COMMENT);			}
<COMMENT>\*\/			BEGIN(c);
<COMMENT>\n			|
<COMMENT>.			/* Eat up comments */
\"				{ PREP_STRING;
				  *yr(svalue)='\0';
				  BEGIN(STRD);				}
"'"				{ PREP_STRING;
				  *yr(svalue)='\0';
				  BEGIN(STR);				}
<STR,STRD>\n			|
<STR,STRD>([^"'])*		strcat(yr(svalue), yytext);
<STRD>\"\"			|
<STR>''				strcat(yr(svalue), yytext+1);
<STRD>\"			|
<STR>"'"			{ if(pr(pass)==PARSING) /* Catch string const */
					addtolist((struct symrec **) &pr(var),NULL,STRING,yp);
				  BEGIN(INITIAL);
				  return STRING;			}
"{"				{ BEGIN(DT);
				  yr(tvalue)=(struct tm*)malloc(sizeof(struct tm));
				  memset(yr(tvalue),'\0',sizeof(struct tm));}
<DT>{DAT}			sscanf(yytext, "%d/%d/%d", &yr(tvalue->tm_mon), &yr(tvalue->tm_mday), &yr(tvalue->tm_year));
<DT>([^}])*			return LEXERRD; /* Anti-backup catch-all */
<DT>"}"				{ if (pr(pass)==PARSING) /* Catch date const */
					addtolist((struct symrec **) &pr(var),NULL,TOK_DATE, yp);
				  BEGIN(INITIAL);
				  return TOK_DATE;			}
{NUM}\.[[:digit:]]+		{ yr(dvalue) = atof(yytext);
				  return DOUBLE;			}
{NUM}\.[^[:space:]]*		return LEXERRN; /* Anti-backup catch-all */
{NUM}				{ yr(ivalue) = atoi(yytext);
				  return INTEGER;			}
"$"[[:digit:]]+			{ yr(ivalue) = atoi(yytext+1);
				  return INTEGER;			}
not				return NOT;
and				return AND;
nand				return NAND;
or				return OR;
xor				return XOR;
nor				return NOR;
"!="				| 
"<>"				return NEQ;
"<="				return LEQ;
">="				return GEQ;
">"				|
"<"				|
"="				|
"+"				|
"-"				|
"*"				|
"/"				|
[()]				return yytext[0];
starts_with			return STARTS_WITH;
contains			return CONTAINS;
true				return TOK_TRUE;
false				return TOK_FALSE;
div				return DIV;
mod				return MOD;
month				return MONTH;
day				return DAY;
year				return YEAR;
{ID}				return searchlist(pr(var), yytext, yp);
<*>[[:space:]]+			/* Skip over white space */

 /*
  * Rules below refer to variable name->value substitution
  */
<SYMBOL>{ID}			{ PREP_STRING;
				  *yr(svalue) = '\0';
				  strcpy(yr(svalue), yytext);
				  BEGIN(EQUAL_SIGN);			}

 /* This rule is here to allow whitespaces before and after '='		*/
<EQUAL_SIGN>"="			{ BEGIN(INITIAL);
				  return SYM;				}
%%

/*
 * Symbol list stuff
 */

void addtolist(struct symrec **list, char *name, int type, YYSTYPE *value)
{
struct symrec *tmp;

tmp = (*list);
(*list) = (struct symrec *) malloc(sizeof(struct symrec));
(*list)->next=tmp;
(*list)->type=type;
(*list)->name=name;
 if (type==STRING)	/* Save some space */ 
	value->svalue = (char *)realloc(value->svalue, strlen(value->svalue)+1);
memcpy(&(*list)->value, value, sizeof(YYSTYPE));
}

int searchlist(struct symrec *list, char *name, YYSTYPE *value)
{
while(list!=NULL) {
	if ((list->name!=NULL) && (strcasecmp(name, list->name)==0)){
		memcpy(value, &list->value, sizeof(YYSTYPE));
		return list->type;
	}
	list=list->next;
}
return SYM;
}

void cleanup_list(struct symrec *list)
{
struct symrec *tmp;

while (list!=NULL) {
	tmp = list->next;
	if (list->name!=NULL) free(list->name);
	if (list->type == STRING) free(list->value.svalue);
	if (list->type == TOK_DATE) free(list->value.tvalue);
	free(list);
	list=tmp; 
	}
}

/*
 * Main function
 */
char *EvaluateExpression(char *exprin, char *symin)
{
char *symp;
int token;
YYSTYPE yylval;
struct param par;
STARTDEB("EvaluateExpression")
SETLOCALE
par.var=NULL;
yy_scan_string(symin);

par.pass=SYMBOL_LOAD;

while(1) {
  par.pass=SYMBOL_LOAD;
  token=yylex(&yylval, (void *)&par);
  if (token==0) break;	/* End of variables */
  if (token!=SYM) {	/* Accept symbols only, otherwise abort */
    printf("%d", token);
    yyerrormy("Symbol loading error: variable name expected", &par);
    break;
    }
  symp=yylval.svalue;
  par.pass=VALUE_LOAD;
  token=yylex(&yylval, (void *)&par);
  addtolist((struct symrec **) &par.var, symp, token, &yylval);
} 

if (token==0) {		/* Symbols loaded correctly, reached EOF */
  yy_delete_buffer(YY_CURRENT_BUFFER);

  /* Done loading symbols. Prepare for parsing. */
  par.pass=PARSING;
  yy_scan_string(exprin);
  yyparse((void *)&par);
  }
yy_delete_buffer(YY_CURRENT_BUFFER);
cleanup_list(par.var);
MSGDEB(2, (LOG_FACILITY, "result: %s; symbols: %s; expression: %s.", par.emsg,symin,exprin));
ENDDEB("EvaluateExpression")
return par.emsg;
}

