#pragma once

#define __STDC_WANT_LIB_EXT1__ 1

#include "include/utils/arg_parser.h"
#include "include/utils/utils.h"
#include "include/utils/list.h"
#include "include/utils/logger.h"
#include "include/utils/queue.h"
#include "include/utils/stack.h"

// #include "include/utils/filo.h"
#include "include/utils/memory.h"
#include "include/utils/circbuf.h"
// #include "include/utils/bus_server.h"
// #include "include/utils/sockutils.h"
// #include "include/utils/fdutils.h"
// #include "include/utils/workqueue.h"

#include "include/utils/hashmap.h"
#include "include/utils/strutils.h"
#include "include/utils/strlib.h"
// #include "include/utils/procutils.h"
#include "include/utils/slab.h"
// #include "include/utils/serial.h"


// #include <utils/utils.h>
// #include <utils/bus_server.h>
// #include <utils/sockutils.h>
// #include <utils/fdutils.h>

#ifdef DBJ_CLI_HAS_OPTS
#include "external/getopt.h"
#endif // DBJ_CLI_HAS_OPTS

#include <io.h>
#include <crtdbg.h>

#define DBJ_ASSERT _ASSERTE

static inline bool str_begins_with(const char str[static 1], const char substr[static 1])
{
	const int str_len = strlen(str);
	const int prefix_len = strlen(substr);

	if (prefix_len >= str_len) return false;

	return !strncmp(str, substr, prefix_len);
}

static inline bool str_ends_with(const char str[static 1], const char suffix[static 1])
{
	const int str_len = strlen(str);
	const int suffix_len = strlen(suffix);

	if (suffix_len >= str_len) return false;

	return	(0 == strcmp(str + (str_len - suffix_len), suffix));
}

#define dbj_strtok_r_implementation
#ifdef dbj_strtok_r_implementation

/* Parse S into tokens separated by characters in DELIM.
   If S is NULL, the saved pointer in SAVE_PTR is used as
   the next starting point.  For example:
	char s[] = "-abc-=-def";
	char *sp;
	x = strtok_r(s, "-", &sp);        // x = "abc", sp = "=-def"
	x = strtok_r(NULL, "-=", &sp);        // x = "def", sp = NULL
	x = strtok_r(NULL, "=", &sp);        // x = NULL
		// s = "abc\0-def\0"
*/
static inline char* strtok_r(char* s, const char* delim, char** save_ptr)
{
	char* end;
	if (s == NULL)
		s = *save_ptr;
	if (*s == '\0')
	{
		*save_ptr = s;
		return NULL;
	}
	/* Scan leading delimiters.  */
	s += strspn(s, delim);
	if (*s == '\0')
	{
		*save_ptr = s;
		return NULL;
	}
	/* Find the end of the token.  */
	end = s + strcspn(s, delim);
	if (*end == '\0')
	{
		*save_ptr = end;
		return s;
	}
	/* Terminate the token and make *SAVE_PTR point past it.  */
	*end = '\0';
	*save_ptr = end + 1;
	return s;
}

#endif // dbj_strtok_r_implementation

static inline unsigned dbj_app_folder_subfolder(
	const unsigned out_size, char out[static out_size], char app_full_path[static 1],
	const unsigned subfolder_size, const char subfolder[static subfolder_size]
)
{
	// to the basename
	char* p = strrchr(app_full_path, '\\');

	unsigned dist_ = (p + 1) - &app_full_path[0];
	if (!(dist_ < out_size)) return EINVAL;

#pragma warning(suppress:4996)
	int rez_ = memcpy_s(out, out_size, app_full_path, dist_);

	if (rez_ != 0) return EINVAL;

	p = strrchr(out, '\\');
	// the index of one past the last separator
	dist_ = (p + 1) - &out[0];
	if (!(dist_ < out_size)) return EINVAL;

#pragma warning(suppress:4996)
	return memcpy_s(&out[dist_], out_size - dist_, subfolder, subfolder_size);
}



