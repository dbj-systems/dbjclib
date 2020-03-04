/*
Copyright 2018 by dbj@dbj.org

Concept 1: string is two pointers, front and back
char * str = "ABC" ;
char * front = str ;
char * back  = str + strlen(str) ;

Effectively this is the concept of a 'range'
Sub-range is two pointers to the *same* buffer

Concept 2: minimize the use of the string.h

*/

#include "dbj_front_back_string.h"
#include "../dbjclib_core.h"



/* private utils */
static size_t private_strlen(char const *str_)
{
	assert(str_);
	char *walker_ = (char *)str_;
	size_t rezult_ = 0;
	while (walker_ != '\0')
	{
		rezult_ += 1;
		walker_++;
		if (rezult_ == DBJ_MAX_STRING_LENGTH)
			break;
	}
	return rezult_;
}

/*
allocate the new structure
*/
dbj_string *dbj_string_make_empty()
{
	dbj_string *pair_ = DBJ_MALLOC(dbj_string);
	assert(pair_);
	pair_->front = 0;
	pair_->back = 0;
	return pair_;
}

/*
return true if front and back are not NULL
and size is in the allowed boundaries
*/
bool dbj_valid_string(const dbj_string *str)
{
	if (!str)
		return false;
	return (
		str->front && str->back && ((str->back - str->front) > 0) && (DBJ_MAX_STRING_LENGTH > (str->back - str->front)));
}

void dbj_string_free(dbj_string *str)
{
	assert(str);

	free((void *)str->front);
	str->front = NULL; /* must unlink */

	free(str);
	str = 0;
}

/*
just a pointer distance
*/
const size_t dbj_string_len(const dbj_string *str_)
{
	assert(dbj_valid_string(str_));
	return (size_t)(str_->back - str_->front);
}

/*
make and allocate for a size givent
*/
dbj_string *dbj_string_alloc(size_t count)
{
	dbj_string *rez = dbj_string_make_empty();

	assert(DBJ_MAX_STRING_LENGTH > count);
	char *payload = DBJ_CALLOC( char, count );
	assert(payload);
	rez->front = payload;
	rez->back = payload + count;
	return rez;
}

dbj_string *dbj_string_append(
	const dbj_string *left_,
	const dbj_string *right_)
{
	assert(dbj_valid_string(left_));
	assert(dbj_valid_string(right_));

	dbj_string *rezult_ = dbj_string_alloc(dbj_string_len(left_) + dbj_string_len(right_));
	char *w_ = 0;
	char *r_ = rezult_->front;

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

	assert((rezult_->back - rezult_->front) > 0);

	return rezult_;
}

/*
compare the contents of two strings,
return true if equal
*/
bool dbj_string_compare(
	const dbj_string *left_,
	const dbj_string *right_)
{
	assert(dbj_valid_string(left_));
	assert(dbj_valid_string(right_));

	if (dbj_string_len(left_) != dbj_string_len(right_))
		return false;

	char *l_ = left_->front;
	char *r_ = right_->front;

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

return the sub-range as dbj_string with pointers to the same buffer
or NULL , with errno set to the the error
*/
dbj_string *dbj_to_subrange(dbj_string *str_, dbj_string *sub_)
{
	assert(str_ && sub_);
	assert(dbj_string_len(str_) > 0);
	assert(dbj_string_len(sub_) > 0);
	assert(dbj_string_len(sub_) < dbj_string_len(str_));

	dbj_string *sub_range_ = 0;

	/* outer loop is walk along the string */
	for (char *sp = str_->front; sp < str_->back; ++sp)
	{
		/* potenial substr start */
		if (*sp == *sub_->front)
		{
			char *sub_start_location = sp;
			bool sub_found_flag = false;
			/* walk along the potential substring */
			for (char *sw = sub_->front; sw != sub_->back; ++sw)
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
				sub_range_ = dbj_string_make_empty();
				sub_range_->front = sub_start_location;
				sub_range_->back = sp;
				return sub_range_;
				// marked for partial free-ing
			}
		}
	}
	/* none found */
	errno = EINVAL;
	return 0;
}

/*
	is pointer p pointing inside the string range?
	return DBJ_NPOS if not found
*/
const size_t dbj_p_is_in_range(const char *p_, dbj_string *str_)
{

	/* walk along the source */
	char *char_p = (char *)p_;
	for (char *walker = str_->front; *walker != *str_->back; ++walker)
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
const size_t dbj_c_is_in_range(const char c_, dbj_string *str_)
{
	/* walk along the source */
	for (char *walker = str_->front; *walker != *str_->back; ++walker)
	{
		if (c_ == *walker)
			return (size_t)(walker - str_->front);
	}
	return DBJ_NPOS;
}

/*
return append left and right of a sub_range
*/
dbj_string *dbj_remove_substring(dbj_string *range, dbj_string *sub_range)
{
	// the big sanity check first
	if (!((range->front < sub_range->front) && (sub_range->back < range->back)))
	{
		errno = EINVAL;
		return 0;
	}

	dbj_string *left = dbj_string_make_empty();
	dbj_string *right = dbj_string_make_empty();

	left->front = range->front;
	left->back = sub_range->front;
	right->front = sub_range->back;
	right->back = range->back;

	dbj_string *rez = dbj_string_append(left, right);

	dbj_string_free(left);
	dbj_string_free(right);

	return rez;
}

/*
 *********************************************************************************
 * DBJ STRING VIEW
 *********************************************************************************
 */

/*
effectively make a dbj_string from const char *
if arg is longer then DBJ_MAX_STRING_LENGTH
that is where the back will be
*/
dbj_string_view *dbj_make_sv(const char *string_)
{
	const size_t slen = private_strlen(string_);
	assert(DBJ_MAX_STRING_LENGTH > slen);

	dbj_string_view *sv_ = DBJ_MALLOC(dbj_string_view) ;

	sv_->front = (char *)string_;
	/* NOTE! if string_ is empty, back == front */
	sv_->back = (char *)string_ + slen;
	assert((size_t)(sv_->back - sv_->front) == slen);
	return sv_;
}

/*
take sub range as requested
*/
dbj_string_view *dbj_sv_make(const char *str, size_t from_, size_t to_)
{
	from_ -= 1;
	// to_ -= 1; we do not move the 'to' left since the concept is
	// back pointer is one beyond the last 'to'
	if ((to_ - from_) > private_strlen(str))
	{
		errno = EINVAL;
		return NULL;
	}
	dbj_string_view *retval = DBJ_MALLOC(dbj_string_view);
	// full_free is false here already
	retval->front = (char *)&str[from_];
	retval->back = (char *)&str[to_];
	return retval;
}

bool dbj_sv_compare(dbj_string_view* left_, dbj_string_view* right_)
{
	if (dbj_sv_len(left_) != dbj_sv_len(right_))
		return false;

	char *l_ = left_->front;
	char *r_ = right_->front;

	for (l_ = left_->front; l_ != left_->back; ++l_)
	{
		if (*l_ != *r_)
			return false;
		++r_;
	}
	return true;
}