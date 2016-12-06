/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   Test superset UDFs with some standard calls.
 */

INPUT connect.sql;

/*
 *  String functions
 */

/* f_EncryptMD5 */
select '0cc175b9c0f1b6a831c399e269772661', f_EncryptMD5('a')
  from rdb$database;

/* f_Replace */
select 'h59llo world', f_Replace('hello world', 'e', '59') from rdb$database;

/* f_ConvertFrom33 */
select 1000, f_ConvertFrom33('WB') from rdb$database;

/* f_ConvertTo33 */
select 'WB', f_ConvertTo33(1000) from rdb$database;

/* f_ConvertFromBase */
select 42, f_ConvertFromBase('101010', 2, '01') from rdb$database;

/* f_ConvertToBase */
select '101010', f_ConvertToBase(42, 2, '01') from rdb$database;

/*
 *  Math functions
 */

/* f_Round */
select 16, f_Round(15.5) from rdb$database;

/*
 *  Date and time routines
 */

/* f_IsLeapYear */
select 0, f_IsLeapYear(cast('6/22/99' as date)) from rdb$database;

/* f_IsLeapYear */
select 0, f_IsLeapYear(cast('6/22/2000' as date)) from rdb$database;

/* f_IsLeapYear */
select 1, f_IsLeapYear(cast('6/22/96' as date)) from rdb$database;
