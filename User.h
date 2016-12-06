/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   User.h
 *     User maintenance routines declarations:
 *	UserMaintenance -
 */
#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#define UM_ERROR			100001
#define UM_CANNOT_DETERMINE_ACTION	100002
#define UM_CANNOT_SET_ENV_ISC_USERNAME	100003
#define UM_CANNOT_SET_ENV_ISC_PASSWORD	100004
#define UM_SYSUSER_NOT_VALID		100005
#define UM_SYSPASSWORD_NOT_VALID	100006
#define UM_CANNOT_EXECUTE_PROCESS	100007

#ifdef __cplusplus
 extern "C"
 {
#endif

int UserMaintenance PROTO_LIST ((char *, char *, char *, char *, char *));

#ifdef __cplusplus
 }
#endif
#endif /* USER_H_INCLUDED */
