
#define DBJ_CHAR_RANGE_IMPLEMENTATION 1
#include "../dbj_chr_rng/dbj_chr_rng.h"

#include "munit/munit.h"

// specimen starts from 1
// for "1234567890", sub(5,7) yields '567'
#define SPECIMEN "1234567890"
/* ----------------------------------------------------------------------- */
MunitResult core_tests(const MunitParameter params[], void* data)
{
	dbj_chr_rng dbjs_1 = dbj_chr_rng_assign(SPECIMEN);
	munit_assert_true(dbj_valid_chr_range(&dbjs_1));
	munit_assert_true(10 == dbj_chr_rng_len(&dbjs_1));

	dbj_chr_rng dbjs_2 = dbj_chr_rng_assign(SPECIMEN SPECIMEN);
	munit_assert_true(dbj_valid_chr_range(&dbjs_2));
	munit_assert_true(20 == dbj_chr_rng_len(&dbjs_2));

	munit_assert_false(dbj_chr_rng_compare(&dbjs_1, &dbjs_2));

	dbj_chr_rng dbjs_3 = dbj_remove_substring(&dbjs_2, &dbjs_1);

	dbj_chr_rng_free(&dbjs_1);
	dbj_chr_rng_free(&dbjs_2);
	dbj_chr_rng_free(&dbjs_3);

	return MUNIT_OK;
}
/* ----------------------------------------------------------------------- */
MunitResult sv_test(const MunitParameter params[], void* data)
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
