#ifndef _DBJ_FRONT_BACK_STRING_
#define _DBJ_FRONT_BACK_STRING_

/*
Copyright 2018, 2019, 2020 by dbj@dbj.org

DBJ_FBS == dbj Front Back String

DBJ_FBS is two pointers, front and back
char * str = "ABC" ;
char * front = str ;
char * back  = str + strlen(str) ;

Effectively this is the concept of a 'range'
Sub-range is two pointers to the *same* buffer

Concept: minimize the use of the string.h
*/
// #include "dbjclib.h"
#ifdef __clang__
/*
http://clang.llvm.org/docs/UsersManual.html#controlling-diagnostics-in-system-headers
*/
#pragma clang system_header
#endif /* __clang__ */

static const size_t		DBJ_NPOS = (size_t)-1;
static const size_t		DBJ_MAX_STRING_LENGTH = 0xFFFF;


typedef struct dbj_string {
	 /*if true, free the front ptr
	 before freeing the struct*/
	bool   full_free;
	char * front;
	char * back;
} dbj_string;

/*
allocate the new structure
*/
dbj_string* dbj_string_null();

/*
return true if front and back are not NULL
and are not equal
and size is in the allowed boundaries
*/
bool dbj_valid_string(const dbj_string* str);


void dbj_string_free(dbj_string* str);

const size_t dbj_string_len(const dbj_string* str_);

/*
effectively make a view from const char *
*/
dbj_string*
dbj_string_make_view(const char* string_);

/*
front of the allocated dbj_string has to be freed
*/
dbj_string* dbj_string_alloc(size_t count);

dbj_string* dbj_string_append(
	const dbj_string* left_,
	const dbj_string* right_
);

/*
compare the contents of two strings,
return true if equal
*/
inline bool dbj_string_compare(
	const dbj_string* left_,
	const dbj_string* right_
);
/*
take sub range as requested
free the string struct eventually but not the front pointer
*/
dbj_string* dbj_string_from(const char* str, size_t from_, size_t to_);
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

#endif // _DBJ_FRONT_BACK_STRING_