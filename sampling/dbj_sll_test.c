/* µnit is MIT-licensed */
#include "munit/munit.h"

#include "../dbjclib_core.h"
#include "../dbj_sll/dbj_sll.h"


static void test_dbj_sll(const char* what_to_append, size_t how_many_times )
{
	dbj_sll_node* head_ = dbj_sll_make_head();

	while (1 < how_many_times--) dbj_sll_append(head_, what_to_append);

/*
Debugging messages, are hidden by default (see the --log-visible option).
https://nemequ.github.io/munit/#--log-visible

dbj_sll_node_dump_visitor default stream is stderr
*/
		munit_logf(MUNIT_LOG_DEBUG,"DBJ SLL dump");
		dbj_sll_foreach(head_, dbj_sll_node_dump_visitor);

	munit_assert_true(0 == strcmp(dbj_sll_remove_tail(head_)->data, what_to_append));
	dbj_sll_erase(head_);

		munit_logf(MUNIT_LOG_DEBUG,"Head after SLL erasure");
		dbj_sll_node_dump_visitor( head_);

	munit_assert_true(true == is_dbj_sll_empty(head_));

	unsigned long k1 = dbj_sll_append(head_, "Abra")->key;
	unsigned long k2 = dbj_sll_append(head_, "Ka")->key;
	unsigned long k3 = dbj_sll_append(head_, "Dabra")->key;

	DBJ_UNUSED(k1);
	DBJ_UNUSED(k2);
	DBJ_UNUSED(k3);

	dbj_sll_node* node_ = dbj_sll_find(head_, k2);
	munit_assert_true(0 == strcmp(node_->data, "Ka"));
	munit_assert_true(false == is_dbj_sll_empty(head_));
	dbj_sll_erase_with_head(head_);
}

MunitResult
dbj_sll_test(const MunitParameter params[], void *data)
{
  (void)params;
  (void)data;
	
	test_dbj_sll("BIMBILI BAMBILI BUMBILI", 0xF );
  return MUNIT_OK;
}
