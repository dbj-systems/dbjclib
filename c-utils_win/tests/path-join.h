#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#define PATH_JOIN_SEPERATOR   '\\'
#else
#define PATH_JOIN_SEPERATOR   '/'
#endif

/*
 (c) 2021 by dbj@dbj.org
 */

static inline int path_join(
	const unsigned outsize, char out[static outsize],
	const char dir[static 1],
	const char file[static 1])
{
	const int dir_len = strlen(dir);
	const int size = dir_len + strlen(file) + 2;

	if (size >= outsize) return EINVAL;

#pragma warning(suppress:4996)
	memset(out, 0, outsize);

#pragma warning(suppress:4996)
	strcpy(out, dir);

	char* p = strrchr(out, 0);

	// add the sep if not alread the last char
	if (*(p - 1) != PATH_JOIN_SEPERATOR) {
		*(p) = PATH_JOIN_SEPERATOR;
	}

	// dkip over the sep if its a file first char
	if (file[0] == PATH_JOIN_SEPERATOR) file++;

	p = strrchr(out, 0); // the one beyond the last

#pragma warning(suppress:4996)
	strcpy(p, file);

	return 0;
}

#undef PATH_JOIN_SEPERATOR
