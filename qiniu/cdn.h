/*
 ============================================================================
 Name        : cdn.h
 Author      : Qiniu.com
 Copyright   : 2017(c) Shanghai Qiniu Information Technologies Co., Ltd.
 Description : 
 ============================================================================
 */

#ifndef QINIU_CDN_H
#define QINIU_CDN_H

#include "base.h"
#include "macro.h"

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(1)

QINIU_DLLAPI extern char * Qiniu_Cdn_MakeDownloadUrlWithDeadline(const char * key, const char * url, Qiniu_Uint64 deadline);

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif // QINIU_CDN_H
