/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   Encrypt.h
 *     Cryptographic routines declarations:
 *	 IBPassword -
 */

#ifndef ENCRYPT_H_INCLUDED
#define ENCRYPT_H_INCLUDED

#ifndef HAVE_LIBCRYPT
/* This is just a stub for libcrypt, as I do not want to deal with
 * export restrictions, laws, etc...
 */ 
/* Warning: function below is NOT AT&T compatible! */
#define crypt(k, s) NULL
#endif /* !HAVE_LIBCRYPT */

#ifdef __cplusplus
 extern "C"
 {
#endif
char *IBPassword PROTO_LIST ((char *));
#ifdef __cplusplus
 }
#endif
#endif /* ENCRYPT_H_INCLUDED */
