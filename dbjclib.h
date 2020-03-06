#ifndef _DBJ_CLIB_API_
#define _DBJ_CLIB_API_
/*
Copyright 2017,2018,2019,2020 by dbj@dbj.org

before hotly disagreeing on some details, please read
 https://matt.sh/howto-c
*/
#include <stdint.h>
#include <stdlib.h>

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
/*
--------------------------------------------------------------------------------------------
*/
#define dbj_clib_MAJOR  0
#define dbj_clib_MINOR  4
#define dbj_clib_PATCH  9

#define dbj_clib_VERNUM (dbj_clib_MAJOR * 10000 + dbj_clib_MINOR * 100 + dbj_clib_PATCH)

#define dbj_clib_VERSION  dbj_clib_STRINGIFY(dbj_clib_MAJOR) "." dbj_clib_STRINGIFY(dbj_clib_MINOR) "." dbj_clib_STRINGIFY(dbj_clib_PATCH)

#define dbj_clib_STRINGIFY(  x )  dbj_clib_STRINGIFY_( x )
#define dbj_clib_STRINGIFY_( x )  #x

#ifdef STRINGIFY
#error name 'STRINGIFY' already found, please rename in here or at your location if possible
#else
#define STRINGIFY dbj_clib_STRINGIFY
#endif
/*
--------------------------------------------------------------------------------------------
 What is perhaps unique about dbjCLIB? 

 1. using strong types -- https://dbj.org/c-strong-duck/
 2. using valstat -- https://github.com/DBJDBJ/dbj-valstat
*/
#ifdef STRONG
#error STRONG already defined?
#else
#define dbj_clib_STRONG(N,T) struct N final { T v; }
#define STRONG dbj_clib_STRONG
#endif

#include "valstat_interop.h"

/*
--------------------------------------------------------------------------------------------
Note: it is a good idea, while inside c++ to have this inside a namespace
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "dbj_str_trim/dbj_string_trim.h"
#include "dbj_fb_string/dbj_front_back_string.h"
#include "dbj_sll/dbj_sll.h" 
#include "dbj_string_list/dbj_string_list.h"

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifndef DBJ_CLIB_INTERNAL_TESTING_
/*
for this to work add the dbjlib folder to addional directories 
for linker, in project properties
*/
#ifdef NDEBUG
#if defined( _WIN32 ) 
#pragma comment(lib, "lib/dbjclib_x86_Release.lib")
#else // _WIN64
#pragma comment(lib, "lib/dbjclib_x64_Release.lib")
#endif
#else /* DEBUG */
#if defined( _WIN64 ) 
#pragma comment(lib, "lib/dbjclib_x64_Debug.lib")
#else // _WIN32
#pragma comment(lib, "lib/dbjclib_86_Debug.lib")
#endif
#endif

#endif // 0

#endif // ! _DBJ_CLIB_API_

