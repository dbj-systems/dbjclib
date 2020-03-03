/* µnit is MIT-licensed */
#include "munit/munit.h"

/* This is just to disable an MSVC warning about conditional
 * expressions being constant, which you shouldn't have to do for your
 * code.  It's only here because we want to be able to do silly things
 * like assert that 0 != 1 for our demo. */
#if defined(_MSC_VER)
#pragma warning(disable : 4127)
#endif

/* DBJ: This is a lot of code for 5 unit tests */
/**********************************************************************/
extern MunitResult dbj_string_trim_test(const MunitParameter params[], void *data);
extern MunitResult dbj_string_list_performance(const MunitParameter params[], void *data);
extern MunitResult dbj_string_list_precision(const MunitParameter params[], void *data);
extern MunitResult dbj_sll_test(const MunitParameter params[], void *data);
extern MunitResult dbj_fb_string_test(const MunitParameter params[], void *data);

/**********************************************************************/
#define DBJ_MUNIT_TEST_ADD( TITLE_, FP_) {(char *)TITLE_, FP_, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
#define DBJ_MUNIT_TEST_EOL  {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}

#define DBJ_MUNIT_SUITE_EOL { NULL, NULL, 0, 0, MUNIT_SUITE_OPTION_NONE }
#define DBJ_MUNIT_SUITE_ADD( TITLE_, AP_) {(char *)TITLE_, AP_, NULL, 1, MUNIT_SUITE_OPTION_NONE} 
/**********************************************************************/
/* first need to create arrays of tets, or just one but that is not practical */
static MunitTest string_trim_tests[] = {
    DBJ_MUNIT_TEST_ADD("/basic test", dbj_string_trim_test),
    DBJ_MUNIT_TEST_EOL };

static MunitTest string_list_tests[] = {
    DBJ_MUNIT_TEST_ADD("/performance", dbj_string_list_performance),
    DBJ_MUNIT_TEST_ADD("/precision", dbj_string_list_precision),
    DBJ_MUNIT_TEST_EOL};

static MunitTest fb_string_tests[] = {
    DBJ_MUNIT_TEST_ADD("/basic", dbj_fb_string_test),
    DBJ_MUNIT_TEST_EOL};

static MunitTest sll_tests[] = {
    DBJ_MUNIT_TEST_ADD("/basic", dbj_sll_test),
    DBJ_MUNIT_TEST_EOL};

static MunitSuite suites_array[] = { 
  DBJ_MUNIT_SUITE_ADD("/string trim tests", string_trim_tests) ,
  DBJ_MUNIT_SUITE_ADD("/string list tests", string_list_tests),
  DBJ_MUNIT_SUITE_ADD("/fb string tests", fb_string_tests),
  DBJ_MUNIT_SUITE_ADD("/single linked list tests", sll_tests),
  DBJ_MUNIT_SUITE_EOL
  };

static MunitSuite top_collection =
    {(char *)"dbjCLIB", NULL, suites_array , 1, MUNIT_SUITE_OPTION_NONE};

/**********************************************************************/
int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
/* DBJ added for WIN10 shenanigans
   not just the presence of the ANSICON environment variable
   is enough for color escape codes to show as colors
  */
#if defined(_MSC_VER)
  system("@chcp 65001 >NUL");
#endif
  /* That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&top_collection, (void *)NULL, argc, argv);
}

