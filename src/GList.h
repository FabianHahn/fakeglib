#ifndef FAKEGLIB_G_LIST_H
#define FAKEGLIB_G_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"
#include "types.h"

struct GListStruct {
	gpointer data;
	GListStruct *next;
	GListStruct *prev;
};
typedef struct GListStruct GList;

FAKEGLIB_API GList *g_list_append(GList *list, gpointer data);
FAKEGLIB_API GList *g_list_prepend(GList *list, gpointer data);
FAKEGLIB_API void g_list_free(GList *list);
FAKEGLIB_API GList *g_list_first(GList *list);
FAKEGLIB_API GList *g_list_last(GList *list);

#ifdef __cplusplus
}
#endif

#endif