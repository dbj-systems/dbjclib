#ifndef _DBJ_FRONT_BACK_STRING_
#define _DBJ_FRONT_BACK_STRING_

/*
Copyright 2018, 2019, 2020 by dbj@dbj.org, CC BY SA 4.0

DBJ_FBS == dbj Front Back String
*/
// #include "dbjclib.h"
#ifdef __clang__
/*
http://clang.llvm.org/docs/UsersManual.html#controlling-diagnostics-in-system-headers
*/
#pragma clang system_header
#endif /* __clang__ */

#include <stdbool.h>
#include <assert.h>

static const size_t		DBJ_NPOS = (size_t)-1;
static const size_t		DBJ_MAX_STRING_LENGTH = 0xFFFF;
/*
DBJ_FBS is two pointers, front and back
char * str = "ABC" ;
char * front = str ;
char * back  = str + strlen(str) ;

Effectively this is the concept of a char 'range'
Sub-range is two pointers to the *same* buffer

Motivation: 
- minimize the use of the string.h
- work without (semi) dangerous zero delimited strings
*/
 /* free the front ptr before freeing the struct */
typedef struct dbj_string {
	char * front;
	char * back;
} dbj_string;

/*
allocate the new structure
*/
dbj_string* dbj_string_make_empty();
/*
make empty payload of a given count chars
front of the allocated dbj_string has to be freed
*/
dbj_string* dbj_string_alloc(size_t count);

void dbj_string_free(dbj_string* str);

const size_t dbj_string_len(const dbj_string* str_);

/*
return true if front and back are not NULL
and are not equal
and size is in the allowed boundaries
*/
bool dbj_valid_string(const dbj_string* str);

/* ------------------------------------------------------------------ */
dbj_string* dbj_string_append(
	const dbj_string* left_,
	const dbj_string* right_
);

/*
compare the contents of two strings,
return true if equal
*/
bool dbj_string_compare(
	const dbj_string* left_,
	const dbj_string* right_
);

/*
is CONTENT of sub inside the CONTENT of str ?

return the sub-range as dbj_string with pointers to the same buffer
or NULL , with errno set to the the error
*/
dbj_string* dbj_to_subrange(dbj_string* str_, dbj_string* sub_);

/*
	is pointer p pointing inside the string range?
	return DBJ_NPOS if not found
*/
const size_t dbj_p_is_in_range(const char* p_, dbj_string* str_);
/*
	is character c inside
	the string content
	return the location or DBJ_NPOS if not found
*/
const size_t dbj_c_is_in_range(const char c_, dbj_string* str_);

/*
return append left and right of a sub_range
*/
dbj_string* dbj_remove_substring
(dbj_string* range, dbj_string* sub_range);

/*
 *********************************************************************************
 * DBJ STRING VIEW
 *********************************************************************************
 */
 /* do not free */
typedef struct dbj_string_view {
	char * front;
	char * back;
} dbj_string_view ;

/*
return true if front and back are not NULL
and size is in the allowed boundaries
*/
inline bool dbj_valid_sv(const dbj_string_view *str)
{
	if (!str)
		return false;
	return (
		str->front 
		&& str->back 
		&& ((str->back - str->front) > 0) 
		&& (DBJ_MAX_STRING_LENGTH > (size_t)(str->back - str->front)));
}

/*
just a pointer distance
*/
inline const size_t dbj_sv_len(const dbj_string_view *str_)
{
	assert(dbj_valid_sv(str_));
	return (size_t)(str_->back - str_->front);
}
/*
effectively make a view from const char 
warning: this is a view not a copy, keep the source arroud long enough
*/
dbj_string_view*
dbj_make_sv(const char* string_);
/*
take sub range as requested
warning: this is a view not a copy, keep the source arroud long enough
*/
dbj_string_view* dbj_sv_make(const char* str, size_t from_, size_t to_);

bool dbj_sv_compare(dbj_string_view*, dbj_string_view*);


#endif // _DBJ_FRONT_BACK_STRING_