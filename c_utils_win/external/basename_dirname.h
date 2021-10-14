#pragma once

/// https://github.com/jart/cosmopolitan/tree/master/libc/fmt
/*
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/

#include <stdbool.h>
#include <string.h>

#define ISSLASH(c) (c == '/' || c == '\\')
#define ISDELIM(c) (ISSLASH(c) || c == '.')

/**
 * Returns directory portion of path.
 * @param s is mutated
 *
 * dbj added prefix dbj_ to avoid possible complaints by various compilers
 */
static inline char* dbj_dirname(char* s) {
  size_t i = 0, n = 0;
  if (!(n = strlen(s))) return s;
  while (n && ISDELIM(s[n - 1])) --n;
  if (n) {
    while (n && !ISSLASH(s[n - 1])) --n;
    if (n) {
      while (n && ISDELIM(s[n - 1])) --n;
      if (!n) ++n;
    } else {
      s[n++] = '.';
    }
  } else {
    ++n;
  }
  s[n] = '\0';
  return s;
}

/**
 * Returns pointer to last filename component in path.
 *
 * Both / and \ are are considered valid component separators on all
 * platforms. Trailing slashes are ignored. We don't grant special
 * consideration to things like foo/., c:/, \\?\Volume, etc.
 *
 * @param path is UTF-8 path
 * @param size is byte length of path
 * @return pointer inside path or path itself
 */
static inline char* dbj_basename_n(const char* path, size_t size) {
  size_t i, l;
  if (size) {
    if (ISSLASH(path[size - 1])) {
      l = size - 1;
      while (l && ISSLASH(path[l - 1])) --l;
      if (!l) return (/*unconst*/ char*)&path[size - 1];
      size = l;
    }
    for (i = size; i > 0; --i) {
      if (ISSLASH(path[i - 1])) {
        return (/*unconst*/ char*)&path[i];
      }
    }
  }
  return (/*unconst*/ char*)path;
}

static inline char* dbj_basename(const char* path) {
  return dbj_basename_n(path, strlen(path));
}

#undef ISSLASH
#undef ISDELIM
