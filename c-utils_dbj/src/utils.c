/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

 // #include <utils/utils.h>
#include "../dbj_common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
// #include <sys/time.h>
// #include <time.h> // https://blog.habets.se/2010/09/gettimeofday-should-never-be-used-to-measure-time.html
// https://opensource.apple.com/source/curl/curl-78/curl/lib/timeval.c.auto.html
#include <windows.h>


int randint(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

uint32_t round_up_pow2(uint32_t v)
{
	/* from the bit-twiddling hacks */
	v -= 1;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v += 1;
	return v;
}

void hexdump(const void* p, size_t len, const char* fmt, ...)
{
	size_t i;
	va_list args;
	char str[16 + 1] = { 0 };
	const uint8_t* data = p;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	printf(" [%zu] =>\n    0000  %02x ", len, data[0]);
	str[0] = isprint(data[0]) ? data[0] : '.';
	for (i = 1; i < len; i++) {
		if ((i & 0x0f) == 0) {
			printf(" |%16s|", str);
			printf("\n    %04zu  ", i);
		}
		else if ((i & 0x07) == 0) {
			printf(" ");
		}
		printf("%02x ", data[i]);
		str[i & 0x0f] = isprint(data[i]) ? data[i] : '.';
	}
	if ((i &= 0x0f) != 0) {
		if (i <= 8) printf(" ");
		while (i < 16) {
			printf("   ");
			str[i++] = ' ';
		}
		printf(" |%16s|", str);
	}
	else {
		printf(" |%16s|", str);
	}

	printf("\n");
}

// changed by DBJ
// see here
// https://blog.habets.se/2010/09/gettimeofday-should-never-be-used-to-measure-time.html
int64_t usec_now()
{
	/*
	** GetTickCount() is available on _all_ Windows versions from W95 up
	** to nowadays. Returns milliseconds elapsed since last system boot,
	** increases monotonically and wraps once 49.7 days have elapsed.
	*/
	DWORD milliseconds = GetTickCount();
	// future -> int64_t sec_ = milliseconds / 1000;
	int64_t usec_ = (milliseconds % 1000) * 1000;
	return usec_;
}

int64_t usec_since(int64_t last)
{
	return usec_now() - last;
}

int64_t millis_now()
{
	return (int64_t)(usec_now() / 1000L);
}

int64_t millis_since(int64_t last)
{
	return millis_now() - last;
}
