#include <cassert> // assert
#include <climits> // LONG_MAX
#include <cstdarg> // va_list
#include <cstddef> // NULL
#include <cstdio> // vsnprintf
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

FAKEGLIB_API GString *g_string_sized_new(gsize len)
{
	assert(len >= 0);
	assert(len <= LONG_MAX);

	GString *string = new GString{};
	string->str = (gchar *) malloc((len + 1) * sizeof(gchar));
	string->str[0] = '\0';
	string->len = 0;
	string->allocated_len = len;
	return string;
}

FAKEGLIB_API void g_string_vprintf(GString *string, const gchar *format, va_list args)
{
	va_list copiedArgs;
	va_copy(copiedArgs, args);
	int ret = vsnprintf(string->str, string->allocated_len + 1, format, copiedArgs);
	va_end(copiedArgs);
	if(ret < (int) string->allocated_len + 1) {
		assert(ret >= 0);
		string->len = ret;
		return;
	}

	free(string->str);
	string->str = (gchar *) malloc((ret + 1) * sizeof(gchar));
	string->len = ret;
	string->allocated_len = ret;

	ret = vsnprintf(string->str, string->allocated_len + 1, format, args);
	assert(ret == string->allocated_len);
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