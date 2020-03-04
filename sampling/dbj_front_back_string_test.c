
#include "../dbj_fb_string/dbj_front_back_string.h"
#include "munit/munit.h"

// specimen starts from 1
// for "1234567890", sub(5,7) yields '567'
#define SPECIMEN "1234567890"
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
