/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   MathFncs.c
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

#include <stdlib.h>
#include <math.h>
#include "udf_glob.h"
#include "MathFncs.h"

static int GetValueOf(char, char *);

static int GetValueOf(char c, char * digits)
{
  int i;
STARTDEB("GetValueOf")  
  for (i = 0 ;
       (digits[i] != '\0') && (digits[i] != c) ;
       i++) ;
ENDDEB("GetValueOf")
  return i;
}

int ConvertFrom33( char * sz)
{
  int base = 33;
STARTDEB("ConvertFrom33")  
  return ConvertFromBase(sz, &base, "123456789ABCDEFGHIJKLMNPQRSTUVWXYZ");
}

char * ConvertTo33(int * value)
{
  int base = 33;
STARTDEB("ConvertTo33")  
  return ConvertToBase(value, &base, "123456789ABCDEFGHIJKLMNPQRSTUVWXYZ");
}

int ConvertFromBase(char * sz, int * base, char * digits)
{
  int i;
  int res = 0;
  int len = strlen(sz);
STARTDEB("ConvertFromBase")  
  for (i = 0 ; i < len ; i++)
    res = (*base * res) + GetValueOf(sz[i], digits);
ENDDEB("ConvertFromBase")
  return res;
}

char * ConvertToBase(int * value, int *base, char *digits)
{
  char sz_res[LOC_STRING_SIZE + 1];
  char * sz_result = (char *) MALLOC (LOC_STRING_SIZE + 1);
  int i = 0;
  int j, r, len_sz_res;
  int first_time = 1;
STARTDEB("ConvertToBase")  
  for ( j = *value ;
       (j != 0) || first_time;
	j = j / *base, first_time = 0){
    r = j % *base;
    sz_res[i++] = digits[r];
  }
  sz_res[i] = '\0';
  sz_result[i] = '\0';
  len_sz_res = i;
  
  for (i = 0 ; i < len_sz_res ; i++)
    sz_result[i] = sz_res[len_sz_res - i - 1];
ENDDEB("ConvertToBase")
  return sz_result;
}

char * DollarVal(double * value)
{
  char * sz_result = (char *) MALLOC (LOC_STRING_SIZE + 1);
STARTDEB("DollarVal")  
  sprintf(sz_result, "$%.2f", (*value));
ENDDEB("DollarVal")    
  return sz_result;
}

double DoubleAbs(double * value)
{
STARTDEB("DoubleAbs")
  if (*value < 0)
    return -*value;
  else
    return *value;
}

char * FixedPoint(double * value, int * decimals)
{
  char * sz_result = (char *) MALLOC (LOC_STRING_SIZE + 1);
STARTDEB("FixedPoint")  
  sprintf(sz_result, "%.*f", *decimals, *value);
ENDDEB("FixedPoint")    
  return sz_result;
}

int IntegerAbs(int * value)
{
STARTDEB("IntegerAbs")
  if (*value < 0)
    return -*value;
  else
    return *value;
}

int Modulo(int * numerator, int * denominator)
{
STARTDEB("Modulo")
  if (*denominator == 0)
    return 0;
  else
    return *numerator % *denominator;
}

int Round(double *value)
{
STARTDEB("Round")
  if (*value < 0)
    return (int)(*value - 0.5);
  else
    return (int)(*value + 0.5);
}

double RoundFloat(double *Value, double *RoundToNearest)
{
  double t, result;
  int int_val, int_rnd, remainder;
  STARTDEB("RoundFloat")
  t = *Value * 100.00;
  int_val = Round(&t);
  t = *RoundToNearest * 100.00;
  int_rnd = Round(&t);
  if (int_rnd == 0) { 
    result = 0;
    return result; 
  }
  remainder = int_val % int_rnd;
  if (remainder > 0) { 
    if (int_rnd / remainder > 2)	/* round down */
      result = int_val - remainder;
    else				/* round up */
      result = int_val + (int_rnd - remainder);
    /* Now divide the result by 100 to get the proper result. */
    result /= 100;
  } else
    result = *Value;
  ENDDEB("RoundFloat")
  return result;
}

int Truncate(double * value)
{
STARTDEB("Truncate")
  return (int) (*value);
}

int IsDivisibleBy(int *Numerator, int *Denominator)
{
  STARTDEB("IsDivisibleBy")
  if (*Denominator == 0)
    return 0;
  else if (*Numerator == 0)
    return 1;
  else if (*Numerator % *Denominator > 0)
    return 0;
  else return 1;
}

double RoundD(double *x, int *i)
{
double n, xi, fi;
STARTDEB("RoundD")
n = pow(10, *i);
modf((modf((*x * n), &xi) * 2), &fi);
ENDDEB("RoundD")
return (xi+fi)/n;
}
