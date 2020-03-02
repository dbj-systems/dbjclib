
#include "munit/munit.h"
#include "../dbj_string_list/dbj_string_list.h"

#define SPECIMEN u8"ひらがな"

typedef struct TEST_DBJ_DYNAMIC_METADATA_ {
	const unsigned number_of_iterations;
	const unsigned words_to_append;
	const char * word;
} TEST_DBJ_DYNAMIC_METADATA;

static void worker(TEST_DBJ_DYNAMIC_METADATA * test_descriptor)
{
	/* make the string list */
	dbj_string_list_type head_ = dbj_string_list_new();
	/* populate */
	unsigned int k;
	for (k = 0; k < test_descriptor->words_to_append; k++) {
		// on error head_ will be null
		head_ = dbj_string_list_append(head_, (char *)test_descriptor->word);
			munit_assert_true( head_ != NULL );
	}
	/* iterate and use */
	uint16_t size_ = dbj_string_list_size(head_);
	for (uint16_t idx_ = 0; idx_ < size_; idx_++) {
		dbj_string_list_value_type one_string = dbj_string_list_at_index(idx_, head_, size_);
		munit_assert_string_equal ( one_string, test_descriptor->word );
	}
	/* free */
	head_ = dbj_string_list_reset(head_);
	dbj_string_list_free(head_);
}

MunitResult dbj_string_list_performance(const MunitParameter params[], void *data)
{
	TEST_DBJ_DYNAMIC_METADATA test_descriptor =
	{
	.number_of_iterations = 0xFF ,
	.words_to_append = 0xF, /*dbj_string_list_max_capacity - 1,*/
	.word = SPECIMEN /*"12345678"*/
	};

	unsigned int k = 0;
	for (k = 0; k < test_descriptor.number_of_iterations; k++)
	{
		worker(&test_descriptor);
	}
	return MUNIT_OK ;
}

MunitResult dbj_string_list_precision(const MunitParameter params[], void *data)
{
	/* make the string list */
	dbj_string_list_type head_ = dbj_string_list_new();
	munit_assert_true(0 == dbj_string_list_size(head_));
	/* populate */
	dbj_string_list_append(head_, "Abra");
	dbj_string_list_append(head_, "Ca");
	dbj_string_list_append(head_, "Dabra");

	uint16_t size_ = dbj_string_list_size(head_);
	munit_assert_true( 3 == size_);

	/* must not free indivodual strings */
	char const* abra_  = dbj_string_list_at_index(0, head_, size_);
	char const* ca_		= dbj_string_list_at_index(1, head_, size_);
	char const* dabra_	= dbj_string_list_at_index(2, head_, size_);

	(void)abra_; (void)ca_; (void)dabra_;

	dbj_string_list_free(head_);

	return MUNIT_OK ;
}

