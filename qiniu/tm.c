/*
 ============================================================================
 Name        : tm.c
 Author      : Qiniu.com
 Copyright   : 2016(c) Shanghai Qiniu Information Technologies Co., Ltd.
 Description :
 ============================================================================
 */

#include "tm.h"
#include <sys/time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32

QINIU_DLLAPI extern Qiniu_Uint64 Qiniu_Tm_LocalTime(void)
{
	return Qiniu_Posix_GetTimeOfDay();
} // Qiniu

#else

QINIU_DLLAPI extern Qiniu_Uint64 Qiniu_Tm_LocalTime(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec;
} // Qiniu_Tm_LocalTime

#endif

#ifdef __cplusplus
}
#endif

