/*
 * FreeUDFLib - copyright (c) 1999 Gregory Deatz
 *
 *   Declarations script for FreeUDFLib superset UDFs.
 */

/* String functions */

declare external function f_EncryptMD5
  cstring(128)
  returns cstring(33) free_it
  entry_point 'EncryptMD5' module_name 'FreeUDFLib';

declare external function f_Replace
  cstring(256), cstring(16), cstring(16)
  returns cstring(256) free_it
  entry_point 'Replace' module_name 'FreeUDFLib';

/*
 * Math routines
 */

declare external function f_ConvertFrom33
  cstring(32)
  returns integer by value
  entry_point 'ConvertFrom33' module_name 'FreeUDFLib';

declare external function f_ConvertTo33
  integer
  returns cstring(32) free_it
  entry_point 'ConvertTo33' module_name 'FreeUDFLib';

declare external function f_ConvertFromBase
  cstring(32), integer, cstring(8)
  returns integer by value
  entry_point 'ConvertFromBase' module_name 'FreeUDFLib';

declare external function f_ConvertToBase
  integer, integer, cstring(8)
  returns cstring(32) free_it
  entry_point 'ConvertToBase' module_name 'FreeUDFLib';

declare external function f_Round
  double precision
  returns integer by value
  entry_point 'Round' module_name 'FreeUDFLib';

/*
 * Date and time routines
 */

declare external function f_IsLeapYear
  date
  returns integer by value
  entry_point 'IsLeapYear' module_name 'FreeUDFLib';

declare external function f_StrToTime
  cstring(12)
  returns date free_it
  entry_point 'StrToTime' module_name 'FreeUDFLib';

declare external function f_BlobUpper
  blob,
  blob
  returns parameter 2
  entry_point 'BlobUpper' module_name 'FreeUDFLib';

declare external function f_BlobReplace
  cstring(250),
  cstring(250),
  blob,
  blob
  returns parameter 4 
  entry_point 'BlobReplace' module_name 'FreeUDFLib';

declare external function f_IntegerAbs
  integer
  returns integer by value
  entry_point 'IntegerAbs' module_name 'FreeUDFLib';

