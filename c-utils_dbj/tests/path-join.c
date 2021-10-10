
//
// path-join.c
//
// Copyright (c) 2013 Stephen Mathieson
// MIT licensed
//

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../dbj_common.h"
#include "path-join.h"

#define str_starts_with str_begins_with

static bool str_begins_with (const char str[static 1], const char substr[static 1]) 
{
return ! strncmp(str, substr, strlen(substr));
}

static bool str_ends_with(const char str[static 1], const char suffix[static 1])
{
  const int str_len = strlen(str);
  const int suffix_len = strlen(suffix);

  return 
    (str_len >= suffix_len) &&
    (0 == strcmp(str + (str_len-suffix_len), suffix));
}

// int main(void) {
//     SX("%d", str_begins_with("1234567890", "123") ) ;
//     SX("%d", str_begins_with("1234567890", "XXX") ) ;
//     SX("%d", str_ends_with("1234567890", "123") ) ;
//     SX("%d", str_ends_with("1234567890", "890") ) ;
// }

#ifdef _WIN32
#define PATH_JOIN_SEPERATOR   "\\"
#else
#define PATH_JOIN_SEPERATOR   "/"
#endif

/*
 * Join `dir` with `file`
 */

char *
path_join(const char dir[static 1], const char file[static 1]) 
{
  const int size = strlen(dir) + strlen(file) + 2;
  char *buf = malloc(size * sizeof(char));
  if (NULL == buf) return NULL;

  strcpy(buf, dir);

  // add the sep if necessary
  if (!str_ends_with(dir, PATH_JOIN_SEPERATOR)) {
    strcat(buf, PATH_JOIN_SEPERATOR);
  }

  // remove the sep if necessary
  if (str_starts_with(file, PATH_JOIN_SEPERATOR)) {
    char *filecopy = strdup(file);
    if (NULL == filecopy) {
      free(buf);
      return NULL;
    }
    strcat(buf, ++filecopy);
    free(--filecopy);
  } else {
    strcat(buf, file);
  }

  return buf;
}
