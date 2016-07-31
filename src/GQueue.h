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
#define G_QUEUE_INIT { NULL, NULL, 0 }

#ifdef __cplusplus
}
#endif

#endif