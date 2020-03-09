/* µnit is MIT-licensed */
#include "munit/munit.h"

#include "../dbjclib_core.h"
#include "../dbj_hsl/dbj_hsl.h"


static void test_dbj_hsl( const int input_size_, const char* input_[] )
{
	dbj_hsl_node* head_ = dbj_hsl_make_head();

	int walker_ = 0;
	while (walker_ != input_size_) 
		dbj_hsl_append( head_, input_[walker_++]);

	/*
	Debugging messages, are hidden by default (see the --log-visible option).
	https://nemequ.github.io/munit/#--log-visible

	dbj_hsl_node_dump_visitor default stream is stderr
	*/
	munit_logf(MUNIT_LOG_DEBUG, "DBJ SLL dump");
	dbj_hsl_foreach(head_, dbj_hsl_node_dump_visitor);

	// remove tail returns pointer to the new tail
	dbj_hsl_node* new_tail_ = dbj_hsl_remove_tail(head_);

	// head stays
	dbj_hsl_erase(head_);
	munit_logf(MUNIT_LOG_DEBUG, "Head after SLL erasure");
	dbj_hsl_node_dump_visitor(head_);

	// start from the head of the empty list
	munit_assert_true(true == is_dbj_hsl_empty(head_));

	unsigned long k1 = dbj_hsl_append(head_, "Abra")->key;
	unsigned long k2 = dbj_hsl_append(head_, "Ka")->key;
	unsigned long k3 = dbj_hsl_append(head_, "Dabra")->key;

	dbj_hsl_node* node_1 = dbj_hsl_find(head_, k1);
	munit_assert_true(0 == strcmp(node_1->data, "Abra"));

	dbj_hsl_node* node_2 = dbj_hsl_find(head_, k2);
	munit_assert_true(0 == strcmp(node_2->data, "Ka"));

	dbj_hsl_node* node_3 = dbj_hsl_find(head_, k3);
	munit_assert_true(0 == strcmp(node_3->data, "Dabra"));

	munit_assert_true(false == is_dbj_hsl_empty(head_));

	dbj_hsl_erase_with_head(&head_);
	munit_assert_ptr_null(head_);
}

MunitResult
dbj_hsl_test(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;

	test_dbj_hsl(3, (const char * []){ "BIMBILI", "BAMBILI","BUMBILI"  } );
	return MUNIT_OK;
}
