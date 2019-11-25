
#include "../dbjclib_core.h"
#include "../dbj_sll/dbj_sll.h"


void test_dbj_sll(FILE * fp_, const char* what_to_append, size_t how_many_times, bool verbose)
{
	dbj_sll_node* head_ = dbj_sll_make_head();

	while (1 < how_many_times--) dbj_sll_append(head_, what_to_append);

	if (verbose) {
		fprintf(fp_,"\nDBJ SLL dump");
		dbj_sll_foreach(head_, dbj_sll_node_dump_visitor);
		fprintf(fp_,"\n");
	}

	assert(0 == strcmp(dbj_sll_remove_tail(head_)->data, what_to_append));
	dbj_sll_erase(head_);
	if (verbose) {
		fprintf(fp_,"\nHead after SLL erasure");
		dbj_sll_node_dump_visitor(head_);
	}
	assert(true == is_dbj_sll_empty(head_));

	DBJ_UNUSED unsigned long k1 = dbj_sll_append(head_, "Abra")->key;
	DBJ_UNUSED unsigned long k2 = dbj_sll_append(head_, "Ka")->key;
	DBJ_UNUSED unsigned long k3 = dbj_sll_append(head_, "Dabra")->key;

	(void)(sizeof(k1)); (void)(sizeof(k3)); // combat no warning hassle

	dbj_sll_node* node_ = dbj_sll_find(head_, k2);
	assert(0 == strcmp(node_->data, "Ka"));
	assert(false == is_dbj_sll_empty(head_));
	dbj_sll_erase_with_head(head_);
}

int dbj_sll_test(FILE* fp_) {
	
	void test_dbj_sll(fp_,"BIMBILI BAMBILI BUMBILI", 0xFF, true)
}
