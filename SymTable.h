#ifndef SYMTAB_H_INCLUDED
#define SYMTAB_H_INCLUDED

#include <time.h>
#include "udf_glob.h"

#define YYLMAX 8192

#if !(defined SYM) && !(defined YYBISON) /* To avoid subsequent inclusion */
#include "ExprPars.h"
#endif /* SYM */

enum pass_name {
  SYMBOL_LOAD,
  VALUE_LOAD,
  PARSING
};

struct param {
  char *emsg;
  enum pass_name pass;
  void *var;
};

char *EvaluateExpression(char *expin, char *symin);

#define YYLEX_PARAM par
#define YYPARSE_PARAM YYLEX_PARAM

void yyerrormy(char *msg, struct param *YYPARSE_PARAM);

#define YY_DECL int yylex(YYSTYPE *yylval, void *YYLEX_PARAM)

/* Macros to facilitate switching berween reentrant/nonreentrant versions */
#define pr(a) (((struct param *) YYLEX_PARAM)->a)
#define yp yylval
#define yr(a) (yylval->a)

#ifndef YYBISON		/* Avoid definition conflicts with parser */
struct symrec
{
char *name;
int  type;
struct symrec *next;
YYSTYPE value;
};

#ifdef __cplusplus
 extern "C"
 {
#endif

void addtolist (struct symrec **list, char *name, int type, YYSTYPE *value);
int searchlist(struct symrec *list, char *name, YYSTYPE *value);
void cleanup_list(struct symrec *list);

#ifdef __cplusplus
 }
#endif
#endif /* YYBISON */

#endif /* SYMTAB_H_INCLUDED */
