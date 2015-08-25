/*
 $License:
    Copyright (C) 2011 InvenSense Corporation, All Rights Reserved.
 $
 */
#ifndef STDINT_INVENSENSE_H
#define STDINT_INVENSENSE_H

#ifndef WIN32

#if defined __KERNEL__
#include <linux/types.h>
#elif defined EMPL
#include <stdint.h>
#include <string.h>
#else
#include <stdint.h>
#endif

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
typedef uint64_t __u64;

typedef int8_t __s8;
typedef int16_t __s16;
typedef int32_t __s32;
typedef int64_t __s64;

#else

#include <windows.h>

typedef signed char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

typedef int int_fast8_t;
typedef int int_fast16_t;
typedef long int_fast32_t;

typedef unsigned int uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned long uint_fast32_t;

#endif

#endif // STDINT_INVENSENSE_H
