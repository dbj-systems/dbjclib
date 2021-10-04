#pragma once
#ifndef DBJ_CHAR_RANGE_H_
#define DBJ_CHAR_RANGE_H_
#ifdef __clang__
#pragma clang system_header
#endif /* __clang__ */

/*
Copyright 2018, 2019, 2020, 2021 by dbj@dbj.org, CC BY SA 4.0

In 2020 this was DBJ_FBS == dbj Front Back String
*/
#include "../dbjclib.h"
#include "../dbjclib_core.h"

#include <stdbool.h>
#include <assert.h>

DBJ_CLIB_BEGIN

extern const size_t		DBJ_NPOS; //  = (size_t)-1;
extern const size_t		DBJ_CHAR_RANGE_MAX_; // = 0xFFFF;
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
typedef struct dbj_chr_rng {
	char* front;
	char* back;
} dbj_chr_rng;

/*
allocate the new structure
*/
dbj_chr_rng dbj_chr_rng_make_empty();
/*
make empty payload of a given count chars
front of the allocated dbj_chr_rng has to be freed
*/
dbj_chr_rng dbj_chr_rng_alloc(size_t);

/*
 copy a string literal to dbj_chr_rng
*/
dbj_chr_rng dbj_chr_rng_assign(const char*);

void dbj_chr_rng_free(dbj_chr_rng*);

const size_t dbj_chr_rng_len(const dbj_chr_rng*);

/*
return true if front and back are not NULL
and are not equal
and size is in the allowed boundaries
*/
bool dbj_valid_chr_range(const dbj_chr_rng*);

/*  */
dbj_chr_rng dbj_chr_rng_append(
	const dbj_chr_rng*,
	const dbj_chr_rng*
);

/*
compare the contents of two strings,
return true if equal
*/
bool dbj_chr_rng_compare(
	const dbj_chr_rng*,
	const dbj_chr_rng*
);

/*
is CONTENT of sub inside the CONTENT of str ?

return the sub-range as dbj_chr_rng with pointers to the same buffer
or NULL , with errno set to the the error
*/
dbj_chr_rng dbj_to_subrange(dbj_chr_rng*, dbj_chr_rng*);

/*
	is pointer p pointing inside the string range?
	return DBJ_NPOS if not found
*/
const size_t dbj_p_is_in_range(const char*, dbj_chr_rng*);
/*
	is character c inside
	the string content
	return the location or DBJ_NPOS if not found
*/
const size_t dbj_c_is_in_range(const char, dbj_chr_rng*);

/*
return append left and right of a sub_range
*/
dbj_chr_rng dbj_remove_substring(dbj_chr_rng*, dbj_chr_rng*);

/*
 *********************************************************************************
 * DBJ STRING VIEW
 *********************************************************************************
 */
 /* do not free */
typedef struct dbj_chr_rng_view {
	char* front;
	char* back;
} dbj_chr_rng_view;

/*
return true if front and back are not NULL
and size is in the allowed boundaries
*/
inline bool dbj_valid_sv(const dbj_chr_rng_view* str)
{
	if (!str)
		return false;
	return (
		str->front
		&& str->back
		&& ((str->back - str->front) > 0)
		&& (DBJ_CHAR_RANGE_MAX_ > (size_t)(str->back - str->front)));
}

/*
just a pointer distance
*/
inline const size_t dbj_sv_len(const dbj_chr_rng_view* str_)
{
	assert(dbj_valid_sv(str_));
	return (size_t)(str_->back - str_->front);
}
/*
effectively make a view from const char
warning: this is a view not a copy, keep the source arroud long enough
*/
dbj_chr_rng_view*
dbj_make_sv(const char*);
/*
take sub range as requested
warning: this is a view not a copy, keep the source arroud long enough
*/
dbj_chr_rng_view* dbj_sv_make(const char*, size_t, size_t);

bool dbj_sv_compare(dbj_chr_rng_view*, dbj_chr_rng_view*);

/* ---------------------------------------------------------------------------- */
#if (DBJ_CHAR_RANGE_IMPLEMENTATION == 1)
/* ----------------------------------------------------------------------------

Copyright 2018 by dbj@dbj.org

Concept 1: string is two pointers, front and back
char * str = "ABC" ;
char * front = str ;
char * back  = str + strlen(str) ;

Effectively this is the concept of a 'range'
Sub-range is two pointers to the *same* buffer

Concept 2: minimize the use of the string.h

*/

const size_t DBJ_NPOS = (size_t)-1;
const size_t DBJ_CHAR_RANGE_MAX_ = 0xFFFF;


/* private utils */
static inline size_t private_strlen(char const* str_)
{
	DBJ_ASSERT(str_);
	char* walker_ = (char*)str_;
	size_t rezult_ = 0;
	while (walker_[0] != '\0')
	{
		rezult_ += 1;
		walker_++;
		if (rezult_ > DBJ_CHAR_RANGE_MAX_)
			break;
	}

	DBJ_ASSERT(rezult_ < DBJ_CHAR_RANGE_MAX_);

	return rezult_;
}

/*
allocate the new structure
*/
dbj_chr_rng dbj_chr_range_make_empty()
{
	return (dbj_chr_rng) { 0, 0 };
}

/*
return true if front and back are not NULL
and size is in the allowed boundaries
*/
bool dbj_valid_chr_range(const dbj_chr_rng* str)
{
	if (!str)
		return false;
	return (
		str->front && str->back && ((str->back - str->front) > 0) && (DBJ_CHAR_RANGE_MAX_ > (str->back - str->front)));
}

void dbj_chr_range_free(dbj_chr_rng* str)
{
	DBJ_ASSERT(dbj_valid_chr_range(str));

	free(str->front);
	str->front = NULL; /* must unlink */
	str->back = NULL;
}

/*
just a pointer distance
*/
const size_t dbj_chr_range_len(const dbj_chr_rng* str_)
{
	DBJ_ASSERT(dbj_valid_chr_range(str_));
	return (size_t)(str_->back - str_->front);
}

/*
make and allocate for a size givent
*/
dbj_chr_rng dbj_chr_range_alloc(size_t count)
{
	DBJ_ASSERT(DBJ_CHAR_RANGE_MAX_ > count);
	dbj_chr_rng rez = dbj_chr_range_make_empty();
	char* payload = DBJ_CALLOC(char, count);
	DBJ_ASSERT(payload);
	rez.front = payload;
	rez.back = payload + count;
	return rez;
}

dbj_chr_rng dbj_chr_range_assign(const char* str_)
{
	// asserts on size overflow
	const size_t str_len = private_strlen(str_);
	DBJ_ASSERT(DBJ_CHAR_RANGE_MAX_ > str_len);

	dbj_chr_rng rez = dbj_chr_range_make_empty();

	rez.front = _strdup(str_);
	DBJ_ASSERT(rez.front);
	rez.back = rez.front + str_len;
	return rez;
}

dbj_chr_rng dbj_chr_range_append(
	const dbj_chr_rng* left_,
	const dbj_chr_rng* right_)
{
	DBJ_ASSERT(dbj_valid_chr_range(left_));
	DBJ_ASSERT(dbj_valid_chr_range(right_));

	dbj_chr_rng rezult_ = dbj_chr_range_alloc(dbj_chr_range_len(left_) + dbj_chr_range_len(right_));
	char* w_ = 0;
	char* r_ = rezult_.front;

	for (w_ = left_->front; w_ != left_->back; ++w_)
	{
		*r_ = *w_;
		++r_;
	}

	for (w_ = right_->front; w_ != right_->back; ++w_)
	{
		*r_ = *w_;
		++r_;
	}

	DBJ_ASSERT((rezult_.back - rezult_.front) > 0);

	return rezult_;
}

/*
compare the contents of two strings,
return true if equal
*/
bool dbj_chr_range_compare(
	const dbj_chr_rng* left_,
	const dbj_chr_rng* right_)
{
	DBJ_ASSERT(dbj_valid_chr_range(left_));
	DBJ_ASSERT(dbj_valid_chr_range(right_));

	if (dbj_chr_range_len(left_) != dbj_chr_range_len(right_))
		return false;

	char* l_ = left_->front;
	char* r_ = right_->front;

	for (l_ = left_->front; l_ != left_->back; ++l_)
	{
		if (*l_ != *r_)
			return false;
		++r_;
	}
	return true;
}

/*
is CONTENT of sub inside the CONTENT of str ?

return the sub-range as dbj_chr_rng with pointers to the same buffer
or NULL , with errno set to the the error
*/
dbj_chr_rng dbj_to_subrange(dbj_chr_rng* str_, dbj_chr_rng* sub_)
{
	DBJ_ASSERT(str_ && sub_);
	DBJ_ASSERT(dbj_chr_range_len(str_) > 0);
	DBJ_ASSERT(dbj_chr_range_len(sub_) > 0);
	DBJ_ASSERT(dbj_chr_range_len(sub_) < dbj_chr_range_len(str_));

	dbj_chr_rng sub_range_ = {};

	/* outer loop is walk along the string */
	for (char* sp = str_->front; sp != str_->back; ++sp)
	{
		/* potenial substr start */
		if (*sp == *sub_->front)
		{
			char* sub_start_location = sp;
			bool sub_found_flag = false;
			/* walk along the potential substring */
			for (char* sw = sub_->front; sw != sub_->back; ++sw)
			{
				/* compare sub and str char's*/
				sub_found_flag = (*sw == *sp);
				/* and move the outer walker to */
				++sp;
				/* finish the loop */
				if (!sub_found_flag)
					break;
			}
			/* inner loop has finished    */
			/* and there where no misses? */
			if (sub_found_flag)
			{
				sub_range_ = dbj_chr_range_make_empty();
				sub_range_.front = sub_start_location;
				sub_range_.back = sp;
				return sub_range_;
				// marked for partial free-ing
			}
		}
	}
	/* none found */
	errno = EINVAL;
	return sub_range_; // empty means error
}

/*
	is pointer p pointing inside the string range?
	return DBJ_NPOS if not found
*/
const size_t dbj_p_is_in_range(const char* p_, dbj_chr_rng* str_)
{

	/* walk along the source */
	char* char_p = (char*)p_;
	for (char* walker = str_->front; *walker != *str_->back; ++walker)
	{
		if (char_p == walker)
			return (size_t)(walker - str_->front);
	}
	return DBJ_NPOS;
}
/*
	is character c inside
	the string content
	return the location or DBJ_NPOS if not found
*/
const size_t dbj_c_is_in_range(const char c_, dbj_chr_rng* str_)
{
	/* walk along the source */
	for (char* walker = str_->front; *walker != *str_->back; ++walker)
	{
		if (c_ == *walker)
			return (size_t)(walker - str_->front);
	}
	return DBJ_NPOS;
}

/*
return appended left and right of a sub_range taken out
*/
dbj_chr_rng dbj_remove_substring(dbj_chr_rng* range, dbj_chr_rng* sub_range)
{
	// the big sanity check first, if sub_range is indeed
	// a sub range to range
	dbj_chr_rng check_ = dbj_to_subrange(range, sub_range);
	if (!dbj_valid_chr_range(&check_))
	{
		errno = EINVAL;
		return (dbj_chr_rng) { 0, 0 };
	}

	dbj_chr_rng left = dbj_chr_range_make_empty();
	dbj_chr_rng right = dbj_chr_range_make_empty();

	left.front = range->front;
	left.back = check_.front;
	right.front = check_.back;
	right.back = range->back;

	//if ((!dbj_valid_chr_range(&left)) || (!dbj_valid_chr_range(&right)))
	//{
	//	errno = EINVAL;
	//	return (dbj_chr_rng) { 0, 0 };
	//}

	dbj_chr_rng rez = dbj_chr_range_append(&left, &right);

	dbj_chr_range_free(&left);
	dbj_chr_range_free(&right);

	return rez;
}

/*
 *********************************************************************************
 * DBJ STRING VIEW
 *********************************************************************************
 */

 /*
 effectively make a dbj_chr_rng from const char *
 if arg is longer then DBJ_CHAR_RANGE_MAX_
 that is where the back will be
 */
dbj_chr_rng_view* dbj_make_sv(const char* string_)
{
	const size_t slen = private_strlen(string_);
	DBJ_ASSERT(DBJ_CHAR_RANGE_MAX_ > slen);

	dbj_chr_rng_view* sv_ = DBJ_MALLOC(dbj_chr_rng_view);

	sv_->front = (char*)string_;
	/* NOTE! if string_ is empty, back == front */
	sv_->back = (char*)string_ + slen;
	DBJ_ASSERT((size_t)(sv_->back - sv_->front) == slen);
	return sv_;
}

/*
take sub range as requested
*/
dbj_chr_rng_view* dbj_sv_make(const char* str, size_t from_, size_t to_)
{
	from_ -= 1;
	// to_ -= 1; we do not move the 'to' left since the concept is
	// back pointer is one beyond the last 'to'
	if ((to_ - from_) > private_strlen(str))
	{
		errno = EINVAL;
		return NULL;
	}
	dbj_chr_rng_view* retval = DBJ_MALLOC(dbj_chr_rng_view);
	// full_free is false here already
	retval->front = (char*)&str[from_];
	retval->back = (char*)&str[to_];
	return retval;
}

bool dbj_sv_compare(dbj_chr_rng_view* left_, dbj_chr_rng_view* right_)
{
	if (dbj_sv_len(left_) != dbj_sv_len(right_))
		return false;

	char* l_ = left_->front;
	char* r_ = right_->front;

	for (l_ = left_->front; l_ != left_->back; ++l_)
	{
		if (*l_ != *r_)
			return false;
		++r_;
	}
	return true;
}

/* ---------------------------------------------------------------------------- */
DBJ_CLIB_END
/* ---------------------------------------------------------------------------- */
#endif // DBJ_CHAR_RANGE_IMPLEMENTATION
/* ---------------------------------------------------------------------------- */


#endif // DBJ_CHAR_RANGE_H_