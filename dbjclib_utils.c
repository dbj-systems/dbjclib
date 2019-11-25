// the actual dbj++clib low level stuff

#include "dbjclib.h"
#include <stdarg.h>
/*
last arg *must* be NULL
*/
void free_free_set_them_free(void * vp, ...)
{
	const size_t max_args = 255; size_t arg_count = 0;
	va_list marker;
	va_start(marker, vp); /* Initialize variable arguments. */
	while (vp != NULL)
	{
		free(vp);
		vp = NULL;
		vp = va_arg(marker, void*);
		/* feeble attempt to make it safer  */
		if (++arg_count == max_args) break;
	}
	va_end(marker);   /* Reset variable argument list. */
}
