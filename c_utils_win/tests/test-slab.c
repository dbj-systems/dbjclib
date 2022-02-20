#include "test.h"

// #include <utils/slab.h>

#ifndef ROUND_UP
#define ROUND_UP(x, y) ((x + y - 1) & ~(y - 1))
#endif  // ! ROUND_UP

static int slab_most_basic_test() {

  struct string_ {
    char data[0xFF];
  };

  enum { string_size = sizeof(struct string_) };

  enum { TEST_SLAB_COUNT = 2 };  // can not be 1?
  // int i = 0;

  // array of pointers to the struct allocated
  // struct string_ *p[TEST_SLAB_COUNT] = {0};

  // aka "blob"
  uint8_t slab_space[string_size * TEST_SLAB_COUNT] = {0};

  const unsigned slab_space_size = sizeof(slab_space);

  // the handle to the stack allocator
  slab_t slab;

  int free_slots = slab_init(&slab, string_size, slab_space, slab_space_size);

  if (free_slots < 1) return -1;

  struct string_ *sp_ = 0;

  if (slab_alloc(&slab, (void **)&sp_) < 0) return -1;

  if (slab_free((void **)&sp_) < 0) return -1;

  return 0;
}

struct test_slab_blocks {
  int test;
  long long long_test;
};

#define TEST_SLAB_COUNT 10

int test_slab_alloc_free() {
  int i = 0;

  // array of pointers to the struct allocated?
  struct test_slab_blocks *p[TEST_SLAB_COUNT + 1];

  // aka "blob"
  uint8_t slab_space[(sizeof(struct test_slab_blocks) + 8) * TEST_SLAB_COUNT];

  // the handle to the stack allocator
  slab_t slab;

  slab_init(&slab, sizeof(struct test_slab_blocks), slab_space,
            sizeof(slab_space));

  while (i < TEST_SLAB_COUNT) {
    if (slab_alloc(&slab, (void **)&p[i])) return -1;
    i++;
  }

  if (slab_alloc(&slab, (void **)&p[i]) == 0) return -1;

  while (--i < 0) {
    if (slab_free(p[i])) return -1;
  }

  if (slab_free((void **)&p[0]) == 0) return -1;

  return 0;
}

TEST_DEF(slab) {
  TEST_MOD_INIT();
  TEST_MOD_EXEC(slab_most_basic_test());
  TEST_MOD_EXEC(test_slab_alloc_free());
  TEST_MOD_REPORT();
}
