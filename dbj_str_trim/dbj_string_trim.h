#ifndef _DBJ_STRING_TRIM_INC_
#define _DBJ_STRING_TRIM_INC_
/*
To "trim" is not the same as char replace. Triming means "cut off from left or right or both".
What to trim is the question of trimming policy you can define for this trimmer. 

  "   TEXT " --> "TEXT" // policy: dbj_is_space
" \n  TEXT " --> "\n  TEXT" // same policy
" \n  TEXT " --> "TEXT" // policy: dbj_is_white_space

users need to trim eveything to the '*' char

inline bool find_the_star ( uchar_t c_ ) {
	if (c_ == '*') return false; // stop
	return true; // proceed 
}

dbj_string_trim_policy = find_the_star ;

" \t\n\n\f   *TEXT* \v  \n\n" ---> "*TEXT*"

## Flexibility == Power == Complexity

If user defines the trimming policy differently, the star chars will not
be left in the result. Here is the ammended trimming policy

// #define LEAVE_THE_STARS_IN
inline bool find_the_star(uchar_t c_)
{
#ifdef LEAVE_THE_STARS_IN
	if (c_ == '*')
		return false; // stop
	return true;	  // proceed
#else
	if (isalnum(c_))
		return false; // stop
	else
		return true; // proceed
#endif
}

" \t\n\n\f   *TEXT* \v  \n\n" ---> "TEXT"

*/

#include <stdbool.h>
/*
Advice: for ASCII escape sequence read https://en.wikipedia.org/wiki/Escape_sequences_in_C
Advice: read + understand ctype.h
*/
#include "../valstat_interop.h"
#include <ctype.h>

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region string triming with policies

	typedef unsigned char uchar_t;
	typedef bool (*dbj_string_trim_policy)(unsigned char);

	inline bool dbj_seek_alnum(uchar_t c)
	{
		if (c >= '0' && c <= '9')
			return false;
		if (c >= 'A' && c <= 'Z')
			return false;
		if (c >= 'a' && c <= 'z')
			return false;
		// c is not alnum so move the pointer
		// by returning true
		return true;
	}

	// c is space so move the pointer by returning true
	inline bool dbj_is_space(uchar_t c)
	{
		return c == ' ';
	}

	// any kind of white space ?
	// yes: move the pointer by returning true
	inline bool dbj_is_white_space(uchar_t c)
	{
		return isspace(c); // true means proceed
		// return c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
		// add whatever else you consider white space
		return false;
	}

	/*
	make the driver to move the pointer
	if c is whitespace, space or eos
	not using locale but should be prety resilient to local chars
	since whitespace, space and eos are not locale specific
	*/
	inline bool dbj_move_to_alnum(uchar_t c)
	{
		// positioned on EOS char
		// thus move the pointer
		if ((int)c == 0)
			return true;
		// space + whitespaces
		if (isspace(c))
			return true;
		// ask the driver to stop the movement
		// since c is not any of the above
		return false;
	}
#pragma endregion

/*
	gloobal 'holder' of the current policy function
	defualt policy is char == ' '

	dbj_is_space is default policy, it equates to  char == ' ', test
	users can provide their own drivers
	by assigning to the global bellow
	Note: while inside c++ this is all in the dbj::clib namespace
	*/
#define dbj_default_string_trim_policy dbj_move_to_alnum
	extern dbj_string_trim_policy dbj_current_string_trim_policy;

	/* valstat struct returned contains a pointer to the rezulting newly allocated 
   string

   DO NOT free that pointer. Use it quickly it is a pointer to the stack allocated
   char array

    declare and define valstat_char  */
	valstat(char);

	// if *back_ is NULL then text_
	// must be zero limited string
	// that is with EOS ('\0') a the very end
	//
	// REMEMBER:
	// to conform to the C++ meaning of "end"
	// user has to move the back_
	// one to the right, *before* using it in STD space
	// for example.
	//
	// std::string rezult( front_, back_ + 1 ) ;
	//
	valstat_char dbj_string_trim(const char *text_, char **front_, char **back_);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _DBJ_STRING_TRIM_INC_
