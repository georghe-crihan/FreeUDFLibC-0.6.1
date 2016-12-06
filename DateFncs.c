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
 *       WOY - Return the year and the week concatenated (199901)
 *       Year - What is the year?
 *       YearOfYear - Given a date, return the year as it would
 *                      be returned from the week_of_year function.
 *	AgeInDays -
 *	AgeInMonthsTreshold -
 *	AgeInWeeksTreshold -
 *	AgeInDaysTreshold -
 *	Minute -
 *	Hour -
 */

#include <time.h>
#include "udf_glob.h"
#include "DateFncs.h"

#define isleap(y) (((y) % 4) == 0 && ((y) % 100) != 0 || ((y) % 400) == 0)

static ISC_QUAD * GenIbDate PROTO_LIST ((struct tm *));
static void IWeekOfYear PROTO_LIST ((ISC_QUAD *, int *, int *));

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

#define JANUARY 0
#define FEBRUARY 1
#define MARCH 2
#define APRIL 3
#define MAY 4
#define JUNE 5
#define JULY 6
#define AUGUST 7
#define SEPTEMBER 8
#define OCTOBER 9
#define NOVEMBER 10
#define DECEMBER 11

#ifndef LOCALE_SUPPORT
static char * LONGDOW[] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
  "Saturday"
};

static char * SHORTDOW[] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri",
  "Sat"
};

static char * LONGMONTH[] = {
  "January", "February", "March", "April", "May", "June",
  "July", "August", "September", "October", "November",
  "December"
};

static char * SHORTMONTH[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
#endif

ISC_QUAD * AddMonth (ISC_QUAD *ib_date, int * months_to_add)
{
  struct tm t;
  int tm_mon, tm_year;
STARTDEB("AddMonth")  
  isc_decode_date (ib_date, &t);
  tm_mon = t.tm_mon + (*months_to_add % 12);
  tm_year = t.tm_year + (*months_to_add / 12);
  if ((*months_to_add > 0) && (tm_mon < t.tm_mon))
    tm_year++;
  else if ((*months_to_add < 0) && (tm_mon > t.tm_mon))
    tm_year--;

  /*
   * Now, check that the month day is correctly adjusted.
   * 30 days has Sep, Apr, Jun, Nov all the rest have 31 except Feb
   */
  if (tm_mon != FEBRUARY) {
    if (t.tm_mday > 30) 
      switch (tm_mon) {
      case APRIL:
      case JUNE:
      case SEPTEMBER:
      case NOVEMBER:
	t.tm_mday = 30;
      }
  }
  else if (t.tm_mday > 28)
    t.tm_mday = isleap(tm_year) ? 29 : 28;

  t.tm_mon = tm_mon;
  t.tm_year = tm_year;
ENDDEB("AddMonth")  
  return GenIbDate(&t);
}

ISC_QUAD * AddYear (ISC_QUAD *ib_date, int *years_to_add)
{
  struct tm t;
STARTDEB("AddYear")  
  isc_decode_date(ib_date, &t);
  t.tm_year += *years_to_add;
  
  /*
   * If it's February, and the day of the month is 29, and
   * it's *not* a leap year, then adjust the day
   */
  if (t.tm_mon == FEBRUARY && t.tm_mday > 28 && !isleap(t.tm_year))
    t.tm_mday = 28;
ENDDEB("AddYear")  
  return GenIbDate(&t);
}

int AgeInMonths (ISC_QUAD *ib_date, ISC_QUAD *ib_ref_date)
{
  struct tm t, t_ref;
STARTDEB("AgeInMonths")  
  isc_decode_date(ib_date, &t);
  isc_decode_date(ib_ref_date, &t_ref);
ENDDEB("AgeInMonths")  
  return ((t.tm_year - t_ref.tm_year) * 12) + t.tm_mon - t_ref.tm_mon;
}

/*
 * Bases it's computation on the fact that they year has
 * an *average* of 52 weeks. So this is most certainly an
 * approximate function, but I'm quite confident that it is
 * a reasonably precise function.
 */
int AgeInWeeks (ISC_QUAD *ib_date, ISC_QUAD *ib_ref_date)
{
  struct tm t, t_ref;
STARTDEB("AgeInWeeks")  
  isc_decode_date(ib_date, &t);
  isc_decode_date(ib_ref_date, &t_ref);
ENDDEB("AgeInWeeks")  
  return
    ((t.tm_year - t_ref.tm_year) * 52) +
    (((t.tm_yday - t.tm_wday) -
      (t_ref.tm_yday - t_ref.tm_wday)) / 7);
}

char * CDOWLong (ISC_QUAD *ib_date)
{
  struct tm t;
  char * result = (char *) MALLOC (LOC_STRING_SIZE);
STARTDEB("CDOWLong")  
SETLOCALE
  isc_decode_date(ib_date, &t);
#ifndef LOCALE_SUPPORT 
  sprintf(result, "%s", LONGDOW[t.tm_wday]);
#else
  strftime(result, LOC_STRING_SIZE, "%A", &t);
#endif
ENDDEB("CDOWLong")  
  return result;
}

char * CDOWShort (ISC_QUAD *ib_date)
{
  struct tm t;
  char * result = (char *) MALLOC (LOC_STRING_SIZE);
STARTDEB("CDOWShort")
SETLOCALE  
  isc_decode_date(ib_date, &t);
#ifndef LOCALE_SUPPORT
  sprintf(result, "%s", SHORTDOW[t.tm_wday]);
#else
  strftime(result, LOC_STRING_SIZE, "%a", &t);
#endif
ENDDEB("CDOWShort")  
  return (char *) result;
}

char * CMonthLong (ISC_QUAD *ib_date)
{
  struct tm t;
  char * result = (char *) MALLOC (LOC_STRING_SIZE);
STARTDEB("CMonthLong")
SETLOCALE  
  isc_decode_date(ib_date, &t);
#ifndef LOCALE_SUPPORT
  sprintf(result, "%s", LONGMONTH[t.tm_mon]);
#else
  strftime(result, LOC_STRING_SIZE, "%B", &t);
#endif
ENDDEB("CMonthLong")  
  return (char *) result;
}

char * CMonthShort (ISC_QUAD *ib_date)
{
  struct tm t;
  char * result = (char *) MALLOC (LOC_STRING_SIZE);
SETLOCALE
STARTDEB("CMonthShort")  
  isc_decode_date(ib_date, &t);
#ifndef LOCALE_SUPPORT
  sprintf(result, "%s", SHORTMONTH[t.tm_mon]);
#else
  strftime(result, LOC_STRING_SIZE, "%b", &t);
#endif
ENDDEB("CMonthShort")  
  return (char *) result;
}

int DayOfMonth (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("DayOfMonth")
  isc_decode_date(ib_date, &t);
ENDDEB("DayOfMonth")
  return t.tm_mday;
}

int DayOfWeek (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("DayOfMonth")
  isc_decode_date(ib_date, &t);
ENDDEB("DayOfMonth")
  return t.tm_wday + 1;
}

int DayOfYear (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("DayOfYear")
  isc_decode_date(ib_date, &t);
ENDDEB("DayOfYear")
  return t.tm_yday + 1;
}

int IsLeapYear (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("IsLeapYear")
  isc_decode_date(ib_date, &t);
ENDDEB("IsLeapYear")
  return isleap(t.tm_year);
}

ISC_QUAD * MaxDate (ISC_QUAD *ib_date_1, ISC_QUAD *ib_date_2)
{
  ISC_QUAD * result = (ISC_QUAD *) MALLOC(sizeof(ISC_QUAD));
STARTDEB("MaxDate") 
  if ((ib_date_1->isc_quad_high > ib_date_2->isc_quad_high) ||
      ((ib_date_1->isc_quad_high == ib_date_2->isc_quad_high) &&
       (ib_date_1->isc_quad_low > ib_date_2->isc_quad_low)))
    *result = *ib_date_1;
  else
    *result = *ib_date_2;
ENDDEB("MaxDate")
  return result;
}

ISC_QUAD * MinDate (ISC_QUAD *ib_date_1, ISC_QUAD *ib_date_2)
{
  ISC_QUAD * result = (ISC_QUAD *) MALLOC(sizeof(ISC_QUAD));
STARTDEB("MinDate")  
  if ((ib_date_1->isc_quad_high < ib_date_2->isc_quad_high) ||
      ((ib_date_1->isc_quad_high == ib_date_2->isc_quad_high) &&
       (ib_date_1->isc_quad_low < ib_date_2->isc_quad_low)))
    *result = *ib_date_1;
  else
    *result = *ib_date_2;
ENDDEB("MinDate")
  return result;
}

int Month (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("Month")
  isc_decode_date(ib_date, &t);
ENDDEB("Month")
  return t.tm_mon + 1;
}

int Quarter (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("Quarter")
  isc_decode_date(ib_date, &t);
ENDDEB("Quarter")
  return (t.tm_mon / 4) + 1;
}

ISC_QUAD * StripDate (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("StripDate")
  isc_decode_date(ib_date, &t);

  t.tm_mday = 0;
  t.tm_mon = 0;
  t.tm_year = 0;
  t.tm_wday = 0;
  t.tm_yday = 0;
ENDDEB("StripDate")
  return GenIbDate(&t);
}

ISC_QUAD * StripTime (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("StripTime")
  isc_decode_date(ib_date, &t);

  t.tm_sec = 0;
  t.tm_min = 0;
  t.tm_hour = 0;
  t.tm_isdst = 0;
ENDDEB("StripTime")
  return GenIbDate(&t);
}

ISC_QUAD * StrToTime (char *sz)
{
  struct tm t;
  int len = strlen(sz);
  int h, m, s, am;
  ISC_QUAD * result = (ISC_QUAD *) MALLOC(sizeof(ISC_QUAD));

STARTDEB("StrToTime")
  h = strtol(sz, NULL, 10);
  m = len > 3 ? strtol(&sz[3], NULL, 10) : 0;
  s = len > 6 ? strtol(&sz[6], NULL, 10) : 0;
  am = len > 9 ? strcmp(&sz[9], "PM") : 0;
  
  t.tm_sec = s;
  t.tm_min = m;
  if (am)
    t.tm_hour = (h + ((h == 12) ? 12 : 0)) % 24;
  else
    t.tm_hour = (h + ((h == 12) ? 0 : 12)) % 24;
  t.tm_mday = 0;
  t.tm_mon = 0;
  t.tm_year = 0;
  t.tm_wday = 0;
  t.tm_yday = 0;
  t.tm_isdst = 0;

  isc_encode_date(&t, result);
ENDDEB("StrToTime")
  return result;
}

int WeekOfYear (ISC_QUAD *ib_date)
{
  int week, year;
STARTDEB("WeekOfYear")
  IWeekOfYear(ib_date, &week, &year);
ENDDEB("WeekOfYear")
  return week;
}

char * WOY (ISC_QUAD *ib_date)
{
  char * result = (char *) MALLOC (LOC_STRING_SIZE + 1);
  int week, year;
STARTDEB("WOY")
  IWeekOfYear(ib_date, &week, &year);
  
  sprintf(result, "%d%2d", year, week);
ENDDEB("WOY")
  return result;
}

int Year (ISC_QUAD *ib_date)
{
  struct tm t;
STARTDEB("Year")
  isc_decode_date(ib_date, &t);
ENDDEB("Year")
  return t.tm_year + 1900;
}

int YearOfYear (ISC_QUAD *ib_date)
{
  int week, year;
STARTDEB("YearOfYear")
  IWeekOfYear(ib_date, &week, &year);
ENDDEB("YearOfYear")
  return year;
}

static ISC_QUAD * GenIbDate (struct tm *t)
{
  ISC_QUAD *ibd = (ISC_QUAD *) MALLOC (sizeof(ISC_QUAD));
STARTDEB("GenIbDate")
  isc_encode_date(t, ibd);
ENDDEB("GenIbDate")
  return ibd;
}


/*
 * Simple observations:
 *   7 * 52 = 364 = a common year + 1 (or 2 if it is a leap year)
 *   There are 365-366 days in the year.
 *   A week is Sunday - Saturday.
 * So....
 *
 * if the year begins on a Sun - Wed, then Jan 1 is considered to begin
 *   Week #1.
 * if the year begins on Thu - Sat, then Jan 1 is considered to still
 *   be part of the last week of the previous year.
 * if the year begins on a Wed OR
 *    the year begins on a Tue or Wed and its a leap year THEN
 *   the year has 53 weeks in it...
 */
static void IWeekOfYear (ISC_QUAD *ib_date, int *week, int *year)
{
  struct tm t;
  int dow_ybeg, yr, wk;
STARTDEB("IWeekOfYear")  
  isc_decode_date(ib_date, &t);

  yr = t.tm_year + 1900;

  /* When did the year begin? */
  dow_ybeg = (t.tm_yday < t.tm_wday)
    ? (t.tm_wday - t.tm_yday) % 7
    : (t.tm_yday - t.tm_wday) % 7;

  /* Get the Sunday beginning this week. */
  wk = (t.tm_yday - t.tm_wday);
  /*
   * If the Sunday beginning this week was last year, then
   *   if this year begins on a Wednesday or previous, then
   *   this is most certainly the first week of the year.
   *   if this year begins on a thursday or
   *     last year was a leap year and this year begins on a friday, then
   *   this week is 53 of last year.
   *   Otherwise this week is 52 of last year.
   */
  if (wk < 0) {
    if (dow_ybeg <= WEDNESDAY)
      wk = 1;
    else if ((dow_ybeg == THURSDAY) ||
	     (isleap((yr - 1)) && (dow_ybeg == FRIDAY))) {
      wk = 53;
      yr--;
    } else {
      wk = 52;
      yr--;
    }
  }
  else {
    /* If the Sunday beginning this week falls in this year
     *   if the year begins on a Sun, Mon, Tue or Wed then
     *     This week # is (Week + 7) div 7
     *   otherwise this week is
     *     Week div 7 + 1.
     *   if the week is > 52 then
     *     if this year began on a wed or this year is leap year and it
     *       began on a tuesday, then set the week to 53.
     *     otherwise set the week to 1 of *next* year.
     */
    if (dow_ybeg <= WEDNESDAY)
      wk = (wk + 7 + dow_ybeg) / 7;
    else
      wk = (wk / 7) + 1;
    if (wk > 52) {
      if ((dow_ybeg == WEDNESDAY) ||
	  (isleap(yr) && (dow_ybeg == TUESDAY)))
	wk = 53;
      else {
        wk = 1;
	yr++;
      }
    }
  }
  *week = wk;
  *year = yr;
ENDDEB("IWeekOfYear")
}

int AgeInDays(ISC_QUAD *ib_date, ISC_QUAD *ib_date_reference)
{
  struct tm tm_date, tm_date_reference;
  int days=0, i;

  STARTDEB("AgeInDays")
  isc_decode_date(ib_date, &tm_date);
  isc_decode_date(ib_date_reference, &tm_date_reference);
  for(i=tm_date_reference.tm_year+1; i<tm_date.tm_year; i++) {
     days+=365;
     if(isleap(i+1900)) days++;
     }
  i=365;
  if (isleap(tm_date_reference.tm_year)) i++;
  days+=i-tm_date_reference.tm_yday+1;
  days+=tm_date.tm_yday+1;

  ENDDEB("AgeInDays")

  return days;
}


int AgeInMonthsThreshold(ISC_QUAD *ib_date, ISC_QUAD *ib_date_reference,
                              int *Min, int *UseMin, int *Max, int *UseMax)
{
int result;

  STARTDEB("AgeInMonthsThreshold")
  result = AgeInMonths(ib_date, ib_date_reference);
  if ((*UseMin == 1) && (result < *Min))
    result = *Min;
  else if ((*UseMax == 1) && (result > *Max))
    result = *Max;
  ENDDEB("AgeInMonthsThreshold")

return result;
}


int AgeInWeeksThreshold(ISC_QUAD *ib_date, ISC_QUAD *ib_date_reference,
                             int *Min, int *UseMin, int *Max, int *UseMax)
{
int result;

  STARTDEB("AgeInWeeksThreshold")
  result = AgeInWeeks(ib_date, ib_date_reference);
  if ((*UseMin == 1) && (result < *Min))
    result = *Min;
  else if ((*UseMax == 1) && (result > *Max))
    result = *Max;
  ENDDEB("AgeInWeeksThreshold")
  return result;
}


int AgeInDaysThreshold(ISC_QUAD *ib_date, ISC_QUAD *ib_date_reference,
                            int *Min, int *UseMin, int *Max, int *UseMax)
{
int result;

  STARTDEB("AgeInDaysThreshold")
  result = AgeInDays(ib_date, ib_date_reference);
  if ((*UseMin == 1) && (result < *Min))
    result = *Min;
  else if ((*UseMax == 1) && (result > *Max))
    result = *Max;
  ENDDEB("AgeInDaysThreshold")
  return result;
}

int Minute(ISC_QUAD *ib_time)
{
struct tm t_time;
STARTDEB("Minute")
  isc_decode_date(ib_time, &t_time);
ENDDEB("Minute")
  return t_time.tm_min+1;
}

int Hour(ISC_QUAD *ib_time)
{
struct tm t_time;
STARTDEB("Hour")
  isc_decode_date(ib_time, &t_time);
ENDDEB("Hour")
  return t_time.tm_hour+1;
}

char *hourcolonmin(ISC_QUAD *t)
{
struct tm m;
char *r;

STARTDEB("hourcolonmin")
r=(char *)MALLOC(6);
isc_decode_date(t, &m);
sprintf(r, "%2d:%2d", m.tm_hour, m.tm_min);
ENDDEB("hourcolonmin")
return r;
}
