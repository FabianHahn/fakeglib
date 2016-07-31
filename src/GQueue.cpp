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

FAKEGLIB_API void g_queue_clear(GQueue *queue)
{
	g_list_free(queue->head);
	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;
}

FAKEGLIB_API gboolean g_queue_is_empty(GQueue *queue)
{
	return queue->length == NULL;
}

FAKEGLIB_API guint g_queue_get_length(GQueue *queue)
{
	return queue->length;
}

FAKEGLIB_API void g_queue_reverse(GQueue *queue)
{
	GList *reversedList = g_list_reverse(queue->head);
	queue->tail = queue->head;
	queue->head = reversedList;
}

FAKEGLIB_API GQueue *g_queue_copy(GQueue *queue)
{
	GList *newList = NULL;
	GList *newIter = NULL;
	for(GList *iter = queue->head; iter != NULL; iter = iter->next) {
		GList *node = new GList{};
		node->data = iter->data;
		node->next = NULL;
		node->prev = newIter;

		if(newList == NULL) {
			newList = node;
		}
		if(newIter != NULL) {
			newIter->next = node;
		}

		newIter = node;
	}

	GQueue *copiedQueue = new GQueue{};
	copiedQueue->head = newList;
	copiedQueue->tail = newIter;
	copiedQueue->length = queue->length;
	return copiedQueue;
}

FAKEGLIB_API void g_queue_foreach(GQueue *queue, GFunc func, gpointer userData)
{
	g_list_foreach(queue->head, func, userData);
}
