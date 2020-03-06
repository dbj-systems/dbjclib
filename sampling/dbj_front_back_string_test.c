
#include "../dbj_fb_string/dbj_front_back_string.h"
#include "munit/munit.h"

// specimen starts from 1
// for "1234567890", sub(5,7) yields '567'
#define SPECIMEN "1234567890"
/* ----------------------------------------------------------------------- */
MunitResult core_tests(const MunitParameter params[], void* data)
{
    dbj_string* dbjs_1 = dbj_string_assign(SPECIMEN);
        munit_assert_not_null(dbjs_1);
        munit_assert_true(dbj_valid_string(dbjs_1));
        munit_assert_true( 10 == dbj_string_len(dbjs_1));

    dbj_string* dbjs_2 = dbj_string_assign(SPECIMEN SPECIMEN);
        munit_assert_not_null(dbjs_2);
        munit_assert_true(dbj_valid_string(dbjs_2));
        munit_assert_true(20 == dbj_string_len(dbjs_2));

    munit_assert_false(dbj_string_compare(dbjs_1, dbjs_2));

    dbj_string_free(&dbjs_1);
    dbj_string_free(&dbjs_2);
    
    return MUNIT_OK;
}
/* ----------------------------------------------------------------------- */
MunitResult sv_test(const MunitParameter params[], void *data)
{
  (void)params;
  (void)data;

  // remember: no memory is allocated for the strings viewed
  // thus be carefull to keep them arround

  munit_assert_true(3 == dbj_sv_len(dbj_sv_make(SPECIMEN, 1, 3)));

  munit_assert_true(
      dbj_sv_compare(
          dbj_sv_make(SPECIMEN, 1, 3), dbj_sv_make(SPECIMEN, 1, 3)));

  munit_assert_false(
      dbj_sv_compare(
          dbj_sv_make(SPECIMEN, 1, 3), dbj_sv_make(SPECIMEN, 4, 6)));

  munit_assert_string_not_equal(
      dbj_sv_make(SPECIMEN, 1, 3)->front,
      dbj_sv_make(SPECIMEN, 4, 6)->front);

  munit_assert_string_equal(
      dbj_sv_make(SPECIMEN, 1, 3)->front,
      dbj_sv_make(SPECIMEN, 1, 3)->front);

  return MUNIT_OK;
}

#undef SPECIMEN
