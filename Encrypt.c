/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   Encrypt.c
 *     Cryptographic routines declarations:
 *	 IBPassword -
 */

#include "udf_glob.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <string.h>
#include "Encrypt.h"

/*
  InterBase uses classic DES - a cryptographic algorithm which is dead for some
  5 years now.
  Even more - it doesn't use password salt, which makes its protection system
  particularly week against dictionary attacks - read 'Paper Protected'.
*/
#define PASSWORD_SALT "9z" /* Even though it is defined and used, it does not */
                           /* influence the password. */

char *IBPassword(char *pInStr)
{
  char *pResStr=NULL;
  char OutStr[15];

#if (defined BSD) & (defined HAVE_LIBCRYPT)
  char CurrentCryptFormat[]="des";
#endif

  STARTDEB("IBPassword")

#if (defined BSD) & (defined HAVE_LIBCRYPT)
  strcpy(CurrentCryptFormat, crypt_get_format());
  crypt_set_format("des");
#endif

  strcpy(OutStr, crypt(pInStr, PASSWORD_SALT)); /* Even double encryption */
  strcpy(OutStr, crypt(OutStr+2, PASSWORD_SALT)); /* won't help here. */

#if (defined BSD) & (defined HAVE_LIBCRYPT)
  crypt_set_format(CurrentCryptFormat);
#endif

  pResStr=(char *) MALLOC(12+1);
  strcpy(pResStr, OutStr+2);
  ENDDEB("IBPassword")
  return pResStr;
}
