#pragma once
/*
(c) 2021 by dbj@dbj.org, https://dbj.org/license_dbj

this is standard C, aka C11
*/

#define VT100_RESET             "\033[0m"
#define VT100_RED_BOLD          "\033[91m"
#define VT100_GREEN_BOLD        "\033[92m"
#define VT100_YELLOW_BOLD       "\033[93m"
#define VT100_BLUE_BOLD         "\033[94m"
#define VT100_WHITE_BRIGHT      "\033[97m"
#define VT100_WHITE_BRIGHT_BOLD "\033[97;1m"

#define DBJ_STR_(X) #X
#define DBJ_STR(X)  DBJ_STR_(X)

#define DBJ_PROMPT_(S_) printf(VT100_WHITE_BRIGHT_BOLD "\n%s" VT100_RESET, S_)
#define DBJ_PROMPT(...) DBJ_PROMPT_(DBJ_STR(__VA_ARGS__))

// DBJ Show eXpression
#define SX(F, X) printf("\n%s : " F, (#X), (X))
#define DBJ_SHOW_ENTRY                                                       \
  printf("\n------------------------------------------------------\n%s()\n", \
         __FUNCTION__)

#ifdef _MSC_VER
#include <crtdbg.h>
#define DBJ_ASSERT _ASSERTE
#else
#include <assert.h>
#define DBJ_ASSERT assert
#endif  //  ! _MSC_VER

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
// suppress is a cl extension only
#define SUPR4996
#endif

#include "external/path-join.h"

/*
put this in one place inside a C file
#define DBJ_FILE_READ_ALL_IMPLEMENTATION
*/
#include "external/dbj_file_read_all.h"  // dbj

#ifdef DBJ_CLI_HAS_OPTS
#include "external/getopt.h"
#endif  // DBJ_CLI_HAS_OPTS

/* --------------- other stuff ---------------- */

#define __STDC_WANT_LIB_EXT1__ 1

#include "include/utils/arg_parser.h"
#include "include/utils/list.h"
#include "include/utils/logger.h"
#include "include/utils/queue.h"
#include "include/utils/stack.h"
#include "include/utils/utils.h"

// #include "include/utils/filo.h"
#include "include/utils/circbuf.h"
#include "include/utils/memory.h"
// #include "include/utils/bus_server.h"
// #include "include/utils/sockutils.h"
// #include "include/utils/fdutils.h"
// #include "include/utils/workqueue.h"

#include "include/utils/hashmap.h"
#include "include/utils/strlib.h"
#include "include/utils/strutils.h"
// #include "include/utils/procutils.h"
#include "include/utils/slab.h"
// #include "include/utils/serial.h"

// #include <utils/utils.h>
// #include <utils/bus_server.h>
// #include <utils/sockutils.h>
// #include <utils/fdutils.h>

/* ----------------- dbj functions -------------------------- */

#include <errno.h>
#include <io.h>

static inline bool str_begins_with(const char str[static 1],
                                   const char substr[static 1]) {
  const int str_len = strlen(str);
  const int prefix_len = strlen(substr);

  if (prefix_len >= str_len) return false;

  return !strncmp(str, substr, prefix_len);
}

static inline bool str_ends_with(const char str[static 1],
                                 const char suffix[static 1]) {
  const int str_len = strlen(str);
  const int suffix_len = strlen(suffix);

  if (suffix_len >= str_len) return false;

  return (0 == strcmp(str + (str_len - suffix_len), suffix));
}

#define dbj_strtok_r_implementation
#ifdef dbj_strtok_r_implementation

/* Parse S into tokens separated by characters in DELIM.
   If S is NULL, the saved pointer in SAVE_PTR is used as
   the next starting point.  For example:
        char s[] = "-abc-=-def";
        char *sp;
        x = dbj_strtok_r(s, "-", &sp);        // x = "abc", sp = "=-def"
        x = dbj_strtok_r(NULL, "-=", &sp);        // x = "def", sp = NULL
        x = dbj_strtok_r(NULL, "=", &sp);        // x = NULL
                // s = "abc\0-def\0"
*/
static inline char* dbj_strtok_r(char* s, const char* delim, char** save_ptr) {
  char* end;
  if (s == NULL) s = *save_ptr;
  if (*s == '\0') {
    *save_ptr = s;
    return NULL;
  }
  /* Scan leading delimiters.  */
  s += strspn(s, delim);
  if (*s == '\0') {
    *save_ptr = s;
    return NULL;
  }
  /* Find the end of the token.  */
  end = s + strcspn(s, delim);
  if (*end == '\0') {
    *save_ptr = end;
    return s;
  }
  /* Terminate the token and make *SAVE_PTR point past it.  */
  *end = '\0';
  *save_ptr = end + 1;
  return s;
}

#endif  // dbj_strtok_r_implementation

/*
 append the sub folder to the app folder, returns 0 or EINVAL
*/
static inline unsigned dbj_app_folder_subfolder(
    const unsigned out_size, char out[static out_size],
    char app_full_path[static 1], const unsigned subfolder_size,
    const char subfolder[static subfolder_size]) {
  // to the basename
  char* p = strrchr(app_full_path, '\\');

  unsigned dist_ = (p + 1) - &app_full_path[0];
  if (!(dist_ < out_size)) return EINVAL;

#ifdef _MSC_VER
#pragma warning(suppress : 4996)
#endif
  int rez_ = memcpy_s(out, out_size, app_full_path, dist_);

  if (rez_ != 0) return EINVAL;

  p = strrchr(out, '\\');
  // the index of one past the last separator
  dist_ = (p + 1) - &out[0];
  if (!(dist_ < out_size)) return EINVAL;

#ifdef _MSC_VER
#pragma warning(suppress : 4996)
#endif
  return memcpy_s(&out[dist_], out_size - dist_, subfolder, subfolder_size);
}
