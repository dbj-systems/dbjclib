#pragma once

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

#include "external/getopt.h"

#include <crtdbg.h>

#define DBJ_ASSERT _ASSERTE

static inline bool str_begins_with(const char str[static 1], const char substr[static 1])
{
	return !strncmp(str, substr, strlen(substr));
}

static inline bool str_ends_with(const char str[static 1], const char suffix[static 1])
{
	const int str_len = strlen(str);
	const int suffix_len = strlen(suffix);

	return
		(str_len >= suffix_len) &&
		(0 == strcmp(str + (str_len - suffix_len), suffix));
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



