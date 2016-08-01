#ifndef FAKEGLIB_G_STRING_H
#define FAKEGLIB_G_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"
#include "types.h"

struct GStringStruct {
	gchar *str;
	gsize len;
	gsize allocated_len;
};
typedef struct GStringStruct GString;

FAKEGLIB_API GString *g_string_new(const gchar *init);
FAKEGLIB_API GString *g_string_new_len(const gchar *init, gssize len);
FAKEGLIB_API gchar *g_string_free(GString *string, gboolean free_segment);

#ifdef __cplusplus
}
#endif

#endif