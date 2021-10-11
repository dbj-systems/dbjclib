/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

 // #include "../dbj_common.h"

#include "test.h"

#include "path-join.h" // dbj
#define DBJ_FILE_READ_ALL_IMPLEMENTATION
#include "dbj_file_read_all.h" // dbj

#define DATE_TIME_STR_LEN 64

static char date_time[DATE_TIME_STR_LEN] = { 0 };

TEST_DEF(circular_buffer);
TEST_DEF(strlib);
TEST_DEF(hashmap);
TEST_DEF(strutils);
// TEST_DEF(filo);
TEST_DEF(slab);
// TEST_DEF(procutils);
// TEST_DEF(workqueue);
// TEST_DEF(bus_server);

static test_module_t c_utils_test_modules[] = {
	TEST_MOD(circular_buffer),
	TEST_MOD(strlib),
	TEST_MOD(hashmap),
	TEST_MOD(strutils),
	// TEST_MOD(filo),
	TEST_MOD(slab),
	// TEST_MOD(procutils),
	// TEST_MOD(workqueue),
	// TEST_MOD(bus_server),
	TEST_MOD_SENTINEL,
};

static void test_execute(test_t* t, test_module_t* tm)
{
	test_result_t result;

	printf("Testing '%s'\n", tm->name);
	tm->runner(t, &result);
	mod_printf("Result: name:'%s' total:%d pass:%d\n",
		tm->name, result.total, result.pass);
	t->pass += result.pass;
	t->total += result.total;
}

static char* time_string(time_t* t)
{
	strftime(date_time, DATE_TIME_STR_LEN, "%d/%m/%y-%H:%M:%S %z", localtime(t));
	return date_time;
}

#ifdef DBJ_CLI_HAS_OPTS

static const char* TEST_HELP_TEXT =
"test-bin [OPTIONS]\n"
"\n"
"OPTIONS:\n"
"  -i   Input files directory\n"
"  -h   Print this help text\n"
"\n";

static void process_cli_opts(test_t* ctx, int argc, char* argv[])
{
	int c;
	int opt_ndx;
	static struct option opts[] = {
		{ "inputdir",   required_argument, NULL,                   'i' },
		{ "help",       no_argument,       NULL,                   'h' },
		{ NULL,         0,                 NULL,                    0  }
	};
	const char* opt_str =
		/* no_argument       */ "h"
		/* required_argument */ "i:"
		;
	while ((c = getopt_long(argc, argv, opt_str, opts, &opt_ndx)) >= 0) {
		switch (c) {
		case 'i':
			if (ctx->inputdir) {
				printf("Cannot pass multiple --inputdir");
				exit(-1);
			}
			ctx->inputdir = safe_strdup(optarg);
			break;
		case 'h':
			printf("%s", TEST_HELP_TEXT);
			exit(0);
			break;
		case '?':
		default:
			printf("%s", TEST_HELP_TEXT);
			exit(-1);
		}
	}

	argc -= optind;
	argv += optind;

	if (argc != 0) {
		printf("Invlaid arguments found\n");
		exit(-1);
	}
}
#endif // DBJ_CLI_HAS_OPTS


void test_read_input_file(test_t ctx[static 1], const char filename[static 1],
	char** buf, size_t size[static 1])
{
	char path[BUFSIZ] = { 0 };
	FILE* fd = 0;

	if ( 0 != path_join(BUFSIZ, path, ctx->inputdir, filename)){
		printf("path_join failed, inputdir: %s, filename: %s\n", ctx->inputdir, filename);
		exit(-1);
	}

	if ((fd = fopen(path, "r")) == NULL) {
		printf("Failed to open %s\n", path);
		exit(-1);
	}

	if (file_read_all(fd, buf, size)) {
		printf("Failed to read contents of %s\n", path);
		exit(-1);
	}

	fclose(fd);
}

int main(int argc, char* argv[])
{
	static char input_folder_path[512] = { 0 };

	unsigned app_folder_rez = dbj_app_folder_subfolder(512, input_folder_path, argv[0], sizeof "input", "input");
	
	DBJ_ASSERT(app_folder_rez == 0);

	test_module_t* tm = 0;
	test_t test = { .inputdir = input_folder_path };

	// memset(&test, 0, sizeof(test_t));
#ifdef DBJ_CLI_HAS_OPTS
	process_cli_opts(&test, argc, argv);
#endif // DBJ_CLI_HAS_OPTS

	// Display test header
	printf("\n"
		"------------------------------------------\n"
		"       goToMain C Unit Test Framework     \n"
		"------------------------------------------\n");

	// Run tests
	test.start_time = time(NULL);
	tm = &(c_utils_test_modules[0]);
	while (tm->name != NULL) {
		test_execute(&test, tm);
		tm++;
	}
	test.end_time = time(NULL);

	// Test summary
	printf("------------------------------------------\n"
		"                Test Summary              \n"
		"------------------------------------------\n");

	printf("Start Time: %s\n", time_string(&test.start_time));
	printf("End Time:   %s\n", time_string(&test.end_time));
	printf("Executed: %d\n", test.total);
	printf("Successful: %d\n", test.pass);
	printf("Result: %s\n\n", (test.total == test.pass) ? "PASS" : "FAIL");
	return 0;
}
