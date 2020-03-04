#include "../dbj_str_trim/dbj_string_trim.h"
#include "munit/munit.h"

// DBJ proposal :
//
// void using namespace ::std;
// means: stop using namespace
/* --------------------------------------------------------------------- */
// testing data
static char target[] = {"LINE O FF\n\rTE\v\tXT"};
static char *text_test_data[] = {
	"   LINE O FF\n\rTE\v\tXT    ",
	"   LINE O FF\n\rTE\v\tXT",
	"LINE O FF\n\rTE\v\tXT",
	// all spaces singularity
	"     ",
	// empty string singularity
	""};
/* --------------------------------------------------------------------- */
static char *to_string(char *f, char *b)
{
	munit_assert_ptr(f, !=, NULL);
	munit_assert_ptr(b, !=, NULL);

	const int size = b - f;
	char *rez = munit_newa(char, size + 1);
	memcpy(rez, f, size);
	rez[size] = '\0';
	return rez;
}
/* --------------------------------------------------------------------- */
// trim the string view buffer
// return the result in a char array given
// caller is responsible to free()
static char *trimmer(
	const char text[0xFF],
	size_t text_size,
	// if false send the whole buffer
	bool zero_limited_string)
{
	munit_assert_true(text != NULL);

	if (*text == '\0')
	{
		char *rezbuf = calloc(1, 1);
		rezbuf[0] = '\0';
		return rezbuf;
	}

	// const int text_size = _countof(text);
	char *front_ = 0, *back_ = 0;

	// if requested set back_ pointer
	// to the end of the buffer position
	if (!zero_limited_string)
	{
		back_ = (char *)&text[text_size - 1];
	}

	dbj_string_trim(text, &front_, &back_);

	char *rezbuf = to_string(front_, back_ + 1);
	return rezbuf;
}

// return true on success
static bool trim_assert(
	char required_outcome[],
	size_t test_data_index,
	bool zero_delimited_strings)
{
	const char *trim_rezult = trimmer(
		text_test_data[test_data_index],
		strlen(text_test_data[test_data_index]),
		zero_delimited_strings);
	bool outcome_ = (0 == strcmp(required_outcome, trim_rezult));
	free((void *)trim_rezult);
	return outcome_;
};

MunitResult complex_trim_test(const MunitParameter params[], void *data)
{

	// the classic use case is trimming spaces
	// from zero delimited string literals
	// using default triming policy ( char == ' ')
	dbj_current_string_trim_policy = dbj_is_space;

	// using zero delimited strings
	// results are predictable
	munit_assert_true(
		trim_assert(target, 0, true));
	munit_assert_true(
		trim_assert(target, 1, true));
	munit_assert_true(
		trim_assert(target, 2, true));
	munit_assert_true(
		trim_assert(" ", 3, true));
	munit_assert_true(
		trim_assert("", 4, true));

	// using the whole buffer
	// that is: NON zero limited strings?
	// That is fine, *if* we change the policy
	// as the first current char might be EOS
	// when we start moving the back_ pointer
	// this policy will also provoke moving over anything
	// that is not alphanum but in a more
	// locale resilient manner
	dbj_current_string_trim_policy = dbj_move_to_alnum;

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
	const char *r_0 = trimmer(text_test_data[3], strlen(text_test_data[3]), false);
	// same is for empty string input
	const char *r_1 = trimmer(text_test_data[4], strlen(text_test_data[3]), false);

	free((void *)r_0);
	free((void *)r_1);

	return MUNIT_OK;
}

/* --------------------------------------------------------------------- */
MunitResult simple_string_trim_test(const MunitParameter p[], void *d)
{
	dbj_current_string_trim_policy = dbj_move_to_alnum;
	static char *text = "   \n\r\v\t     TEXT   \n\r\v\t     ";
	static char *trimmed_text = "TEXT";

	char *str_ = 0;
	char *front_ = 0;
	char *back_ = 0;

	dbj_string_trim(text, &front_, &back_);
	// back_ is now pointing to the last char inside the trimmed area
	// not to the 'one after' as is the STL concept
	str_ = to_string(front_, back_ + 1);

	munit_assert_string_equal(str_, trimmed_text);
	free(str_);
	str_ = NULL;
	return MUNIT_OK;
}

/* --------------------------------------------------------------------- */
// #define LEAVE_THE_STARS_IN
inline bool stop_on_star_met(uchar_t c_)
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

MunitResult user_defined_policy(const MunitParameter p[], void *d)
{
	char *front_ = 0;
	char *back_ = 0;
	dbj_current_string_trim_policy = stop_on_star_met;

	dbj_string_trim("   \n\r\v\t     *TEXT*   \n\r\v\t     ", &front_, &back_);

	char *str_ = to_string(front_, back_ + 1);

#ifdef LEAVE_THE_STARS_IN
	munit_assert_string_equal(str_, "*TEXT*");
#else
	munit_assert_string_equal(str_, "TEXT");
#endif

	free(str_);
	str_ = NULL;

	return MUNIT_OK;
}

/* --------------------------------------------------------------------- */
MunitResult trimings_and_errors(const MunitParameter p[], void *d)
{
	char *front_ = 0;
	char *back_ = 0;
	dbj_current_string_trim_policy = dbj_move_to_alnum;

	// SINGULARITY: empty non NULL string
	dbj_string_trim("", &front_, &back_);
	// RESPONSE: front_ == back_ == NULL
	munit_assert_null(front_);
	munit_assert_null(back_);

	// SINGULARITY: single alphanum char string
	dbj_string_trim("*", &front_, &back_);
	// RESPONSE: front_ == back_  == & input[0]
	munit_assert_ptr_equal(front_, back_ );
	munit_assert_uchar( *front_, == , '*');

	front_ = back_ = 0;
	// SINGULARITY: single whitespace char string
	dbj_string_trim("\v", &front_, &back_);
	// RESPONSE: same as if sigle char is alnum
	munit_assert_ptr_equal(front_, back_ );
	munit_assert_uchar( *front_, == , '\v');

	front_ = back_ = 0;
	// SINGULARITY: 2 space char string, both whitespace
	dbj_string_trim("\v\r", &front_, &back_);
	// RESPONSE: front == back == input[1]
	munit_assert_ptr_equal(front_, back_ );

	front_ = back_ = 0;
	// SINGULARITY: input made of all to be trmed out chars
	dbj_string_trim("\v\r \n\t  \f  ", &front_, &back_);
	// RESPONSE: front == back == input[1]
	munit_assert_ptr_equal(front_, back_ );

	// WARNING: no required delimiters on both ends
	// RESPONSE: seems to work as if there are
	front_ = back_ = 0;
	dbj_string_trim("TEXT", &front_, &back_);

	char *str_ = to_string(front_, back_ + 1);

	munit_assert_string_equal(str_, "TEXT");

	free(str_);
	str_ = NULL;

	return MUNIT_OK;
}