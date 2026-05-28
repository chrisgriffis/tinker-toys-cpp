/***
*stdint.h - WG14/N843 C99 Standard, Section 7.18
*
*
*Purpose:
*       This header is part of the ANSI C99 standard library. It
*       describes a number of useful integer types.
*       This file is not part of the original Visual C++ 6.0 distribution,
*       which preceded the C99 standard.
* 
*       Currently, this file contains only the exact-width integer types
*       and constants (see C99 Section 7.18.1.1)
*
*       [Public]
*
****/

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _INC_STDINT
#define _INC_STDINT

#if     !defined(_WIN32) && !defined(_MAC)
#error ERROR: Only Mac or Win32 targets supported!
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
typedef signed   __int8      int8_t;
typedef signed   __int16     int16_t;
//typedef signed   long      int32_t;
typedef unsigned __int8      uint8_t;
typedef unsigned __int16     uint16_t;
//typedef unsigned long      uint32_t;

typedef signed   __int32      int32_t;
typedef unsigned __int32      uint32_t;
typedef signed   __int64      int64_t;
typedef unsigned __int64      uint64_t;

#ifdef  __cplusplus
}
#endif

#endif  /* _INC_STDINT */
