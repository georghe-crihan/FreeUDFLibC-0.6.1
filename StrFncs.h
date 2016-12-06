/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   StrFncs.h
 *     String routines declarations:
 *	 ascii -
 *       Character - return the character based on the passed integer code.
 *       CRLF - return the carriage-return string
 *       EncryptMD5 - hash the passed string using the md5 algorithm
 *       FindWord - return the word in a string starting at some index
 *       FindWord_index - find the index of a word starting at some index
 *       Left - return the leftmost <n> characters of a string
 *       LineWrap - based on lines <n> characters long, return the ith line
 *       lTrim - trim whitespace from the left side of the string
 *       lrTrim - trim whitespace from both sides of the string
 *       Mid - return a substring of a string
 *       PadLeft - pad the left side of a string with some other string
 *       PadRight - pad the right side of a string with some other string
 *       ProperCase - proper case the passed string (every word capitalized)
 *       Replace - replace character <a> with character <b> in the string
 *       Right - return the rightmost <n> characters of a string
 *       rTrim - Trim the right side of a string of whitespace
 *       StringLength - How long is this string?
 *       StripString - Strip a string of unwanted characters
 *       SubStr - Find a substring within a string
 *       GenerateSndxIndex - return the soundex of a string
 *	FindNthWord -
 */

#ifndef STRFNCS_H_INCLUDED
#define STRFNCS_H_INCLUDED

#ifdef __cplusplus
 extern "C"
 {
#endif

int    ascii PROTO_LIST ((char *));
int    rtrim PROTO_LIST ((char *, char *));
char * Character PROTO_LIST ((int *));
char * CRLF PROTO_LIST ((void));
unsigned char * EncryptMD5 PROTO_LIST ((unsigned char *));
char * FindWord PROTO_LIST ((char *, unsigned int *));
int FindWordIndex PROTO_LIST ((char *, unsigned int *));
char * Left PROTO_LIST ((char *, unsigned int *));
char * LineWrap PROTO_LIST ((char *, int *, int *));
char * lTrim PROTO_LIST ((char *));
char * lrTrim PROTO_LIST ((char *));
char * Mid PROTO_LIST ((char *, int *, int *));
char * PadLeft PROTO_LIST ((char *, char *, int *));
char * PadRight PROTO_LIST ((char *, char *, int *));
char * ProperCase PROTO_LIST ((char *));
char * Replace PROTO_LIST ((char *, char *, char *));
void internal_replace PROTO_LIST ((char *, char *, char *, char *));
char * Right PROTO_LIST ((char *, int *));
char * rTrim PROTO_LIST ((char *));
char * GenerateSndxIndex PROTO_LIST ((char *));
int StringLength PROTO_LIST ((char *));
char * StripString PROTO_LIST ((char *, char *));
int SubStr PROTO_LIST ((char *, char *));
unsigned char * FindNthWord PROTO_LIST ((unsigned char *, unsigned int *));

void strlower PROTO_LIST ((char *, char *));
void strupper PROTO_LIST ((char *, char *));
void lrtrim PROTO_LIST ((char *, char *));
#ifdef __cplusplus
 }
#endif
#endif /* STRFNCS_H_INCLUDED */
