#include <cassert> // assert
#include <climits> // LONG_MAX
#include <cstddef> // NULL
#include <cstdlib> // malloc, free
#include <cstring> // strdup, strlen, memcpy

#include "GString.h"

FAKEGLIB_API GString *g_string_new(const gchar *init)
{
	GString *string = new GString{};
	string->str = strdup(init == NULL ? "" : init);
	string->len = (gsize) strlen(string->str);
	string->allocated_len = string->len;
	return string;
}

FAKEGLIB_API GString *g_string_new_len(const gchar *init, gssize len)
{
	assert(len >= 0);
	assert(len <= LONG_MAX);

	GString *string = new GString{};
	string->str = (gchar *) malloc((len + 1) * sizeof(gchar));
	memcpy(string->str, init, len);
	string->str[len] = '\0';

	string->len = (gsize) len;
	string->allocated_len = string->len;
	return string;
}

FAKEGLIB_API gchar *g_string_free(GString *string, gboolean freeSegment)
{
	gchar *str;
	if(freeSegment) {
		str = NULL;
		free(string->str);
	} else {
		str = string->str;
	}
	delete string;

	return str;
}
