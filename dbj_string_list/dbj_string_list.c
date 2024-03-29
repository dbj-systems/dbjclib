
#include "dbj_string_list.h"
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/*extern*/ const uint16_t dbj_string_list_max_capacity = UINT16_MAX - 1;

static const char dbj_string_list_sentinel_char = '\033'; // ESC aka ((char)127);

static const dbj_string_list_value_type dbj_string_list_sentinel_
= (dbj_string_list_value_type)(&dbj_string_list_sentinel_char);

/***************************************************************************s
Here is the secret sauce ... we allocate the whole array of NULL's
and we put the sentinel in the slot 0
*/
dbj_string_list_value_type_ptr dbj_string_list_new()
{
	/* last slot is for the sentinel of the full list  */
	dbj_string_list_value_type_ptr empty_
		= calloc(dbj_string_list_max_capacity + 1, sizeof(dbj_string_list_value_type_ptr));

	assert(empty_);

	if (!empty_) {
		errno = ENOMEM; return NULL;
	}
	*empty_ = (char*)dbj_string_list_sentinel_;
	return empty_;
}

/***************************************************************************
where is the sentinel currently?
Here is the secret sauce no 2 ... we do not keep it, we find it each time
why? because this is very fast on modern machines, and because that simplifies
the design AND the implementation

return the pointer to the sentinel element
*/
static dbj_string_list_value_type_ptr dbj_string_list_sentinel_ptr(dbj_string_list_value_type_ptr head_)
{
	dbj_string_list_value_type_ptr walker_ = head_;
	unsigned counter = 0;

	while (1 == 1) {
		if (*walker_ == dbj_string_list_sentinel_) break;
		walker_++;
		assert(counter++ < dbj_string_list_max_capacity);
	}
	return (walker_);
}

/***************************************************************************
on append we do not reallocate, we just increment the sentinel
note: we do copy the string argument
side effect: this is fast because there is no realloc
side effect: head is not invalidated
*/
dbj_string_list_value_type_ptr dbj_string_list_append(dbj_string_list_value_type_ptr head_, const dbj_string_list_value_type str_)
{
	assert(str_);

	if (!str_ || !head_) {
		errno = EINVAL; return NULL;
	}

	dbj_string_list_value_type_ptr end_ = dbj_string_list_sentinel_ptr(head_);

	// check if we have the overflow
	// WARNING : the last slot is reserved for sentinel
	// str_ can not be placed in the last slot
	size_t current_count_ = (end_ - head_);
#ifdef  _DEBUG 
	assert((1 + current_count_) < dbj_string_list_max_capacity);
#else
	/* RELEASE */
	if ((1 + current_count_) > dbj_string_list_max_capacity) {
		errno = EINVAL; return NULL;
	}
#endif

	if (false == ((1 + current_count_) < dbj_string_list_max_capacity)) {
		errno = ENOMEM; return NULL;
	}

	char* str_copy = _strdup(str_);
	if (!str_copy) { errno = ENOMEM;	return NULL; }

	// new string goes to where the sentinel was
	head_[current_count_ + 0] = str_copy;
	// move the sentinel
	head_[current_count_ + 1]
		= (dbj_string_list_value_type)dbj_string_list_sentinel_;

	return head_;
}

/***************************************************************************
size = sentinel pos - head
has to be computed when needed
*/
uint16_t dbj_string_list_size(dbj_string_list_value_type_ptr head_) {

	dbj_string_list_value_type_ptr end_ = dbj_string_list_sentinel_ptr(head_);
	// check if we have the overflow
	uint16_t current_count_ = (uint16_t)(end_ - head_);
	assert(current_count_ < dbj_string_list_max_capacity);
	return current_count_;
}

/**
get by index
last arg is list size, but only if it is 0; it will be computed
thus you can optimize:

	// get the size only here and only once
	const size_t size_ = dbj_string_list_size( head ) ;

	// for k from 0 to size_
	dbj_string_list_value_type element_ =
	dbj_string_list_at_index( k, head, size_ );
*/
dbj_string_list_value_type
dbj_string_list_at_index(uint16_t idx_, dbj_string_list_value_type_ptr head_, uint16_t size_)
{
	uint16_t current_size_ = size_;
	if (current_size_ < 1) {
		current_size_ = dbj_string_list_size(head_);
	}
	assert(idx_ < current_size_);
	return head_[idx_];
}

void dbj_string_list_free(dbj_string_list_value_type_ptr head_)
{
	assert(head_);
	// not empty?
	if (dbj_string_list_sentinel_ != *head_) {
		// 
		dbj_string_list_value_type_ptr end_ = dbj_string_list_sentinel_ptr(head_);
		char* last_word_ = 0;
		// we skip over the sentinel
		// as the space for it was not allocated
		end_ -= 1;
		while (end_ != head_) {
			last_word_ = *end_;
			free(last_word_);
			last_word_ = *end_ = 0;
			end_ -= 1;
		};
	}
	if (head_) {
		free(head_);
		head_ = 0;
	}
	// free(head_);
}

dbj_string_list_value_type_ptr dbj_string_list_reset(dbj_string_list_value_type_ptr head_)
{
	dbj_string_list_free(head_);
	return dbj_string_list_new();
}

