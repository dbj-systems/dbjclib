#ifndef _DBJ_CLIB_TESTS_
#define _DBJ_CLIB_TESTS_

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

	typedef int (*test_unit) (FILE*) ;

	int dbj_string_list_test(FILE*);

	int dbj_fb_string_test(FILE*);

	int dbj_string_trim_test(FILE*);

#ifdef __cplusplus
} // extern "C" 
#endif

#endif // !_DBJ_CLIB_TESTS_

