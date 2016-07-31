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

#define G_QUEUE_INIT { NULL, NULL, 0 }

#ifdef __cplusplus
}
#endif

#endif