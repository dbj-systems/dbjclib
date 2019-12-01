
#include "dbj_string_list.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>

static const char dbj_string_list_sentinel_char = '\033'; // ESC aka ((char)127);

static const dbj_string_list_value_type dbj_string_list_sentinel_
= (dbj_string_list_value_type)(&dbj_string_list_sentinel_char);

/***************************************************************************s
Here is the secret sauce ... we allocate the whole array of NULL's
and we put the sentinel in the slot 0
*/
dbj_string_list_type dbj_string_list_new()
{
	dbj_string_list_type empty_ = 0;
	empty_ = (dbj_string_list_type)calloc( dbj_string_list_max_size, sizeof(empty_) );

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
return the pointer to the sentinel element
*/
static dbj_string_list_type dbj_string_list_sentinel_ptr(dbj_string_list_type head_)
{
	dbj_string_list_type walker_ = head_;
#ifdef _DEBUG
	unsigned counter = 0;
#endif
	while (1 == 1) {
		if (*walker_ == dbj_string_list_sentinel_) break;
		walker_++;
		assert(counter++ < dbj_string_list_max_size);
	}
	return (walker_);
}

/***************************************************************************
thus on append we do not reallocate, we just increment the sentinel
side effect: head is not invalidated
*/
dbj_string_list_type dbj_string_list_append (dbj_string_list_type head_, const dbj_string_list_value_type str_)
{
	assert(str_);

	if (!str_ || !head_) {
		errno = EINVAL; return NULL;
	}

	dbj_string_list_type end_ = dbj_string_list_sentinel_ptr(head_);

	// check if we have the overflow
	// WARNING : the last stol is reserved for sentinel
	// str_ can not be placed in the last slot
	size_t current_count_ = (end_ - head_);
	assert((1+ current_count_) < dbj_string_list_max_size);

	if (false == ((1+ current_count_) < dbj_string_list_max_size)) {
		errno = ENOMEM; return NULL;
	}

	char* str_copy = _strdup(str_);
	if (!str_copy) {errno = ENOMEM;	return NULL ; }

	// new string goes to where the sentinel was
	head_[current_count_ + 0] =  str_copy ;
	// move the sentinel
	head_[current_count_ + 1]
		= (dbj_string_list_value_type)dbj_string_list_sentinel_;

	return head_;
}

/***************************************************************************
size = sentinel pos - head
*/
uint16_t dbj_string_list_size(dbj_string_list_type head_) {

	dbj_string_list_type end_ = dbj_string_list_sentinel_ptr(head_);
	// check if we have the overflow
	uint16_t current_count_ = (uint16_t)(end_ - head_);
	assert(current_count_ < dbj_string_list_max_size);
	return current_count_;
}

/**
get by index
*/
dbj_string_list_value_type
dbj_string_list_at_index(uint16_t idx_, dbj_string_list_type head_, uint16_t size_)
{
	uint16_t current_size_ = size_;
	if (current_size_ < 1) {
		current_size_ = dbj_string_list_size(head_);
	}
	assert(idx_ < current_size_);
	return head_[idx_];
}

void dbj_string_list_free(dbj_string_list_type head_)
{
	assert(head_);
	// not empty?
	if (dbj_string_list_sentinel_ != *head_) {
		// 
		dbj_string_list_type end_ = dbj_string_list_sentinel_ptr(head_);
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

dbj_string_list_type dbj_string_list_reset(dbj_string_list_type head_)
{
	dbj_string_list_free(head_);
	return dbj_string_list_new();
}

