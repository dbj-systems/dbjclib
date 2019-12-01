#ifndef _DBJ_CLIB_API_
#define _DBJ_CLIB_API_
/*
Copyright 2017,2018,2019,2020 by dbj@dbj.org

before hotly disagreeing onsome details, and trying to reach me, please read
 https://matt.sh/howto-c
*/


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

#define dbj_clib_MAJOR  0
#define dbj_clib_MINOR  3
#define dbj_clib_PATCH  0

#define dbj_clib_VERSION  dbj_clib_STRINGIFY(dbj_clib_MAJOR) "." dbj_clib_STRINGIFY(dbj_clib_MINOR) "." dbj_clib_STRINGIFY(dbj_clib_PATCH)

#define dbj_clib_STRINGIFY(  x )  dbj_clib_STRINGIFY_( x )
#define dbj_clib_STRINGIFY_( x )  #x


/*
Note: it is a good idea, while inside c++ to have this inside a namespace
namespace dbjclib {}
seems like a good choice
*/
#ifdef __cplusplus
namespace dbjclib {
extern "C" {
#endif

#include "dbj_string/dbj_string_trim.h"
#include "dbj_string/dbj_front_back_string.h"
#include "dbj_sll/dbj_sll.h" 
#include "dbj_string_list/dbj_string_list.h"

#ifdef __cplusplus
} // extern "C"
} // namespace dbjclib 
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

/*
for this to work add the dbjlib folder to addional directories 
for linker, in project properties
*/
#ifdef NDEBUG
#if defined( _WIN32 ) 
#pragma comment(lib, "dbjclib_x86_Release.lib")
#else // _WIN64
#pragma comment(lib, "dbjclib_x64_Release.lib")
#endif
#else /* DEBUG */
#if defined( _WIN64 ) 
#pragma comment(lib, "dbjclib_x64_Debug.lib")
#else // _WIN32
#pragma comment(lib, "dbjclib_86_Debug.lib")
#endif
#endif

#endif // ! _DBJ_CLIB_API_

