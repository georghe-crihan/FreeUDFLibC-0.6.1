/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   Test all UDFs with some standard calls.
 */

INPUT connect.sql;

/* Debugging functions */

select f_CloseDebuggerOutput() from rdb$database;

select f_SetDebuggerOutput('c:\junk.out') from rdb$database;

select f_Debug('A portion of junk') from rdb$database;

/* String formatting functions */

select f_GenerateFormattedName('\\ \P\\ \F\\. \M\\\L\, \\S', 'Mr', 'John', 'F', 'Kennedy', 'II') from rdb$database;

select f_ValidateNameFormat('\\ \P\\ \F\\. \M\\\L\, \\S') from rdb$database;


/* String functions */

select f_IBTempPath() from rdb$database;

/* f_Character */
select 'c', f_Character(99) from rdb$database;

/* f_CRLF */
select f_CRLF() from rdb$database;

/* f_FindWord */
select 'hello', f_FindWord('hello world', 0) from rdb$database;

/* f_FindNthWord */
select 'worldee', f_FindNthWord('hello world worldee world', 2) from rdb$database;

/* f_FindWordIndex */
select 0, f_FindWordIndex('hello   world', 5) from rdb$database;

/* f_Left */
select 'hello', f_Left('hello world', 5) from rdb$database;

/* f_LineWrap */
 select 'hello', f_LineWrap('hello world', 0, 6) from rdb$database;

/* f_lTrim */
select 'hello world', f_lTrim('  hello world') from rdb$database;

/* f_lrTrim */
select 'hello world', f_lrTrim('  hello world  ') from rdb$database;

/* f_Mid */
select 'wor', f_Mid('hello world', 6, 3) from rdb$database;

/* f_PadLeft */
select '0150', f_PadLeft('150', '0', 4) from rdb$database;

/* f_PadRight */
select '1500', f_PadRight('150', '0', 4) from rdb$database;

/* f_ProperCase */
select 'Hello World', f_ProperCase('hello world') from rdb$database;

/* f_Right */
select 'world', f_Right('hello world', 5) from rdb$database;

/* f_rTrim */
select f_rTrim('hello world  '), 'hello world' from rdb$database;

/* f_GenerateSndxIndex */
select 'H4000', f_GenerateSndxIndex('hello') from rdb$database;

/* f_StringLength */
select 11, f_StringLength('hello world') from rdb$database;

/* f_StripString */
select 'o word', f_StripString('hello world', 'hel') from rdb$database;

/* f_SubStr */
select 6, f_SubStr('hello world', 'wor') from rdb$database;

/*
 * Math routines
 */

/* f_DollarVal */
select '$15.29', f_DollarVal(15.29) from rdb$database;

/* f_DoubleAbs */
select 15, f_DoubleAbs(-15) from rdb$database;

/* f_FixedPoint */
select '15.379', f_FixedPoint(15.3789, 3) from rdb$database;

/* f_IntegerAbs */
select 15, f_IntegerAbs(-15) from rdb$database;

/* f_Modulo */
select 1, f_Modulo(3, 2) from rdb$database;

/* f_Truncate */
select 15, f_Truncate(15.5) from rdb$database;

/*
 * Date and time routines
 * 6/22/99 is a Tuesday
 */

/* f_AddMonth */
select '8/22/99', f_AddMonth(cast('6/22/99' as date), 2) from rdb$database;

/* f_AddYear */
select '6/22/97', f_AddYear(cast('6/22/99' as date), -2) from rdb$database;

/* f_AgeInMonths */
select 5, f_AgeInMonths(cast('6/22/99' as date), cast('1/1/99' as date)) from rdb$database;

/* f_AgeInWeeks */
select 1, f_AgeInWeeks(cast('6/22/99' as date), cast('6/15/99' as date)) from rdb$database;

/* f_CDOWLong */
select 'Tuesday', f_CDOWLong(cast('6/22/99' as date)) from rdb$database;

/* f_CDOWShort */
select 'Tue', f_CDOWShort(cast('6/22/99' as date)) from rdb$database;

/* f_CMonthLong */
select 'June', f_CMonthLong(cast('6/22/99' as date)) from rdb$database;

/* f_CMonthShort */
select 'Jun', f_CMonthShort(cast('6/22/99' as date)) from rdb$database;

/* f_DayOfMonth */
select 22, f_DayOfMonth(cast('6/22/99' as date)) from rdb$database;

/* f_DayOfWeek */
select 3, f_DayOfWeek(cast('6/22/99' as date)) from rdb$database;

/* f_DayOfYear */
select 173, f_DayOfYear(cast('6/22/99' as date)) from rdb$database;

/* f_MaxDate */
select '6/22/99', f_MaxDate(cast('6/22/99' as date), cast('6/22/98' as date)) from rdb$database;

/* f_MinDate */
select '6/22/98', f_MinDate(cast('6/22/99' as date), cast('6/22/98' as date)) from rdb$database;

/* f_Month */
select 6, f_Month(cast('6/22/99' as date)) from rdb$database;

/* f_Quarter */
select 2, f_Quarter(cast('6/22/99' as date)) from rdb$database;

/* f_StripDate */
select '12/31/1899', f_StripDate(cast('6/22/99' as date)) from rdb$database;

/* f_StripTime */
select '6/22/99', f_StripTime(cast('6/22/99' as date)) from rdb$database;

/* f_WeekOfYear */
select 25, f_WeekOfYear(cast('6/22/99' as date)) from rdb$database;

/* f_WOY */
select '199925', f_WOY(cast('6/22/99' as date)) from rdb$database;

/* f_Year */
select 1999, f_Year(cast('6/22/99' as date)) from rdb$database;

/* f_YearOfYear */
select 1999, f_YearOfYear(cast('6/22/99' as date)) from rdb$database;
