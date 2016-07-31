#ifndef FAKEGLIB_G_QUEUE_H
#define FAKEGLIB_G_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"
#include "types.h"

#include "GList.h"

struct GQueueStruct {
	GList *head;
	GList *tail;
	guint length;
};
typedef struct GQueueStruct GQueue;

FAKEGLIB_API GQueue *g_queue_new(void);
FAKEGLIB_API void g_queue_free(GQueue *queue);
FAKEGLIB_API void g_queue_free_full(GQueue *queue, GDestroyNotify free_func);
FAKEGLIB_API void g_queue_init(GQueue *queue);
FAKEGLIB_API void g_queue_clear(GQueue *queue);
FAKEGLIB_API gboolean g_queue_is_empty(GQueue *queue);
FAKEGLIB_API guint g_queue_get_length(GQueue *queue);
FAKEGLIB_API void g_queue_reverse(GQueue *queue);
FAKEGLIB_API GQueue *g_queue_copy(GQueue *queue);
FAKEGLIB_API void g_queue_foreach(GQueue *queue, GFunc func, gpointer user_data);
FAKEGLIB_API GList *g_queue_find(GQueue *queue,	gconstpointer data);
FAKEGLIB_API GList *g_queue_find_custom(GQueue *queue, gconstpointer data, GCompareFunc func);
FAKEGLIB_API void g_queue_sort(GQueue *queue, GCompareDataFunc compare_func, gpointer user_data);
FAKEGLIB_API void g_queue_push_head(GQueue *queue, gpointer data);
FAKEGLIB_API void g_queue_push_tail(GQueue *queue, gpointer data);
FAKEGLIB_API void g_queue_push_nth(GQueue *queue, gpointer data, gint n);
FAKEGLIB_API gpointer g_queue_pop_head(GQueue *queue);
FAKEGLIB_API gpointer g_queue_pop_tail(GQueue *queue);
FAKEGLIB_API gpointer g_queue_pop_nth(GQueue *queue, guint n);
#define G_QUEUE_INIT { NULL, NULL, 0 }

#ifdef __cplusplus
}
#endif

#endif