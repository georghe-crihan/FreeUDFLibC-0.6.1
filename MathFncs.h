/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   MathFncs.h
 *     Math routines declarations:
 *       ConvertFrom33 - convert a string (base33) to an integer
 *       ConvertTo33 - convert an integer to base33 string
 *       ConvertFromBase - convert a string (base<n>) to an integer
 *       ConvertToBase - convert an integer to a base<n> string
 *       DollarVal - convert a double to a dollar string ($xxxx.xx)
 *       DoubleAbs - get the absolute value of a double
 *       FixedPoint - convert a double to a string with <n> decimal points
 *       IntegerAbs - get the absolute value of an integer
 *       Modulo - perform modulo arithmetic
 *       Round - round a double to the nearest integer
 *       Truncate - return the integer part of a double
 *	RoundFloat -
 *	IsDivisibleBy -
 */

#ifndef MATHFNCS_H_INCLUDED
#define MATHFNCS_H_INCLUDED

#ifdef __cplusplus
 extern "C"
 {
#endif

int ConvertFrom33 PROTO_LIST ((char *));
char * ConvertTo33 PROTO_LIST ((int *));
int ConvertFromBase PROTO_LIST ((char *, int *, char *));
char * ConvertToBase PROTO_LIST ((int *, int *, char *));
char * DollarVal PROTO_LIST ((double *));
double DoubleAbs PROTO_LIST ((double *));
char * FixedPoint PROTO_LIST ((double *, int *));
int IntegerAbs PROTO_LIST ((int *));
int Modulo PROTO_LIST ((int *, int *));
int Round PROTO_LIST ((double *));
double RoundFloat PROTO_LIST ((double *, double *));
int Truncate PROTO_LIST ((double *));
int IsDivisibleBy PROTO_LIST ((int *, int *));
double RoundD PROTO_LIST ((double *, int *));

#ifdef __cplusplus
 }
#endif
#endif /* MATHFNCS_H_INCLUDED */
