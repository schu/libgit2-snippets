
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "common.h"

void NORETURN die(const char *err, ...)
{
	va_list params;

	va_start(params, err);
	vfprintf(stderr, err, params);
	fprintf(stderr, "\n");
	va_end(params);

	exit(128);
}

void NORETURN die_giterror()
{
	int ret;
	const git_error *error;

	(void)(ret);
	(void)(error);

	error = giterr_last();
	ret = error->klass;
	fprintf(stderr, "%s\n", error->message);
	giterr_clear();

	exit(ret);
}
