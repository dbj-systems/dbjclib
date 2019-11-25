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

#endif // ! _DBJ_CLIB_API_

