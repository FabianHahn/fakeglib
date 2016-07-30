#ifndef FAKEGLIB_G_LIST_H
#define FAKEGLIB_G_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"
#include "types.h"

typedef gint (*GCompareFunc)(gconstpointer a, gconstpointer b);

struct GListStruct {
	gpointer data;
	GListStruct *next;
	GListStruct *prev;
};
typedef struct GListStruct GList;

FAKEGLIB_API GList *g_list_append(GList *list, gpointer data);
FAKEGLIB_API GList *g_list_prepend(GList *list, gpointer data);
FAKEGLIB_API GList *g_list_insert(GList *list, gpointer data, gint position);
FAKEGLIB_API GList *g_list_insert_before(GList *list, GList *sibling, gpointer data);
FAKEGLIB_API GList *g_list_insert_sorted(GList *list, gpointer data, GCompareFunc func);
FAKEGLIB_API GList *g_list_remove(GList *list, gconstpointer data);
FAKEGLIB_API GList *g_list_remove_link(GList *list, GList *llink);
FAKEGLIB_API GList *g_list_delete_link(GList *list, GList *link_);
FAKEGLIB_API GList *g_list_remove_all(GList *list, gconstpointer data);
FAKEGLIB_API void g_list_free(GList *list);
FAKEGLIB_API void g_list_free_full(GList *list, GDestroyNotify free_func);
FAKEGLIB_API GList *g_list_alloc(void);
FAKEGLIB_API void g_list_free_1(GList *list);
FAKEGLIB_API guint g_list_length(GList *list);
FAKEGLIB_API GList *g_list_copy(GList *list);
FAKEGLIB_API GList *g_list_first(GList *list);
FAKEGLIB_API GList *g_list_last(GList *list);

#ifdef __cplusplus
}
#endif

#endif