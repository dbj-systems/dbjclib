/* µnit is MIT-licensed, but for this file and this file alone: */
#include "munit/munit.h"

#include "../dbj_fb_string/dbj_front_back_string.h"

/* This is just to disable an MSVC warning about conditional
 * expressions being constant, which you shouldn't have to do for your
 * code.  It's only here because we want to be able to do silly things
 * like assert that 0 != 1 for our demo. */
#if defined(_MSC_VER)
#pragma warning(disable : 4127)
#endif

/* Tests are functions that return void, and take a single void*
 * parameter.  We'll get to what that parameter is later. */
static MunitResult
dbj_front_back_string_test(const MunitParameter params[], void *data)
{
  (void)params;
  (void)data;
// specimen starts from 1
// thus sub(5,7) is '567'
#define SPECIMEN "1234567890"

  dbj_string *dbj_str_1 = dbj_string_from("12456", 3, 5); /* should make "456" dbj_string */
  dbj_string *dbj_str_2 = dbj_string_from("45612", 1, 3); /* should make "456" dbj_string */

  munit_assert_true(
      dbj_string_compare(dbj_str_1, dbj_str_2));

  munit_assert_string_equal(
      dbj_str_1->front, dbj_str_2->front);

  dbj_string_free(dbj_str_1);
  dbj_string_free(dbj_str_2);
  return MUNIT_OK;
  #undef SPECIMEN
}

/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest test_suite_tests[] = {
    {(char *)"dbj_front_back_string_test", dbj_front_back_string_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite test_suite = 
{ (char *)"",    test_suite_tests,   NULL,    1,    MUNIT_SUITE_OPTION_NONE};

/* This is only necessary for EXIT_SUCCESS and EXIT_FAILURE, which you
 * *should* be using but probably aren't (no, zero and non-zero don't
 * always mean success and failure).  I guess my point is that nothing
 * about µnit requires it. */
#include <stdlib.h>

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
/* DBJ added
   not just the presence of the ANSICON environment variable
   is enough ...
  */
#if defined(_MSC_VER)
  system("@chcp 65001 >NUL");
#endif
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void *)"µnit", argc, argv);
}
