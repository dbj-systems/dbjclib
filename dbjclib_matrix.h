#pragma once

#include "valstat_interop.h"

DBJ_CLIB_BEGIN
//
//---------------------------------------------------------------------------
//for 2d arrays we do not use pointer pointer and static allocator
//
#define DBJ_MATRIX(width,height,type, name)\
typedef struct matrix_##name {\
	int width; \
	int height;\
	type * data;\
} matrix_##name ;

/*
----------------------------------------------------------------
c 2darray is organized in row-major-order
https://en.wikipedia.org/wiki/Row-_and_column-major_order

reminder on C 2d array orinteering: matrix[width][height];
this is pure non VLA version

1. always draw a diagram if in doubt what and why is bellow
2. arr[i][j] is same as *(*(arr+i)+j)

WARNIG! _malloca might resort to heap! _freea() should be used to free
		the result of _malloca
*/
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

#define DBJ_STACKMAT_FREE(P_) if (P_) {_freea(P_); P_ = 0; }


	DBJ_CLIB_END
