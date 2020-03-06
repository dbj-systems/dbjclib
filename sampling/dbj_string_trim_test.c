#include "../dbj_str_trim/dbj_string_trim.h"
#include "munit/munit.h"

// DBJ proposal :
//
// void using namespace ::std;
// means: stop using namespace
/* --------------------------------------------------------------------- */
// testing data
static char target[] = "LINE O FF\n\rTE\v\tXT";
static char *text_test_data[] = {
	"   LINE O FF\n\rTE\v\tXT    ",
	"   LINE O FF\n\rTE\v\tXT",
	"LINE O FF\n\rTE\v\tXT",
	// all spaces singularity
	"     ",
	// empty string singularity
	""};

/* --------------------------------------------------------------------- */

static void trim_proc(	char required_outcome[],	size_t test_data_index)
{
	char* front_ = 0, * back_ = 0;
	valstat_char trim_rezult = dbj_string_trim(text_test_data[test_data_index], &front_, &back_);

	if (is_valstat_error(trim_rezult)) {
		munit_logf(MUNIT_LOG_DEBUG, "%s", trim_rezult.stat);
	}
	else {
		munit_assert_ptr_not_null(trim_rezult.val);
		munit_assert_string_equal(required_outcome, trim_rezult.val);
		free(trim_rezult.val);
	}
};

MunitResult complex_trim_test(const MunitParameter params[], void *data)
{

	// the classic use case is trimming spaces
	// from zero delimited string literals
	// using default triming policy ( char == ' ')
	dbj_current_string_trim_policy = dbj_is_space;

	// using zero delimited strings
	// results are predictable
		trim_proc(target, 0);
		trim_proc(target, 1);
		trim_proc(target, 2);
		trim_proc(" ", 3);
		trim_proc("", 4);

	// using the whole buffer
	// that is: NON zero limited strings?
	// That is fine, *if* we change the policy
	// as the first current char might be EOS
	// when we start moving the back_ pointer
	// this policy will also provoke moving over anything
	// that is not alphanum but in a more
	// locale resilient manner
	dbj_current_string_trim_policy = dbj_move_to_alnum;

		trim_proc(target, 0);
		trim_proc(target, 1);
		trim_proc(target, 2);

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


	valstat_char vstat_returned = dbj_string_trim(text, &front_, &back_);

	munit_assert_true(is_valstat_ok(vstat_returned));

	// back_ is now pointing to the last char inside the trimmed area
	// not to the 'one after' as is the STL concept
	str_ = vstat_returned.val;

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

	valstat_char vstat_returned 
		=  dbj_string_trim("   \n\r\v\t     *TEXT*   \n\r\v\t     ", &front_, &back_);

	munit_assert_true(is_valstat_ok(vstat_returned));

	char* str_ = vstat_returned.val;

#ifdef LEAVE_THE_STARS_IN
	munit_assert_string_equal(str_, "*TEXT*");
#else
	munit_assert_string_equal(str_, "TEXT");
#endif

	free(str_);
	str_ = NULL;

	return MUNIT_OK;
}

/* --------------------------------------------------------------------- 

    struct valstat_char {
	    char * val ; char * stat ;
	};

	valstat_char is returned from dbj_string_trim
	the val pointer is a pointer to newly allocated 
	trimmed result
	we have the responsibility to free it...

*/
MunitResult trimings_and_errors(const MunitParameter p[], void *d)
{
	char *front_ = 0;
	char *back_ = 0;
	dbj_current_string_trim_policy = dbj_move_to_alnum;

	// SINGULARITY: empty non NULL string
	valstat_char vstat_returned = dbj_string_trim("", &front_, &back_);
	// RESPONSE: front_ == back_ == NULL
	munit_assert_null(front_);
	munit_assert_null(back_);
	munit_assert_true( is_valstat_error( vstat_returned)  );

	front_ = back_ = 0;
	// SINGULARITY: single alphanum char string
	vstat_returned = dbj_string_trim("*", &front_, &back_);
	// RESPONSE: front_ == back_  == & input[0]
	munit_assert_ptr_equal(front_, back_ );
	munit_assert_uchar( *front_, == , '*');
	munit_assert_true(is_valstat_error(vstat_returned));

	front_ = back_ = 0;
	// SINGULARITY: single whitespace char string
	vstat_returned = dbj_string_trim("\v", &front_, &back_);
	// RESPONSE: same as if sigle char is alnum
	munit_assert_ptr_equal(front_, back_ );
	munit_assert_uchar( *front_, == , '\v');
	munit_assert_true(is_valstat_error(vstat_returned));

	front_ = back_ = 0;
	// SINGULARITY: 2 space char string, both whitespace
	vstat_returned = dbj_string_trim("\v\r", &front_, &back_);
	// RESPONSE: front == back == input[1]
	munit_assert_ptr_equal(front_, back_ );
	munit_assert_true(is_valstat_error(vstat_returned));

	front_ = back_ = 0;
	// SINGULARITY: input made of all to be trmed out chars
	vstat_returned = dbj_string_trim("\v\r \n\t  \f  ", &front_, &back_);
	// RESPONSE: front == back == input[1]
	munit_assert_ptr_equal(front_, back_ );
	munit_assert_true(is_valstat_error(vstat_returned));

	// WARNING: no required delimiters on both ends
	// RESPONSE: seems to work as if there are
	front_ = back_ = 0;
	vstat_returned = dbj_string_trim("TEXT", &front_, &back_);
	munit_assert_true(is_valstat_ok(vstat_returned));

	// OK means vstat_returned.vstat is pointing to a newly allocated string
	char *str_ = vstat_returned.val ;

	munit_assert_string_equal(str_, "TEXT");

	free(str_);
	str_ = NULL;

	return MUNIT_OK;
}