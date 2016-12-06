/* Use these 'wrappers' to put functionality into
   any of your databases. */

/* Blob functions */

declare external function f_BlobMaxSegmentLength
  blob
  returns integer by value
  entry_point 'BlobMaxSegmentLength' module_name 'FreeUDFLib';

declare external function f_BlobSegmentCount
  blob
  returns integer by value
  entry_point 'BlobSegmentCount' module_name 'FreeUDFLib';

declare external function f_BlobSize
  blob
  returns integer by value
  entry_point 'BlobSize' module_name 'FreeUDFLib';

declare external function f_BlobAsPChar
  blob
  returns cstring(1024) /* free_it */ /* or 32000 or whatever... */
  entry_point 'BlobAsPChar' module_name 'FreeUDFLib';

declare external function f_BlobLeft
  blob, integer
  returns cstring(1024) /* free_it */
  entry_point 'BlobLeft' module_name 'FreeUDFLib';

declare external function f_BlobLine
  blob, integer
  returns cstring(1024) /* free_it */
  entry_point 'BlobLine' module_name 'FreeUDFLib';

declare external function f_BlobMid
  blob, integer, integer
  returns cstring(1024) /* free_it */
  entry_point 'BlobMid' module_name 'FreeUDFLib';

declare external function f_BlobRight
  blob, integer
  returns cstring(1024) /* free_it */
  entry_point 'BlobRight' module_name 'FreeUDFLib';

declare external function f_StrBlob
  cstring(254),
  blob
  returns parameter 2
  entry_point 'StrBlob' module_name 'FreeUDFLib';

declare external function f_BlobBinCmp
  blob,
  blob
  returns integer by value 
  entry_point 'BlobBinCmp' module_name 'FreeUDFLib';

/* Date/Time routines */

declare external function f_AddMonth
  date,
  integer
  returns
  date /* free_it */
  entry_point 'AddMonth' module_name 'FreeUDFLib';

declare external function f_AddYear
  date,
  integer
  returns
  date /* free_it */
  entry_point 'AddYear' module_name 'FreeUDFLib';

declare external function f_AgeInDays
  date, date
  returns
  integer by value
  entry_point 'AgeInDays' module_name 'FreeUDFLib';

declare external function f_AgeInDaysThreshold
  date, date, integer, integer, integer, integer
  returns
  integer by value
  entry_point 'AgeInDaysThreshold' module_name 'FreeUDFLib';

declare external function f_AgeInMonths
  date, date
  returns
  integer by value
  entry_point 'AgeInMonths' module_name 'FreeUDFLib';

declare external function f_AgeInMonthsThreshold
  date, date, integer, integer, integer, integer
  returns
  integer by value
  entry_point 'AgeInMonthsThreshold' module_name 'FreeUDFLib';

declare external function f_AgeInWeeks
  date, date
  returns
  integer by value
  entry_point 'AgeInWeeks' module_name 'FreeUDFLib';

declare external function f_AgeInWeeksThreshold
  date, date, integer, integer, integer, integer
  returns
  integer by value
  entry_point 'AgeInWeeksThreshold' module_name 'FreeUDFLib';

declare external function f_CDOWLong
  date
  returns cstring(10) /* free_it */
  entry_point 'CDOWLong' module_name 'FreeUDFLib';

declare external function f_CDOWShort
  date
  returns cstring(4) /* free_it */
  entry_point 'CDOWShort' module_name 'FreeUDFLib';

declare external function f_CMonthLong
  date
  returns cstring(10) /* free_it */
  entry_point 'CMonthLong' module_name 'FreeUDFLib';

declare external function f_CMonthShort
  date
  returns cstring(4) /* free_it */
  entry_point 'CMonthShort' module_name 'FreeUDFLib';

declare external function f_DayOfMonth
  date
  returns
  integer by value
  entry_point 'DayOfMonth' module_name 'FreeUDFLib';

declare external function f_DayOfWeek
  date
  returns
  integer by value
  entry_point 'DayOfWeek' module_name 'FreeUDFLib';

declare external function f_DayOfYear
  date
  returns
  integer by value
  entry_point 'DayOfYear' module_name 'FreeUDFLib';

declare external function f_MaxDate
  date, date
  returns
  date /* free_it */
  entry_point 'MaxDate'
  module_name 'FreeUDFLib';

declare external function f_MinDate
  date, date
  returns
  date /* free_it */
  entry_point 'MinDate'
  module_name 'FreeUDFLib';

declare external function f_Month
  date
  returns
  integer by value
  entry_point 'Month' module_name 'FreeUDFLib';

declare external function f_Quarter
  date
  returns
  integer by value
  entry_point 'Quarter' module_name 'FreeUDFLib';

declare external function f_StripDate
  date
  returns
  date /* free_it */
  entry_point 'StripDate' module_name 'FreeUDFLib';

declare external function f_StripTime
  date
  returns
  date /* free_it */
  entry_point 'StripTime' module_name 'FreeUDFLib';

declare external function f_WeekOfYear
  date
  returns
  integer by value
  entry_point 'WeekOfYear' module_name 'FreeUDFLib';

declare external function f_WOY
  date
  returns
  cstring(254) /* free_it */
  entry_point 'WOY' module_name 'FreeUDFLib';

declare external function f_Year
  date
  returns
  integer by value
  entry_point 'Year' module_name 'FreeUDFLib';

declare external function f_YearOfYear
  date
  returns
  integer by value
  entry_point 'YearOfYear' module_name 'FreeUDFLib';

/* Mathematical functions */
declare external function f_DollarVal
  double precision
  returns
  cstring(254) /* free_it */
  entry_point 'DollarVal' module_name 'FreeUDFLib';

declare external function f_DoubleAbs
  double precision
  returns
  double precision by value
  entry_point 'DoubleAbs' module_name 'FreeUDFLib';

declare external function f_FixedPoint
  double precision, integer
  returns
  cstring(254) /* free_it */
  entry_point 'FixedPoint' module_name 'FreeUDFLib';

declare external function f_Modulo
  integer, integer
  returns
  integer by value
  entry_point 'Modulo' module_name 'FreeUDFLib';

declare external function f_IsDivisibleBy
  integer, integer
  returns
  integer by value
  entry_point 'IsDivisibleBy' module_name 'FreeUDFLib';

declare external function f_RoundFloat
  double precision,
  double precision
  returns
  double precision by value
  entry_point 'RoundFloat' module_name 'FreeUDFLib';

declare external function f_Truncate
  double precision
  returns integer by value
  entry_point 'Truncate' module_name 'FreeUDFLib';

/* String functions */
declare external function f_Character
  integer
  returns cstring(2) /* free_it */
  entry_point 'Character' module_name 'FreeUDFLib';

declare external function f_CRLF
  returns cstring(3) /* free_it */
  entry_point 'CRLF' module_name 'FreeUDFLib';

declare external function f_FindNthWord
  cstring(8196),
  integer
  returns cstring(254) /* free_it */
  entry_point 'FindNthWord' module_name 'FreeUDFLib';

declare external function f_FindWord
  cstring(8196),
  integer
  returns cstring(254) /* free_it */
  entry_point 'FindWord' module_name 'FreeUDFLib';

declare external function f_FindWordIndex
  cstring(8196),
  integer
  returns integer by value
  entry_point 'FindWordIndex' module_name 'FreeUDFLib';

declare external function f_Left
  cstring(254), integer
  returns
  cstring(254) /* free_it */
  entry_point 'Left' module_name 'FreeUDFLib';

declare external function f_LineWrap
  cstring(1024), integer, integer
  returns
  cstring(254) /* free_it */
  entry_point 'LineWrap' module_name 'FreeUDFLib';

declare external function f_lrTrim
  cstring(254)
  returns
  cstring(254) /* free_it */
  entry_point 'lrTrim' module_name 'FreeUDFLib';

declare external function f_BiglrTrim
  cstring(1024)
  returns
  cstring(1024) /* free_it */
  entry_point 'lrTrim' module_name 'FreeUDFLib';

declare external function f_lTrim
  cstring(254)
  returns
  cstring(254) /* free_it */
  entry_point 'lTrim' module_name 'FreeUDFLib';

declare external function f_Mid
  cstring(254), integer, integer
  returns
  cstring(254) /* free_it */
  entry_point 'Mid' module_name 'FreeUDFLib';

declare external function f_PadLeft
  cstring(254),
  cstring(10),
  integer
  returns
  cstring(254) /* free_it */
  entry_point 'PadLeft' module_name 'FreeUDFLib';

declare external function f_PadRight
  cstring(254),
  cstring(10),
  integer
  returns
  cstring(254) /* free_it */
  entry_point 'PadRight' module_name 'FreeUDFLib';

declare external function f_ProperCase
  cstring(254)
  returns
  cstring(254) /* free_it */
  entry_point 'ProperCase' module_name 'FreeUDFLib';

declare external function f_Right
  cstring(254), integer
  returns
  cstring(254) /* free_it */
  entry_point 'Right' module_name 'FreeUDFLib';

declare external function f_rTrim
  cstring(254)
  returns
  cstring(254) /* free_it */
  entry_point 'rTrim' module_name 'FreeUDFLib';

declare external function f_StringLength
  cstring(254)
  returns
  integer by value
  entry_point 'StringLength' module_name 'FreeUDFLib';

declare external function f_BigStringLength
  cstring(1024)
  returns
  integer by value
  entry_point 'StringLength' module_name 'FreeUDFLib';

declare external function f_StripString
  cstring(254),
  cstring(254)
  returns
  cstring(254) /* free_it */
  entry_point 'StripString' module_name 'FreeUDFLib';

declare external function f_SubStr
  cstring(254),
  cstring(254)
  returns
  integer by value
  entry_point 'SubStr' module_name 'FreeUDFLib';

declare external function f_BigSubStr
  cstring(1024),
  cstring(1024)
  returns
  integer by value
  entry_point 'SubStr' module_name 'FreeUDFLib';

declare external function f_IBPassword
  cstring(32)
  returns
  cstring(32) /* free_it */
  entry_point 'IBPassword' module_name 'FreeUDFLib';

/* Data formatting routines */
declare external function f_GenerateFormattedName
  cstring(256),
  cstring(64),
  cstring(32),
  cstring(2),
  cstring(80),
  cstring(32)
  returns
  cstring(256) /* free_it */
  entry_point 'GenerateFormattedName' module_name 'FreeUDFLib';

declare external function f_ValidateNameFormat
  cstring(256)
  returns
  integer by value
  entry_point 'ValidateNameFormat' module_name 'FreeUDFLib';

/* Regular expressions */
declare external function f_ValidateRegularExpression
  cstring(2048)
  returns
  integer by value
  entry_point 'ValidateRegularExpression' module_name 'FreeUDFLib';

declare external function f_ValidateStringInRE
  cstring(2048),
  cstring(2048)
  returns
  integer by value
  entry_point 'ValidateStringInRE' module_name 'FreeUDFLib';

/* Miscellaneous functions */

declare external function f_CloseDebuggerOutput
  returns integer by value
  entry_point 'CloseDebuggerOutput' module_name 'FreeUDFLib';

declare external function f_Debug
  cstring(1024)
  returns integer by value
  entry_point 'Debug' module_name 'FreeUDFLib';

declare external function f_IBTempPath
  returns
  cstring(1024) /* free_it */
  entry_point 'IBTempPath' module_name 'FreeUDFLib';

declare external function f_SetDebuggerOutput
  cstring(1024)
  returns integer by value
  entry_point 'SetDebuggerOutput' module_name 'FreeUDFLib';

declare external function f_ValidateCycleExpression
  cstring(1024),
  integer
  returns integer by value
  entry_point 'ValidateCycleExpression' module_name 'FreeUDFLib';

declare external function f_EvaluateCycleExpression
  cstring(1024),
  integer,
  date,
  date,
  double precision
  returns integer by value
  entry_point 'EvaluateCycleExpression' module_name 'FreeUDFLib';

declare external function f_EvaluateExpression
  cstring(8196), /* Expression */
  cstring(8196) /* Symbols */
  returns cstring(8196) /* free_it */ /* 'TRUE', 'FALSE' or error messages */
  entry_point 'EvaluateExpression' module_name 'FreeUDFLib';


declare external function f_GenerateSndxIndex
  cstring(256)
  returns
  cstring(6) /* free_it */
  entry_point 'GenerateSndxIndex' module_name 'FreeUDFLib';

