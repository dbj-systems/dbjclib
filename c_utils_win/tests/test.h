
#ifndef _TEST_H_
#define _TEST_H_
/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __clang__
#pragma clang system_header
#endif  // __clang__

#include <stdio.h>
#include <time.h>

#include "../dbj_common.h"

// to be used from DBJ_PROMPT
#define DBJ_TXT_LINE ------------------------------------------

// #define printf(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)

#define TEST_DEF(m)     void do_test_##m(test_t* ctx, test_result_t* res)
#define TEST_MOD_INIT() test_result_t _test = {0, 0}
// DBJ: added printing the exact function to be executed

#define TEST_MOD_EXEC(f)                               \
  do {                                                 \
    printf(VT100_GREEN_BOLD "\n%s" VT100_RESET, (#f)); \
    if (f == 0) _test.pass++;                          \
    _test.total++;                                     \
  } while (0)

#define TEST_MOD_REPORT() \
  res->pass = _test.pass; \
  res->total = _test.total;
#define TEST_MOD(m) \
  { STR(m), do_test_##m }
#define TEST_MOD_SENTINEL \
  { NULL, NULL }

#define TEST_MOD_READ_FILE(f, b, s) read_file_with_test_data(ctx, f, b, s);

typedef struct {
  int total;
  int pass;
} test_result_t;

typedef struct {
  char* inputdir;
  time_t start_time;
  time_t end_time;
  int total;
  int pass;
} test_t;

typedef struct {
  const char* name;
  void (*runner)(test_t* ctx, test_result_t* result);
} test_module_t;

// void read_file_with_test_data(test_t *ctx, char *filename, char **buf, size_t
// *size);
void read_file_with_test_data(test_t ctx[static 1],
                              const char filename[static 1], char** buf,
                              size_t size[static 1]);

#endif
