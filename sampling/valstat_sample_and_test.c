/* µnit is MIT-licensed */
#include "../valstat_interop.h"
#include "munit/munit.h"

// VALSTAT used is: valstat_uint64_t
// here we declare and define it
VALSTAT(uint64_t);

// VALSTAT is a struct of two pointers
// & divider_rezult_ is used as 
// VALSTAT instance aggregate initializer
static uint64_t divider_rezult_ = 0;

// we could use this: valstat_type(uint64_t)
// or valstat_uint64_t as we did here
static valstat_uint64_t
divider(uint64_t dividend, uint64_t divisor)
{
	if (0 == divisor)
		return valstat_error(valstat_uint64_t, "invalid_argument: zero divisor");

	if ((dividend) >= UINT16_MAX)
		return valstat_error(valstat_uint64_t, "dividend too large");

	if ((divisor) >= UINT16_MAX)
		return valstat_error(valstat_uint64_t, "divisor too large");

	divider_rezult_ = (dividend / divisor);

	return valstat_ok(valstat_uint64_t, divider_rezult_);
}

MunitResult
dbj_valstat_test(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;

	valstat_uint64_t rez1 = divider(4, 2);

	munit_assert_true(is_valstat_ok(rez1));
	munit_assert_int64(2, == , *rez1.val);

	valstat_uint64_t rez2 = divider(4, 0);

	munit_assert_true(is_valstat_error(rez2));
	munit_assert_not_null(rez2.stat);
	munit_logf(MUNIT_LOG_DEBUG, "%s", rez2.stat);

	return MUNIT_OK;
}