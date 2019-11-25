#pragma once
/*
Copyright 2017,2018,2019,2020 by dbj@dbj.org

before hotly disagreeing and trying to reach me, please read
 https://matt.sh/howto-c
*/

#ifndef _DBJ_CLIB_CORE_INC_
#define _DBJ_CLIB_CORE_INC_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

#ifdef __clang__
/*
http://clang.llvm.org/docs/UsersManual.html#controlling-diagnostics-in-system-headers
*/
#pragma clang system_header
#endif /* __clang__ */

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif


#if defined (_MSC_VER)
#define PLATFORM "Windows"
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
// we do the above since we use MSVC UCRT *from* the clang c code
#elif defined (__linux)
#define PLATFORM "Linux"
#else
#error Unknown platform?
#endif


#if ! defined( __cplusplus )

#ifndef _MSC_VER
# if !defined(__STDC_VERSION__) ||  (__STDC_VERSION__ < 199901L)
#error    Your compiler is not conforming to C99
#error    this requires the macro __STDC_VERSION__ to be set to the
#error    indicated value (or larger).
#error    NOTE: For C11, __STDC_VERSION__ == 201112L
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
#define DBJ_REMOVE(expr) (void)(sizeof(expr))
#define DBJ_UNUSED DBJ_REMOVE

#	if ! defined(_MSC_EXTENSIONS)
#error Need MSC EXTENSIONS DEFINED
#	endif

#if !defined( _WIN32 ) && !defined(_WIN64)
#error Need _WIN32 or _WIN64
#endif

#endif
/*
Note: while inside c++ all is in the dbj::clib namespace
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef uchar_t
	typedef uint8_t 	uchar_t;
#endif  uchar_t

#ifndef size_t
	typedef uint_fast64_t  size_t;
#endif


#define DBJ_MALLOC(N) calloc(1,N)

#define DBJ_FREE(p) do{  if (p) free((void *)p); p = 0;}while(0)

	/*
	NOTE: must place NULL as the last arg!
		  max args is 255
	*/
	void free_free_set_them_free(void* vp, ...);

#define DBJ_MULTI_FREE(...) free_free_set_them_free((void *)__VA_ARGS__, NULL)

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // ! _DBJ_CLIB_CORE_INC_

