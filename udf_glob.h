/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   udf_glob.h
 */
#ifndef UDF_GLOB_H_INCLUDED
#define UDF_GLOB_H_INCLUDED

#include "config.h"
#include "ibase.h"

#ifdef HAVE_LIBIB_UTIL
#include <ib_util.h>
#else
#include <stdlib.h>
#endif

#ifndef PROTO_LIST
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif
#endif

#ifdef __cplusplus
 extern "C"
 {

#endif

#ifndef HAVE_SETENV
int setenv PROTO_LIST ((const char *, const char *, int));
#endif

#ifndef HAVE_STRERROR
char *strerror PROTO_LIST ((int));
#endif

#ifndef HAVE_STRLCPY
size_t strlcpy PROTO_LIST ((char *, const char *, size_t));
#endif

#ifndef HAVE_GMTIME_R
#include <time.h>
struct tm * gmtime_r PROTO_LIST ((const time_t *, struct tm *)); 
#endif

#ifdef __cplusplus
 }
#endif

#include "Debug.h"

#define UDF_SUCCESS 0
#define UDF_FAILURE 1

#define LOC_STRING_SIZE 256
#define BLOB_STRING_SIZE 1024 /* Blobs are large */
#define MAX_PATH_LEN 1024

#ifdef __cplusplus
 extern "C"
 {
#endif
#ifdef LOCALE_SUPPORT
#include <locale.h>
#define SETLOCALE setlocale(LC_ALL,LOCALE_SUPPORT);
#else
#define SETLOCALE
#endif
#ifdef __cplusplus
 }
#endif

/* Fix up for older interbases */
#ifndef ISC_USHORT
#define ISC_USHORT unsigned short
#endif

#ifndef HAVE_BLOBCALLBACK
typedef struct blobcallback {
    short (ISC_FAR *blob_get_segment)
                (void ISC_FAR* hnd, unsigned char* buffer, ISC_USHORT buf_size,
ISC_USHORT* result_len);
    void                ISC_FAR *blob_handle;
    ISC_LONG    blob_number_segments;
    ISC_LONG    blob_max_segment;
    ISC_LONG    blob_total_length;
    void (ISC_FAR *blob_put_segment)
                (void ISC_FAR* hnd, unsigned char* buffer, ISC_USHORT buf_size);
    ISC_LONG (ISC_FAR *blob_lseek)
                (void ISC_FAR* hnd, ISC_USHORT mode, ISC_LONG offset);
} ISC_FAR *BLOBCALLBACK;  
#endif

#endif /* UDF_GLOB_H_INCLUDED */
