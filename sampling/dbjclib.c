/* µnit is MIT-licensed */
#include "munit/munit.h"

/* This is just to disable an MSVC warning about conditional
 * expressions being constant, which you shouldn't have to do for your
 * code.  It's only here because we want to be able to do silly things
 * like assert that 0 != 1 for our demo. */
#if defined(_MSC_VER)
#pragma warning(disable : 4127)
#endif

/* Tests are functions that return void, and take a single void*
 * parameter.  We'll get to what that params is later. 
 */
/**********************************************************************/
extern MunitResult dbj_string_trim_test(const MunitParameter params[], void *data);
extern MunitResult dbj_string_list_performance(const MunitParameter params[], void *data);
extern MunitResult dbj_string_list_precision(const MunitParameter params[], void *data);
extern MunitResult dbj_sll_test(const MunitParameter params[], void *data);
extern MunitResult dbj_fb_string_test(const MunitParameter params[], void *data);

/**********************************************************************/
/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest test_suite_tests[] = {
    {(char *)"dbj string trim test", dbj_string_trim_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},

    {(char *)"dbj string list performance", dbj_string_list_performance, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"dbj string list precision", dbj_string_list_precision, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},

    {(char *)"dbj front back string test", dbj_fb_string_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"dbj single linked lst test",       dbj_sll_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    
    /* MUST! Mark the end of the array with an entry where the test function is NULL */
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite test_suite =
    {(char *)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

/* This is only necessary for EXIT_SUCCESS and EXIT_FAILURE, which you
 * *should* be using but probably aren't (no, zero and non-zero don't
 * always mean success and failure).  I guess my point is that nothing
 * about µnit requires it. */
#include <stdlib.h>
/**********************************************************************/
int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
/* DBJ added
   not just the presence of the ANSICON environment variable
   is enough ...
  */
#if defined(_MSC_VER)
  system("@chcp 65001 >NUL");
#endif
  /* That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void *)NULL, argc, argv);
}
