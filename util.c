#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "util.h"

void die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(1);
}

char *extension(char *path)
{
	char *last_dot = NULL;
	while (*path) {
		if (*path == '.' && *(path+1))
			last_dot = path;
		path++;
	}
	return last_dot ? last_dot+1 : last_dot;
}
