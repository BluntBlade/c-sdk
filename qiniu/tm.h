/*
 ============================================================================
 Name        : tm.h
 Author      : Qiniu.com
 Copyright   : 2016(c) Shanghai Qiniu Information Technologies Co., Ltd.
 Description :
 ============================================================================
 */

#ifndef QINIU_TIME_H
#define QINIU_TIME_H

#include "base.h"

#ifdef _WIN32
#include "../windows/emu_posix.h" // for type Qiniu_Posix_GetTimeOfDay
#endif

#ifdef __cplusplus
extern "C"
{
#endif

QINIU_DLLAPI extern Qiniu_Uint64 Qiniu_Tm_LocalTime(void);

#ifdef __cplusplus
}
#endif

#endif // QINIU_TIME_H
