/*
 ============================================================================
 Name        : qetag.h
 Author      : Qiniu.com
 Copyright   : 2012(c) Shanghai Qiniu Information Technologies Co., Ltd.
 Description : 
 ============================================================================
 */

#include "base.h"

#ifndef QINIU_QETAG_H
#define QINIU_QETAG_H

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(1)

struct _Qiniu_Qetag_Context;
struct _Qiniu_Qetag_Block;

// 底层函数
Qiniu_Error Qiniu_Qetag_New(struct _Qiniu_Qetag_Context ** ctx, size_t fsize);
void Qiniu_Qetag_Destroy(struct _Qiniu_Qetag_Context * ctx);
Qiniu_Error Qiniu_Qetag_Next(struct _Qiniu_Qetag_Context * ctx, struct _Qiniu_Qetag_Block ** blk, size_t * blkMaxSize);
Qiniu_Error Qiniu_Qetag_Update(struct _Qiniu_Qetag_Block * blk, const char * buf, size_t size);
Qiniu_Error Qiniu_Qetag_Final(struct _Qiniu_Qetag_Context * ctx, char ** digest);

// 单线程计算 QETAG
Qiniu_Error Qiniu_Qetag_DigestFile(const char * localFile, size_t fsize, char ** digest);
Qiniu_Error Qiniu_Qetag_DigestBuffer(const char * buf, size_t fsize, char ** digest);

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* QINIU_QETAG_H */
