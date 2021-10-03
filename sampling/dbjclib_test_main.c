
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

/* µnit is MIT-licensed */
#include "munit/munit.h"

/* This is just to disable an MSVC warning about conditional
 * expressions being constant, which you shouldn't have to do for your
 * code.  It's only here because we want to be able to do silly things
 * like assert that 0 != 1 for our demo. */
#if defined(_MSC_VER)
#pragma warning(disable : 4127)
#endif

 /*
  DBJ: This is a lot of code for unit tests
  test unit functions are in separate C files
  they are collected here and the whole MUNIT API
  shenanigans are done so we can have them
  separated by our logic when displaying the results

  The outcome is a single console executable with non
  trivial set of CLI arguments that can be used
  or have to be used; it depends on the user requirements
  */
  /**********************************************************************/


extern MunitResult trimings_and_errors(const MunitParameter[], void*);
extern MunitResult user_defined_policy(const MunitParameter[], void*);
extern MunitResult simple_string_trim_test(const MunitParameter[], void*);
extern MunitResult complex_trim_test(const MunitParameter[], void*);

extern MunitResult dbj_string_list_performance(const MunitParameter[], void*);
extern MunitResult dbj_string_list_precision(const MunitParameter[], void*);
extern MunitResult dbj_hsl_test(const MunitParameter[], void*);

extern MunitResult core_tests(const MunitParameter[], void*);
extern MunitResult sv_test(const MunitParameter[], void*);

extern MunitResult dbj_valstat_test(const MunitParameter[], void*);


/*
first need to create arrays of tests, or just one but that is not practical
*/
static MunitTest string_trim_tests[] = {
	DBJ_MUNIT_TEST_ADD("/wrong input", trimings_and_errors),
	DBJ_MUNIT_TEST_ADD("/user defined", user_defined_policy),
	DBJ_MUNIT_TEST_ADD("/simple   ", simple_string_trim_test),
	DBJ_MUNIT_TEST_ADD("/basic test", complex_trim_test),
	DBJ_MUNIT_TEST_EOL };

static MunitTest string_list_tests[] = {
	DBJ_MUNIT_TEST_ADD("/performance", dbj_string_list_performance),
	DBJ_MUNIT_TEST_ADD("/precision", dbj_string_list_precision),
	DBJ_MUNIT_TEST_EOL };

static MunitTest fb_string_tests[] = {
	DBJ_MUNIT_TEST_ADD("/core", core_tests),
	DBJ_MUNIT_TEST_ADD("/basic", sv_test),
	DBJ_MUNIT_TEST_EOL };

static MunitTest sll_tests[] = {
	DBJ_MUNIT_TEST_ADD("/basic", dbj_hsl_test),
	DBJ_MUNIT_TEST_EOL };

static MunitTest valstat_tests[] = {
	DBJ_MUNIT_TEST_ADD("/basic", dbj_valstat_test),
	DBJ_MUNIT_TEST_EOL };


/* define elsewhere */
extern MunitTest playground_tests[];
/*
no need to create array of suites
*/
static MunitSuite suites_array[] = {
  DBJ_MUNIT_SUITE_ADD("/munit playground", playground_tests) ,
  DBJ_MUNIT_SUITE_ADD("/string trim tests", string_trim_tests) ,
  DBJ_MUNIT_SUITE_ADD("/string list tests", string_list_tests),
  DBJ_MUNIT_SUITE_ADD("/fb string tests", fb_string_tests),
  DBJ_MUNIT_SUITE_ADD("/s.linked list tests", sll_tests),
  DBJ_MUNIT_SUITE_ADD("/valstat tests", valstat_tests),
  DBJ_MUNIT_SUITE_EOL
};

/*
now need to create top level suite
*/
static MunitSuite top_collection =
{ (char*)"dbjCLIB", NULL, suites_array , 1, MUNIT_SUITE_OPTION_NONE };

/**********************************************************************/
int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
	/* DBJ added for WIN10 shenanigans
	   not just the presence of the ANSICON environment variable
	   is enough for color escape codes to show as colors
	  */
#if defined(_MSC_VER)
	system("@chcp 65001 >NUL");
#endif
	/* That second argument
	 * is the user_ parameter which will be passed either to the
	 * test or (if provided) the fixture setup function. */
	munit_suite_main(&top_collection, (void*)NULL, argc, argv);

	return _CrtDumpMemoryLeaks();
}

