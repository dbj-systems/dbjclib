#pragma once
/*
Copyright 2017,2018,2019,2020 by dbj@dbj.org

before hotly disagreeing and trying to reach me, please read
 https://matt.sh/howto-c
*/

#ifndef _DBJ_CLIB_CORE_INC_
#define _DBJ_CLIB_CORE_INC_

#if !defined( _WIN32 ) && !defined(_WIN64)
#error Need _WIN32 or _WIN64
#endif

#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include "valstat_interop.h"

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


#if defined (_WIN32)
#define PLATFORM "Windows"
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#elif defined (_WIN64)
#define PLATFORM "Windows64"
// we do the above since we use MSVC UCRT *from* the clang c code
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
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wincompatible-pointer-types"
	#pragma clang diagnostic ignored "-Wsizeof-array-argument"
#endif // __clang__
// #define DBJ_UNUSED(expr) (void)((void*)(expr))

#if defined(__cplusplus)
# define DBJ_UNUSED(x)
#else
# define DBJ_UNUSED(x) (void)(x)
#endif

#ifdef __clang__
#	if ! defined(_MSC_EXTENSIONS)
#error Need MSC EXTENSIONS DEFINED
#	endif
#endif // __clang__

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
/* 
----------------------------------------------------------------
c 2darray is organize in row-major-order
https://en.wikipedia.org/wiki/Row-_and_column-major_order

reminder on C 2d array orinteering: matrix[width][height];
this is pure non VLA version 

1. always draw a diagram if in doubt what and why is bellow 
2. arr[i][j] is same as *(*(arr+i)+j)

WARNIG! _malloca might resort to heap! _freea() should be used to free
        the result of _malloca
*/

	/**/
#define DBJ_STACKMAT_ALLOX(height, width, type, name ) \
type ** name ; \
do {\
if ( (size_t)( height * width * sizeof(type) ) > (size_t)_ALLOCA_S_THRESHOLD ) \
{ \
	perror("too greedy for stack"); \
	exit( EXIT_FAILURE ) ; \
} \
name = (type**)_malloca(height * sizeof(type *)) ; \
assert(name); \
	size_t k; \
     for (k = 0; k < height; ++k) \
       name[k] = (type*)_malloca(width * sizeof(type)); \
}\
 while(0)

#define DBJ_FREE(p) do{  if (p) free((void *)p); p = 0;}while(0)

	/*
	NOTE: must place NULL as the last arg!
		  max args is 255
	*/
	void free_free_set_them_free(void* vp, ...);
/* yes that is a NULL as the last arg */
#define DBJ_MULTI_FREE(...) free_free_set_them_free((void *)__VA_ARGS__, NULL)

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

/*
---------------------------------------------------------------------------
for 2d arrays we do not use pointer pointer and static allocator
*/
#define DBJ_MATRIX(width,height,type, name)\
typedef struct matrix_##name {\
	int width; \
	int height;\
	type * data;\
} matrix_##name ;

#endif // ! _DBJ_CLIB_CORE_INC_

