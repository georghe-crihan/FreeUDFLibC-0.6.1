/*
 * FreeUDFLibC - copyright (c) 1999 Gregory Deatz
 *
 *   BlobFncs.h
 *     Blob routines declarations:
 *	 BlobMaxSegmentLength -
 *	 BlobSegmentCount -
 *	 BlobSize -
 *	 BlobAsPChar -
 *	 BlobLeft -
 *	 BlobLine -
 *	 BlobMid -
 *	 BlobRight -
 *	 StrBlob -
 *	 BlobBinCmp -
 */

#ifndef BLOBFNCS_H_INCLUDED
#define BLOBFNCS_H_INCLUDED

#ifdef __cplusplus
 extern "C"
 {
#endif

/* Information functions */
int BlobMaxSegmentLength PROTO_LIST ((BLOBCALLBACK));
int BlobSegmentCount PROTO_LIST ((BLOBCALLBACK));
int BlobSize PROTO_LIST ((BLOBCALLBACK));
/* String-ish functions */
/* Obviously, these functions should be used with care, and only when you are
   confident that your blob is not overly large.
   Since varchars can go all the way to 32767 (I think), I find
   Blobs to be somewhat useless as containers for large strings, but
   here are some functions to do it anyways... 
*/
void internal_blob2str PROTO_LIST ((BLOBCALLBACK, unsigned char *, ISC_USHORT));
unsigned char *BlobAsPChar PROTO_LIST ((BLOBCALLBACK));
unsigned char *BlobLeft PROTO_LIST ((BLOBCALLBACK, unsigned int *));
unsigned char *BlobLine PROTO_LIST ((BLOBCALLBACK, int *));
unsigned char *BlobMid PROTO_LIST ((BLOBCALLBACK, unsigned int *, unsigned int *));
unsigned char *BlobRight PROTO_LIST ((BLOBCALLBACK, int *));
BLOBCALLBACK StrBlob PROTO_LIST ((unsigned char *, BLOBCALLBACK));
int BlobBinCmp PROTO_LIST ((BLOBCALLBACK, BLOBCALLBACK));
int BlobIComp PROTO_LIST ((BLOBCALLBACK, BLOBCALLBACK));
int DumpBlobToFile PROTO_LIST ((char *, char *, BLOBCALLBACK));
unsigned char *FindNthWordBlob PROTO_LIST ((BLOBCALLBACK, unsigned int *));
BLOBCALLBACK BlobUpper PROTO_LIST ((BLOBCALLBACK, BLOBCALLBACK));
BLOBCALLBACK BlobReplace PROTO_LIST ((unsigned char *, unsigned char *, BLOBCALLBACK, BLOBCALLBACK));
BLOBCALLBACK BlobCatStr PROTO_LIST ((BLOBCALLBACK, unsigned char *, BLOBCALLBACK));
BLOBCALLBACK BlobCat PROTO_LIST ((BLOBCALLBACK, BLOBCALLBACK, BLOBCALLBACK));

#ifdef __cplusplus
 }
#endif
#endif /* BLOBFNCS_H_INCLUDED */
