
#include "munit/munit.h"
#include "../dbj_fb_string/dbj_front_back_string.h"

MunitResult
dbj_fb_string_test(const MunitParameter params[], void *data)
{
  (void)params;
  (void)data;
// specimen starts from 1
// thus sub(5,7) is '567'
#define SPECIMEN "1234567890"

  // remember: no memory is allocated for the strings viewed
  // thus be carefull to keep them arround
  dbj_string *dbj_str_1 = dbj_string_view("12456", 3, 5); /* should make "456" dbj_string */
  dbj_string *dbj_str_2 = dbj_string_view("45612", 1, 3); /* should make "456" dbj_string */

  munit_assert_true(
      dbj_string_compare(dbj_str_1, dbj_str_2));

  /* remember: fronts are not zero terminated strings */
  munit_assert_string_not_equal(
      dbj_str_1->front, dbj_str_2->front);

  dbj_string_free(dbj_str_1);
  dbj_string_free(dbj_str_2);
  return MUNIT_OK;
#undef SPECIMEN
}