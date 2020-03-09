
/*
(c) Copyright 2018-2019 by dbj@dbj.org

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http ://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include "../dbjclib_core.h"
#include "dbj_hsl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <assert.h>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4505 )
#endif

/*already in this lib*/
/* char* dbj_strdup(const char* s); */

/* for the DBJ SLL key making */
/* the djb2 from: http://www.cse.yorku.ca/~oz/hash.html */
static unsigned long dbj_hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

/********************************************************/
dbj_hsl_node * dbj_hsl_node_new()
{
	dbj_hsl_node * new_
		= (dbj_hsl_node *)malloc(sizeof(dbj_hsl_node));
	if (new_) {
		new_->key = 0;
		new_->data = 0;
		new_->next = 0;
	}
	else {
		errno = ENOMEM;
	}
	return new_;
}
/********************************************************/
dbj_hsl_node * dbj_hsl_make_head() {
	dbj_hsl_node * _head_ = dbj_hsl_node_new();
	if (_head_) {
		_head_->key = DBJ_SLL_HEAD_KEY;
		_head_->data = 0;
		_head_->next = 0;
	}
	/* head node has no data it is easy to delete */
	return _head_;
	/* ENOMEM has been set if NULL */
}
/********************************************************/
dbj_hsl_node * dbj_hsl_tail(dbj_hsl_node * head_)
{
	/* get the head */
	dbj_hsl_node * walker_ = head_;
	while (walker_) {
		if (!walker_->next) break;
		walker_ = walker_->next;
	}
	return walker_;
}
/********************************************************/
/*  erase the whole list
    keep the head
*/
void dbj_hsl_erase(dbj_hsl_node * head_)
{
	dbj_hsl_node * current_ = head_->next;
	dbj_hsl_node * temp_ = 0;
	while (current_) {
		temp_ = current_->next;
		if (current_->data) { free(current_->data); current_->data = 0; }
		free(current_);
		current_ = temp_;
	};
	/* make the head aware there are no nodes left  */
	head_->next = 0;
}

/********************************************************/
/*
	Find by key is the default
	to find by string one can implement the visitor
*/
dbj_hsl_node * dbj_hsl_find(dbj_hsl_node * head_, unsigned long k_)
{
	dbj_hsl_node * walker_ = head_->next;
	while (walker_) {
		if (walker_->key == k_) return walker_;
		walker_ = walker_->next;
	}
	return NULL;
}

/********************************************************/
size_t dbj_hsl_count(dbj_hsl_node * head_)
{
	size_t count_ = 0;
	/* get and skip the head */
	dbj_hsl_node * walker_ = head_->next;
	while (walker_) {
		++count_;
		walker_ = walker_->next;
	}
	return count_;
}
/********************************************************/
/*
return the newly made and appended node
str argument is copied and thus has to
be freed sometimes later
key is generated as the hash() of the str_argument
*/
dbj_hsl_node *
dbj_hsl_append(dbj_hsl_node * head_, const char * str_)
{
	assert(head_);

	/* make new node */
	dbj_hsl_node * new_node = dbj_hsl_node_new();
	new_node->data = _strdup(str_);
	new_node->key = dbj_hash((unsigned char *)str_);

	/* append it as new tail */
	dbj_hsl_node * tail_node = dbj_hsl_tail(head_);
	tail_node->next = new_node;
	return new_node;
}

/********************************************************/
/* sll visitors                                         */
/********************************************************/
static FILE * dbj_hsl_output_target_ = (FILE*)0 ;
/* if no other target then stderr will be used */
#define default_output_target stderr

void set_dbj_hsl_output_target(FILE* file_ )
{
	dbj_hsl_output_target_ = file_;
}


bool dbj_hsl_node_dump_visitor(dbj_hsl_node * node_)
{

	FILE* target_ = (dbj_hsl_output_target_ ? dbj_hsl_output_target_ : default_output_target );
	
	assert(node_);

	fprintf( target_, "\n\n%p", node_);
	fprintf( target_, "\n--------------------------------------");
	fprintf( target_, "\nKey: %ld", node_->key);
	fprintf( target_, "\nStr: [%s]", (node_->data ? node_->data : "NULL"));
	fprintf( target_, "\nNext: %p", (node_->next ? node_->next : 0x0));
	/* return false as a signal NOT to stop */
	return false;
}

/*
this is where we can clearly see the disdvantage of singly linked lists
vs doubly linked lists.

but SLL's simplicity more than compesates for this
SLL lists are short and todays machines are fast
so the algorithms like the one bellow are OK in normal situations
*/
static bool dbj_hsl_remove_tail_visitor(dbj_hsl_node * node_)
{
	if (!node_->next) return true; /* stop */

	/* is the next one the tail? */
	if (!node_->next->next) {
		/* next one is the tail */
		free(node_->next->data);
		free(node_->next);
		node_->next = 0;
		return true; /* stop */
	}
	return false; /* proceed */
}

/* return the new tail or 0 if list is empty */
dbj_hsl_node * dbj_hsl_remove_tail(dbj_hsl_node * head_)
{
	assert(head_);
	if (is_dbj_hsl_empty(head_)) return 0;
	return dbj_hsl_foreach(head_, dbj_hsl_remove_tail_visitor);
}

/********************************************************/

#ifdef _MSC_VER
#pragma warning( pop )
#endif
