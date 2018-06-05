#include <cstring> // strncmp, strlen

#include "fakeglib/GStringUtil.h"

FAKEGLIB_API gboolean g_str_has_prefix(const gchar *string, const gchar *prefix)
{
	size_t stringLength = strlen(string);
	size_t prefixLength = strlen(prefix);

	if(prefixLength > stringLength) {
		return false;
	}

	return strncmp(string, prefix, prefixLength) == 0;
}

FAKEGLIB_API gboolean g_str_has_suffix(const gchar *string, const gchar *suffix)
{
	size_t stringLength = strlen(string);
	size_t suffixLength = strlen(suffix);

	if(suffixLength > stringLength) {
		return false;
	}

	return strncmp(string + (stringLength - suffixLength), suffix, suffixLength) == 0;
}
