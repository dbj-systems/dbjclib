#pragma once
/*
Copyright 2017,2018,2019,2020 by dbj@dbj.org

before hotly disagreeing and trying to reach me, please read
 https://matt.sh/howto-c
*/

#ifndef _DBJ_CLIB_CORE_INC_
#define _DBJ_CLIB_CORE_INC_

/* -------------------------------------------------------------------------------------------- */
#undef DBJ_CLIB_BEGIN
#undef DBJ_CLIB_END

#ifdef __cplusplus
#define DBJ_CLIB_BEGIN namespace dbjclib { extern "C" {
#define DBJ_CLIB_END  } }
#else
#define DBJ_CLIB_BEGIN
#define DBJ_CLIB_END
#endif
/* -------------------------------------------------------------------------------------------- */

#if !defined( _WIN32 ) && !defined(_WIN64)
#error __FILE__ needs _WIN32 or _WIN64
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

#include "valstat_interop.h"

#ifdef __clang__
#pragma clang system_header

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif // __clang__


#if defined (_WIN32)
#define PLATFORM "Windows"
#elif defined (_WIN64)
#define PLATFORM "Windows64"
#elif defined (__linux__)
#define PLATFORM "Linux"
#else
#error Unknown platform?
#endif


#ifdef __STDC_VERSION__
# if (__STDC_VERSION__ < 199901L)
#error    Your compiler is not conforming to C99
#endif
# if (__STDC_VERSION__ < 201112L)
#error    Your compiler is not conforming to C11
#endif
#endif

#ifdef _MSC_VER
#include <crtdbg.h>
#define DBJ_ASSERT _ASSERTE
#else
#include <assert.h>
#define DBJ_ASSERT assert
#endif

/* use this to remove unused code */
/* this verion does not evaluate the expression at runtime or compile time even */
#undef DBJ_UNUSED

#undef DBJ_UNUSED
# define DBJ_UNUSED(...) ((void)(0))

#ifdef __clang__
#	if ! defined(_MSC_EXTENSIONS)
#error Need MSC EXTENSIONS DEFINED
#	endif
#endif // __clang__


/*
Note: while inside c++ all is in the dbj::clib namespace
*/
DBJ_CLIB_BEGIN

#ifndef uchar_t
typedef uint8_t 	uchar_t;
#endif  // uchar_t

#ifndef size_t
#if defined(_WIN64)
typedef uint_fast64_t  size_t;
#elif defined(_WIN32)
typedef uint_fast32_t  size_t;
#else
/* nothing */
#endif
#endif


#undef DBJ_MALLOC
#define DBJ_MALLOC(T) (T*)calloc(1, sizeof(T))

#undef DBJ_CALLOC
#define DBJ_CALLOC(T,S) (T*)calloc(S, sizeof(T))

#ifdef _MSC_VER
// standard is: extern void* alloca(unsigned long long);
// MSFT is
// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/malloca?view=vs-2019
#define DBJ_STACK_ALLOC(S,T) (T*)_malloca(S * sizeof(T))
#else
#error compiles only with MSVC or clang packaged with VStudio
#endif


#define DBJ_FREE(p) do{  if (p) free((void *)p); p = 0;}while(0)

/*
NOTE: must place NULL as the last arg!
	  max args is 255
*/
static inline void free_free_set_them_free(void* vp, ...)
{
	const size_t max_args = 255; size_t arg_count = 0;
	va_list marker;
	va_start(marker, vp); /* Initialize variable arguments. */
	while (vp != NULL)
	{
		free(vp);
		vp = NULL;
		vp = va_arg(marker, void*);
		/* feeble attempt to make it safer  */
		if (++arg_count == max_args) break;
	}
	va_end(marker);   /* Reset variable argument list. */
}

/* yes that is a NULL as the last arg */
#define DBJ_MULTI_FREE(...) free_free_set_them_free((void *)__VA_ARGS__, NULL)



//#ifdef __clang__
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wincompatible-pointer-types"
//#pragma clang diagnostic ignored "-Wsizeof-array-argument"
//#endif // __clang__

//#ifdef __clang__
//#pragma clang diagnostic pop
//#endif

#endif // ! _DBJ_CLIB_CORE_INC_

