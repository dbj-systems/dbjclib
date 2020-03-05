#include "../dbjclib_core.h"
#include "dbj_string_trim.h"

/*
Copyright 2018-2020 by dbj@dbj.org, CC BY SA 4.0

call with b + 1 aka  to_string( front, back + 1);
*/
static char *to_string(char *f, char *b)
{
	assert(f, !=, NULL);
	assert(b, !=, NULL);

	const int size = b - f;
	char *rez = DBJ_CALLOC(char, size + 1);
	memcpy(rez, f, size);
	rez[size] = '\0';
	return rez;
}
	// static const int		EOS = (int)'\0';
	/*
	static const char * digits[] = { "0123456789" };
	static const char * uppercase_letters[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	static const char * lowercase_letters[] = { "abcdefghijklmnopqrstuvwxyz" };
	*/

	dbj_string_trim_policy dbj_current_string_trim_policy = dbj_default_string_trim_policy ;

	/*
	internal front/back driver
	this is the policy user
	*/
	static void string_trim_front_back_driver(char ** begin_, char ** end_)
	{
		// empty string case
		if (*begin_ == *end_) return;

		while (dbj_current_string_trim_policy(**begin_)) {
			(*begin_)++;
			// depending on the policy
			// trim result has collapsed into empty string
			if (*begin_ == *end_) return;
		};

		// the right trim
		while (dbj_current_string_trim_policy(**end_)) {
			(*end_)--;
		};
	}

// if *back_ is NULL then text_
// must be zero limited string
// that is with EOS ('\0') a the very end
// REMEMBER:
// to conform to the STL meaning of "end"
// user has to move the back_ result
// one to the right
	valstat_char dbj_string_trim( const char * text_,	char ** front_, char ** back_	)
	{

		if ( dbj_current_string_trim_policy == NULL ) 
		{
			*front_ = *back_ = NULL; 
			return valstat_error( valstat_char, valstat_status("dbj_current_string_trim_policy is null?") );
		}
		/* singularity: empty not-a-null string */
		if ( text_ == NULL ) 
		{
			*front_ = *back_ = NULL; 
			return valstat_error( valstat_char, valstat_status("bad input: null input string") );
		}

		/* singularity: empty not-a-null string */
		if ( text_[0] == '\0') 
		{
			*front_ = *back_ = NULL; 
			return valstat_error( valstat_char, valstat_status("bad input: empty not-a-null input string") );
		}
		
		// do not assume front points to slot [0]
		// of the input string
		if ( *front_ == NULL )
		*front_ = (char *)& text_[0];
		
		if (*back_ == NULL) {
			size_t text_len = strlen(text_);
			// notice how back_ is not the C++ end_
			*back_ = (char *)& text_[text_len-1];
		} 

		assert(*front_);
		assert(*back_);

		// return on empty text
		if (*front_ == *back_ ) {
			return
			valstat_error( valstat_char, valstat_status("bad input:front and back walkers are pointing to the same address") );
		}

		// move the pointers according to policy
		string_trim_front_back_driver(front_, back_);

				// return on empty text
		if (*front_ == *back_ ) {
			return
			valstat_error( valstat_char, valstat_status("bad input:front and back walkers are pointing to the same address") );
		}
		// OK valstat
        return ( valstat_char ){ to_string(front_, back_ + 1) , NULL } ;
	}

// EOF