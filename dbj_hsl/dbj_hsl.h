
/*
(c) Copyright 2018-2020 by dbj@dbj.org -- CC BY SA 4.0

DBJ HSL -- Hashed String List

Singly Linked List of strings
Specialised non-generic , one head per thread solution
Each node has a key which is a hash of the string it also has
*/

#pragma once

#ifndef DBJ_SLL
#define DBJ_SLL

#include <stdbool.h>
#include <stdio.h>

#ifdef __clang__
/*
http://clang.llvm.org/docs/UsersManual.html#controlling-diagnostics-in-system-headers
*/
#pragma clang system_header
#endif /* __clang__ */

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct dbj_hsl_node dbj_hsl_node;

	/*
	we cast to this and calculate the key as the hash of the casting outcome
	then we use that key to find things in the list
	*/
	typedef unsigned char dbj_hsl_data_type ;

#define DBJ_SLL_HEAD_KEY ((unsigned long)-1)

	struct dbj_hsl_node {
		unsigned long key;
		/*
		 the above is usually the hash of the data bellow
		 this it is unsigned long
		 see the dbj_hash and the link in its comment
		*/
		char* data;
		dbj_hsl_node* next;
	} ;

	inline dbj_hsl_node* dbj_hsl_next(dbj_hsl_node* current_)
	{
		assert(current_);
		return current_->next;
	}
	inline char const* dbj_hsl_data(dbj_hsl_node* current_)
	{
		assert(current_);
		return current_->data;
	}
	inline unsigned long dbj_hsl_key(dbj_hsl_node* current_)
	{
		assert(current_);
		return current_->key;
	}

	inline bool is_dbj_hsl_head(dbj_hsl_node* head_) {
		assert(head_);
		if (head_) {
			// head_->data we might use in the future
			return (head_->key == DBJ_SLL_HEAD_KEY);
		}
		return false;
	}

	inline bool is_dbj_hsl_empty(dbj_hsl_node* head_)
	{
		assert(is_dbj_hsl_head(head_));
		return head_->next == 0;
	}

	/* return the tail node */
	dbj_hsl_node* dbj_hsl_tail(dbj_hsl_node* /*head_*/);

	dbj_hsl_node* dbj_hsl_make_head();

	/********************************************************/
	/* */
	dbj_hsl_node* dbj_hsl_append(dbj_hsl_node* head_, const char* str_);

	/********************************************************/
	/*  erase the whole list
		keep the head 
	*/
	void dbj_hsl_erase(dbj_hsl_node* /*head_*/);

	/********************************************************/
	/*  erase the whole list
		free the head too
	*/
	inline void dbj_hsl_erase_with_head(dbj_hsl_node** head_)
	{
		dbj_hsl_erase(*head_);
		free(*head_); /* head has no data */
		*head_ = 0;
	}

	dbj_hsl_node* dbj_hsl_find(dbj_hsl_node* head_, unsigned long k_);

	dbj_hsl_node* dbj_hsl_remove_tail(dbj_hsl_node* head_);
	   
	size_t dbj_hsl_count(dbj_hsl_node* head_);

	/********************************************************/
	/*
	 visitor function to each node
	 visitation is stopped when visitor returns true
	 remember: head is never used it is just an anchor
	 return the pointer to the last node visited
	 visitor function signature:

	 bool (*visitor)(dbj_hsl_node *)

	NOTE: most (if not all) SLL operations
	can be implemented as visitors
	*/
	inline dbj_hsl_node* dbj_hsl_foreach
	(dbj_hsl_node* head_, bool(*visitor)(dbj_hsl_node*))
	{
		dbj_hsl_node* walker_ = head_->next;
		while (walker_) {
			if (visitor(walker_)) return walker_;
			walker_ = walker_->next;
		}
		/* not found */
		return walker_;
	}

	/********************************************************/
	/* sll visitors                                         */
	/********************************************************/

	void set_dbj_hsl_output_target ( FILE * ) ;
	/* dumps the sll into the file */
	/* if no other target then stderr will be used */
	bool dbj_hsl_node_dump_visitor(dbj_hsl_node* node_);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // ! DBJ_SLL
