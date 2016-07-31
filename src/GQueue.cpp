#include "GQueue.h"

FAKEGLIB_API GQueue *g_queue_new(void)
{
	GQueue *queue = new GQueue{};
	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;
	return queue;
}

FAKEGLIB_API void g_queue_free(GQueue *queue)
{
	g_list_free(queue->head);
	delete queue;
}

FAKEGLIB_API void g_queue_free_full(GQueue *queue, GDestroyNotify freeFunc)
{
	g_list_free_full(queue->head, freeFunc);
	delete queue;
}

FAKEGLIB_API void g_queue_init(GQueue *queue)
{
	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;
}