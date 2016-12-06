/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   Misc.h
 *     Miscellaneous routines declarations:
 *	 ValidateBillindExpression -
 *       IBTempPath -
 *	 GenerateFormattedName -
 *	 ValidateNameFormat - 
 *	 ValidateCyclicExpression -
 *	 EvaluateCyclicExpression -
 *	 ValidateRegularExpression -
 *	 ValidateStringInRE -
 */

#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#ifdef __cplusplus
 extern "C"
 {
#endif

int waitchild PROTO_LIST ((pid_t));
pid_t shell PROTO_LIST ((char *, char **));
int ValidateBillingExpression PROTO_LIST ((char *, int *));
char * IBTempPath PROTO_LIST ((void));
char * GenerateFormattedName PROTO_LIST ((char *, char *, char *, char *, char *, char *));
int ValidateNameFormat PROTO_LIST ((char *));
int EvaluateCycleExpression PROTO_LIST ((char *, int *, ISC_QUAD *, ISC_QUAD *, double *));
int ValidateCycleExpression PROTO_LIST ((char *, int *));
char *wintransform PROTO_LIST ((char *, char *));
int ValidateRegularExpression PROTO_LIST ((char *));
int ValidateStringInRE PROTO_LIST ((char *, char *));

#ifdef __cplusplus
 }
#endif
#endif /* MISC_H_INCLUDED */
