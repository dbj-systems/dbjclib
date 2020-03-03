#include "../dbj_str_trim/dbj_string_trim.h"
#include "munit/munit.h"

	// DBJ proposal :
	//
	// void using namespace ::std;
	// stop using namespace

	// testing data
	static char target[] = { "LINE O FF\n\rTE\v\tXT" };
	static char * text_test_data[] = {
		 "   LINE O FF\n\rTE\v\tXT    " ,
		 "   LINE O FF\n\rTE\v\tXT" ,
		    "LINE O FF\n\rTE\v\tXT" ,
		// all spaces singularity
		 "     " ,
		// empty string singularity
		 "" 
	};

	// trim the string view buffer
	// return the result in a char array given
	// caller is responsible to free()
	static char * trimmer(
		const char text[0xFF],
		size_t text_size ,
		// if false send the whole buffer
		bool zero_limited_string 
	)
	{
		munit_assert_true (text != NULL);

		if (*text == '\0') {
			char * rezbuf = calloc(1, 1);
			rezbuf[0] = '\0';
			return rezbuf;
		}

		// const int text_size = _countof(text);
		char * front_ = 0, *back_ = 0;

		// if requested set back_ pointer
		// to the end of the buffer position
		if (!zero_limited_string) {
			back_ = (char *)&text[text_size - 1];
		}

		dbj_string_trim(text, &front_, &back_);

		const __int64 rez_size_ = (back_ + 1 - front_);
		char * rezbuf = calloc(1 + rez_size_,sizeof(char));
munit_assert_true(rezbuf);
		memcpy(rezbuf, front_, rez_size_);
		// we can add 1 since we are smaller than text
		// bcause we are triming it
		rezbuf[rez_size_] = '\0';
		return rezbuf ;
	}

	// return true on success
	static bool trim_assert (
		char required_outcome [], 
		size_t test_data_index, 
		bool zero_delimited_strings
	) {
		const char * trim_rezult
			= trimmer(text_test_data[test_data_index], strlen(text_test_data[test_data_index]),   zero_delimited_strings);
		bool outcome_ = (0 == strcmp( required_outcome, trim_rezult));
			free((void *)trim_rezult);
		return outcome_;
	};

	// extern "C" void dbj_string_trim_test();

	MunitResult dbj_string_trim_test(const MunitParameter params[], void *data)
	{

		// the classic use case is trimming spaces
		// from zero delimited string literals
		// using default triming policy ( char == ' ')
		current_dbj_string_trim_policy = dbj_is_space;

		// using zero delimited strings
		// results are predictable
		munit_assert_true(
		trim_assert(target, 0, true));
		munit_assert_true(
		trim_assert(target, 1, true));
		munit_assert_true(
		trim_assert(target, 2, true));
		munit_assert_true(
		trim_assert(" ",	3, true));
		munit_assert_true(
		trim_assert("",		4, true));

		// using the whole buffer
		// that is: NON zero limited strings?
		// That is fine, *if* we change the policy
		// as the first current char might be EOS
		// when we start moving the back_ pointer
		// this policy will also provoke moving over anything 
		// that is not alphanum but in a more 
		// locale resilient manner
		current_dbj_string_trim_policy = dbj_move_if_not_alnum;

		munit_assert_true(
		trim_assert(target, 0, false));
		munit_assert_true(
		trim_assert(target, 1, false));
		munit_assert_true(
		trim_assert(target, 2, false));

		// results are not that predictable
		// when trimming char array vs strings 
		// with user defined policies
		// in here for example
		// all spaces input will collapse to 
		// char[1]{0} not char * to ""
		const char * r_0 = trimmer(text_test_data[3], strlen(text_test_data[3]), false);
		// same is for empty string input
		const char * r_1 = trimmer(text_test_data[4], strlen(text_test_data[3]), false);

		free((void *)r_0);
		free((void *)r_1);

		return MUNIT_OK;
	}


