/*
this is not part of the library.
include in your project made for testing
*/
#include "../dbj_string_list/dbj_string_list.h"
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

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
		head_ = dbj_string_list_append(head_, (char *)test_descriptor->word);
	}
	/* iterate and use */
	uint16_t size_ = dbj_string_list_size(head_);
	for (uint16_t idx_ = 0; idx_ < size_; idx_++) {
		dbj_string_list_value_type one_string = dbj_string_list_at_index(idx_, head_, size_);
		assert( ! strcmp( one_string, test_descriptor->word) );
	}
	/* free */
	head_ = dbj_string_list_reset(head_);
	dbj_string_list_free(head_);
}

static void dbj_string_list_performance( FILE * fp_ )
{
	TEST_DBJ_DYNAMIC_METADATA test_descriptor =	
	{ 
	.number_of_iterations = BUFSIZ * 10,
	.words_to_append = BUFSIZ,
	.word = "12345678" 
	};

	unsigned int k = 0;

	time_t start = clock() ;
	for (k = 0; k < test_descriptor.number_of_iterations; k++)
	{
		worker(&test_descriptor);
	}
	time_t finish = clock();

	float elapsed_time_sec = (finish - start) / (float)(CLOCKS_PER_SEC) ;
	// double elapsed_time_msec = 1000.0 * (elapsed_time_sec);

	fprintf( fp_ ,
		"\n\ndbj list test"
		"\n%06d iterations,"
		"\nhas taken: %06.3f sec"
		"\nEach iteration added the word '%s',"
		"\n%06l times, and then destroyed the list\n\n",
		BUFSIZ, 
		elapsed_time_sec,
		test_descriptor.word, 
		test_descriptor.number_of_iterations);
}

void dbj_string_list_precision( FILE * fp_ )
{
	/* make the string list */
	dbj_string_list_type head_ = dbj_string_list_new();
	{
		uint16_t size_ = dbj_string_list_size(head_);
	}
	/* populate */
	head_ = dbj_string_list_append(head_, "Abra");
	head_ = dbj_string_list_append(head_, "Ca");
	head_ = dbj_string_list_append(head_, "Dabra");

	uint16_t size_ = dbj_string_list_size(head_);

	char const* abra_	= dbj_string_list_at_index(0, head_, size_);
	char const* ca_		= dbj_string_list_at_index(1, head_, size_);
	char const* dabra_	= dbj_string_list_at_index(2, head_, size_);

	dbj_string_list_free(head_);
}

int dbj_string_list_test(FILE* fp_)
{
	dbj_string_list_precision(fp_);
	dbj_string_list_performance( fp_ );
	return 42;
}

