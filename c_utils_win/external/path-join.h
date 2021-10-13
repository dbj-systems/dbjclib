#pragma once

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
(c) 2021 by dbj@dbj.org

do not have to put it on separate previous line from the offending call
sometimes one must put it immediately before . Example


if ( SUPR4996 sscanf(line, "%[^=] = \"%[^\"]\"", key, value) == 2
        || SUPR4996 sscanf(line, "%[^=] = '%[^\']'", key, value) == 2)
{
}

only that will silence the cl.exe compiler

*/
#ifdef _MSC_VER
#define SUPR4996 __pragma(warning(suppress : 4996))
#else
#define SUPR4996
#endif

#ifdef _WIN32
#define PATH_JOIN_SEPERATOR '\\'
#else
#define PATH_JOIN_SEPERATOR '/'
#endif

/*
 (c) 2021 by dbj@dbj.org , https://dbj.org/license_dbj

 This is standard C, aka C11.

 precondition:

 dir and file must be whitespace trimmed on both sides, with not zero chars
 anywhere in the middle.
 */

static inline int path_join(const unsigned outsize, char out[static outsize],
                            const char dir[static 1],
                            const char file[static 1]) {
  const int dir_len = strlen(dir);
  const int size = dir_len + strlen(file) + 2;

  if (size >= outsize) return EINVAL;

#ifdef _MSC_VER
#pragma warning(suppress : 4996)
#endif
  memset(out, 0, outsize);

#ifdef _MSC_VER
#pragma warning(suppress : 4996)
#endif
  strcpy(out, dir);

  char* p = strrchr(out, 0);

  // add the sep if not alread the last char
  if (*(p - 1) != PATH_JOIN_SEPERATOR) {
    *(p) = PATH_JOIN_SEPERATOR;
  }

  // skip over the sep. if its a file first char
  if (file[0] == PATH_JOIN_SEPERATOR) file++;

  p = strrchr(out, 0);  // the one beyond the last

#ifdef _MSC_VER
#pragma warning(suppress : 4996)
#endif
  strcpy(p, file);

  return 0;
}

#undef PATH_JOIN_SEPERATOR
