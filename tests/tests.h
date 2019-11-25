#ifndef _DBJ_CLIB_TESTS_
#define _DBJ_CLIB_TESTS_

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

	typedef int (*) (FILE*) test_unit;

	int dbj_string_list_test(FILE*);

	int dbj_front_back_string_test(FILE*);

	int dbj_string_trim_test(FILE* fp_);

#ifdef __cplusplus
} // extern "C" 
#endif

#endif // !_DBJ_CLIB_TESTS_

