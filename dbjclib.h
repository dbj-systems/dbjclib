#ifndef _DBJ_CLIB_API_
#define _DBJ_CLIB_API_
/*
Copyright 2017,2018,2019,2020,2021 by dbj@dbj.org

before hotly disagreeing on some details, please read
 https://matt.sh/howto-c

 --------------------------------------------------------------------------------------------
 What is perhaps unique about dbjCLIB?

 1. using strong types -- https://dbj.org/c-strong-duck/
 2. using valstat -- https://github.com/DBJDBJ/dbj-valstat
*/

#include <assert.h>
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
#undef DBJ_CLIB_BEGIN
#undef DBJ_CLIB_END

#ifdef __cplusplus
#define DBJ_CLIB_BEGIN namespace dbjclib { extern "C" {
#define DBJ_CLIB_END  } }
#else
#define DBJ_CLIB_BEGIN
#define DBJ_CLIB_END
#endif
/*
--------------------------------------------------------------------------------------------
*/
#define dbj_clib_MAJOR  0
#define dbj_clib_MINOR  5
#define dbj_clib_PATCH  0

#define dbj_clib_VERNUM (dbj_clib_MAJOR * 10000 + dbj_clib_MINOR * 100 + dbj_clib_PATCH)

#define dbj_clib_VERSION  dbj_clib_STRINGIFY(dbj_clib_MAJOR) "." dbj_clib_STRINGIFY(dbj_clib_MINOR) "." dbj_clib_STRINGIFY(dbj_clib_PATCH)

#define dbj_clib_STRINGIFY(  x )  dbj_clib_STRINGIFY_( x )
#define dbj_clib_STRINGIFY_( x )  #x

#ifdef STRINGIFY
#error name 'STRINGIFY' already found, please rename in here or at your location if possible
#else
#define STRINGIFY dbj_clib_STRINGIFY
#endif

//
// header concetrator is a bad idea
// 
//#include "dbj_str_trim/dbj_string_trim.h"
//#include "dbj_chr_rng/dbj_chr_rng.h"
//#include "dbj_hsl/dbj_hsl.h"
//#include "dbj_string_list/dbj_string_list.h"

/*
for this to work add the dbjlib folder to addional directories
for linker, in project properties
*/
// #ifdef NDEBUG
// #if defined( _WIN32 )
// #pragma comment(lib, "dbjclib_x86_Release.lib")
// #else // _WIN64
// #pragma comment(lib, "dbjclib_x64_Release.lib")
// #endif
// #else /* DEBUG */
// #if defined( _WIN64 ) 
// #pragma comment(lib, "dbjclib_x64_Debug.lib")
// #else // _WIN32
// #pragma comment(lib, "dbjclib_86_Debug.lib")
// #endif
// #endif



#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // ! _DBJ_CLIB_API_

