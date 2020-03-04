// the actual dbj++clib low level stuff

#include "../dbjclib_core.h"
#include "dbj_string.h"
#include <stdarg.h>

/*
Destination size can be less than strlen of destination ...
*/
char * dbj_strcpy_s(char * destination_, size_t destination_size, char const * source_) 
{
	// source_ must be zero limited
	// or simply zero-ed
	size_t index_ = 0;
	while ((destination_[index_] = source_[index_]) != 0 )
	{
		if ((++index_) > destination_size) {
			perror(__FILE__);
			perror("dbj_strcpy_s() problem: destination_size too small?");
			errno = EINVAL;
			return NULL;
		}
	}
		return destination_;
}

/*---------------------------------------------------------------------*/
char * dbj_strdup(const char *source_)
{
	DBJ_ASSERT(source_);
#ifdef _DEBUG
	const char * eos_found = strchr(source_, (char)0);
#ifdef _MSC_VER
	DBJ_ASSERT(eos_found);
#endif
	if (!eos_found) {
		errno = EINVAL;
		return NULL;
	}

#endif // _DEBUG

	size_t destination_size = strlen(source_);
	// Space for length plus null
	uchar_t *destination_  =  DBJ_CALLOC(uchar_t, destination_size );
	if (destination_  == NULL) {
		errno = ENOMEM;
		return NULL;
	}  

	destination_[destination_size] = '\0';
	return (char *)memcpy(destination_, source_, destination_size);
}

char * dbj_strndup(const char *s, size_t n)
{
	char *result = 0;
	size_t len = strlen(s);

	if (n < len) len = n;

	result = DBJ_CALLOC( char, len + 1);
	if (result == NULL) {
		errno = ENOMEM;
		return NULL;
	}  // No memory

	result[len] = '\0';
	return (char *)memcpy(result, s, len);
}


// remove *all* chars given in the string arg
// return the new string
// user has to free() eventually
// both arguments must be zero limited
char * dbj_str_remove( const char * str_, const char * chars_to_remove_ )
{
	assert(strchr(str_, 0));
	assert(strchr(chars_to_remove_, 0));

	size_t str_size = strlen(str_);
	// VLA actually ;)
	char * vla_buff_ = DBJ_CALLOC(char, str_size ); 
	assert( vla_buff_ );
	char * vla_buff_first = & vla_buff_[0] ;
	char * vla_buff_last  = & vla_buff_[str_size - 1] ;
	char * buff_p = vla_buff_first;
	for  (char * j = (char *)str_; *j != (char)0; j++) {
		if (0 == strchr(chars_to_remove_, *j))
		{
			*buff_p = *j; buff_p++;
		}
		// safety measure
		if (buff_p == (1 + vla_buff_last)) break; 
	}
	*buff_p = 0; 
	char * retval = dbj_strndup(vla_buff_, buff_p - vla_buff_);
	free(vla_buff_);
	return retval;
}