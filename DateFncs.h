/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   DateFncs.c
 *     Date and time routines:
 *       AddMonth - add some number of months to a date
 *       AddYear - add some number of years to a date
 *       AgeInMonths - Between "date" and "ref_date", how many months?
 *       AgeInWeeks - Between "date" and "ref_date", how many weeks?
 *       CDOWLong - Convert the date to a "long" pretty version
 *       CDOWShort - Convert the date to a "short" pretty version
 *       CMonthLong - What is the long name of the month?
 *       CMonthShort - What is the short name of the month?
 *       DayOfMonth - What is the day of the month?
 *       DayOfWeek - What is the day of the week?
 *       DayOfYear - What is the day of the year?
 *       IsLeapYear - Is it leap year?
 *       MaxDate - Compare two dates and return the maximum.
 *       MinDate - Compare two dates and return the minimum.
 *       Month - What is the month?
 *       Quarter - What is the quarter?
 *       StripDate - Strip the date portion of the date
 *       StripTime - Strip the time portion of the date
 *       StrToTime - Given a time string (hh:mm:ss AM/PM) return date of it
 *       WeekOfYear - What is the week of the year?
 *       Woy - Return the year and the week concatenated (199901)
 *	Year - What is the year?
 *	YearOfYear - Given a date, return the year as it would
 *                      be returned from the week_of_year function.
 *	AgeInDays -
 *	AgeInMonthsTreshold -
 *	AgeInWeeksTreshold -
 *	AgeInDaysTreshold -
 *	Hour -
 *	Minute -
 *
 */

#ifndef DATEFNCS_H_INCLUDED
#define DATEFNCS_H_INCLUDED

#ifdef __cplusplus
 extern "C"
 {
#endif

ISC_QUAD * AddMonth PROTO_LIST ((ISC_QUAD *, int *));
ISC_QUAD * AddYear PROTO_LIST ((ISC_QUAD *, int *));
int AgeInMonths PROTO_LIST ((ISC_QUAD *, ISC_QUAD *));
int AgeInWeeks PROTO_LIST ((ISC_QUAD *, ISC_QUAD *));
char * CDOWLong PROTO_LIST ((ISC_QUAD *));
char * CDOWShort PROTO_LIST ((ISC_QUAD *));
char * CMonthLong PROTO_LIST ((ISC_QUAD *));
char * CMonthShort PROTO_LIST ((ISC_QUAD *));
int DayOfMonth PROTO_LIST ((ISC_QUAD *));
int DayOfWeek PROTO_LIST ((ISC_QUAD *));
int DayOfYear PROTO_LIST ((ISC_QUAD *));
int IsLeapYear PROTO_LIST ((ISC_QUAD *));
ISC_QUAD * MaxDate PROTO_LIST ((ISC_QUAD *, ISC_QUAD *));
ISC_QUAD * MinDate PROTO_LIST ((ISC_QUAD *, ISC_QUAD *));
int Month PROTO_LIST ((ISC_QUAD *));
int Quarter PROTO_LIST ((ISC_QUAD *));
ISC_QUAD * StripDate PROTO_LIST ((ISC_QUAD *));
ISC_QUAD * StripTime PROTO_LIST ((ISC_QUAD *));
ISC_QUAD * StrToTime PROTO_LIST ((char *));
int WeekOfYear PROTO_LIST ((ISC_QUAD *));
char * WOY PROTO_LIST ((ISC_QUAD *));
int Year PROTO_LIST ((ISC_QUAD *));
int YearOfYear PROTO_LIST ((ISC_QUAD *));
int AgeInMonthsThreshold PROTO_LIST ((ISC_QUAD *, ISC_QUAD *, int *, int *, int *, int *));
int AgeInWeeksThreshold PROTO_LIST ((ISC_QUAD *, ISC_QUAD *, int *, int *, int *, int *));
int AgeInDaysThreshold PROTO_LIST ((ISC_QUAD *, ISC_QUAD *, int *, int *, int *, int *));
int AgeInDays PROTO_LIST ((ISC_QUAD *, ISC_QUAD *));
int Hour PROTO_LIST ((ISC_QUAD *));
int Minute PROTO_LIST ((ISC_QUAD *));
char *hourcolonmin PROTO_LIST ((ISC_QUAD *));

#ifdef __cplusplus
 }
#endif
#endif /* DATEGNCS_H_INCLUDED */
