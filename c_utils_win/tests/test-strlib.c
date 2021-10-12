/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// #include <utils/strlib.h>

#include "test.h"

// DBJ: this is the one "test string", I made it static
// due to its design it has to be global, on the level of compilation unit
static STRING_DEF(test_str, 8);

void test_print_info_string(const char *msg, string_t *s)
{
	printf( VT100_BLUE_BOLD  "\t[%s] s->len: %zu s->max_len: %zu s->buf: %s\n" VT100_RESET,
		msg, s->len, s->max_len, s->buf);
}

// DBJ: -1 always means error for the strlib
int test_check_str(string_t *s, const char *str)
{
	if (strcmp(s->buf, str) != 0)
		return -1;

	return 0;
}

// DBJ: -1 always means error for the strlib
int test_str_printf()
{
	int ret;
// DBJ: second arg is "mode", c == create, f == fill, a == append
// one can combine into: "ca", "fa" it seems
	SX("%d", ret = string_printf(&test_str, "c", "age: %d", 10)) ;

	if (ret <= 0 || test_check_str(&test_str, "age: 10")) {
		printf("\tret: %d", ret);
		test_print_info_string("string_printf: create", &test_str);
		return -1;
	}

	SX("%d", ret = string_printf(&test_str, "a", " id: %d", 101) ) ;

	if (ret != -1) {
		printf("\tret: %d", ret);
		test_print_info_string("string_printf: create", &test_str);
		return -1;
	}

	return 0;
}

int test_str_copy()
{
	int ret;

	SX("%d", ret = string_copy(&test_str, "c", "1234567890", 10));
	if (ret != -1) {
		test_print_info_string("Create excess", &test_str);
		return -1;
	}

	SX("%d", ret = string_copy(&test_str, "cf", "1234567890", 10));
	if (ret != 8) {
		test_print_info_string("Create fill", &test_str);
		return -1;
	}

	SX("%d",ret = string_copy(&test_str, "c", "1234567", 7));
	if (ret != 7) {
		test_print_info_string("Create", &test_str);
		return -1;
	}

	SX("%d",ret = string_copy(&test_str, "a", "1", 1));
	if (ret != 1) {
		test_print_info_string("Append", &test_str);
		return -1;
	}

	SX("%d", ret = string_copy(&test_str, "a", "123", 3));
	if (ret != -1) {
		test_print_info_string("Append after full", &test_str);
		return -1;
	}

	return 0;
}

TEST_DEF(strlib)
{
	TEST_MOD_INIT();

	TEST_MOD_EXEC(test_str_copy());
	TEST_MOD_EXEC(test_str_printf());

	TEST_MOD_REPORT();
}
