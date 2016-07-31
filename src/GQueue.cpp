#include <cassert> // assert
#include <climits> // UINT_MAX

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

FAKEGLIB_API GList *g_queue_find(GQueue *queue, gconstpointer data)
{
	return g_list_find(queue->head, data);
}

FAKEGLIB_API GList *g_queue_find_custom(GQueue *queue, gconstpointer data, GCompareFunc func)
{
	return g_list_find_custom(queue->head, data, func);
}

FAKEGLIB_API void g_queue_sort(GQueue *queue, GCompareDataFunc compareFunc, gpointer userData)
{
	queue->head = g_list_sort_with_data(queue->head, compareFunc, userData);
	queue->tail = g_list_last(queue->head);
}

FAKEGLIB_API void g_queue_push_head(GQueue *queue, gpointer data)
{
	queue->head = g_list_prepend(queue->head, data);

	if(queue->tail == NULL) {
		queue->tail = queue->head;
	}

	queue->length++;
}

FAKEGLIB_API void g_queue_push_tail(GQueue *queue, gpointer data)
{
	if(queue->tail == NULL) {
		queue->head = g_list_prepend(queue->head, data);
		queue->tail = queue->head;
	} else {
		g_list_append(queue->tail, data);
		queue->tail = queue->tail->next;
	}

	queue->length++;
}

FAKEGLIB_API void g_queue_push_nth(GQueue *queue, gpointer data, gint position)
{
	guint unsignedPosition = position >= 0 ? (guint) position : UINT_MAX;

	GList *after = NULL;
	if(unsignedPosition < queue->length) {
		after = g_list_nth(queue->head, unsignedPosition);
	}
	queue->head = g_list_insert_before(queue->head, after, data);

	if(queue->tail == NULL) {
		queue->tail = queue->head;
	} else if(after == NULL) {
		queue->tail = queue->tail->next;
	}

	queue->length++;
}

FAKEGLIB_API gpointer g_queue_pop_head(GQueue *queue)
{
	if(queue->length == 0) {
		return NULL;
	}

	assert(queue->head != NULL);

	gpointer data = queue->head->data;

	queue->head = g_list_delete_link(queue->head, queue->head);

	if(queue->head == NULL) {
		queue->tail = NULL;
	}

	queue->length--;

	return data;
}

FAKEGLIB_API gpointer g_queue_pop_tail(GQueue *queue)
{
	if(queue->length == 0) {
		return NULL;
	}

	GList *tail = queue->tail;
	assert(tail != NULL);

	gpointer data = tail->data;
	
	queue->tail = tail->prev;
	queue->head = g_list_delete_link(queue->head, tail);
	queue->length--;

	return data;
}

FAKEGLIB_API gpointer g_queue_pop_nth(GQueue *queue, guint n)
{
	if(queue->length == 0 || n >= queue->length) {
		return NULL;
	}

	GList *nth = g_list_nth(queue->head, n);
	assert(nth != NULL);
	gpointer data = nth->data;

	if(nth == queue->tail) {
		queue->tail = queue->tail->prev;
	}

	queue->head = g_list_delete_link(queue->head, nth);
	queue->length--;

	return data;
}

FAKEGLIB_API gpointer g_queue_peek_head(GQueue *queue)
{
	if(queue->length == 0) {
		return NULL;
	}

	assert(queue->head != NULL);
	return queue->head->data;
}

FAKEGLIB_API gpointer g_queue_peek_tail(GQueue *queue)
{
	if(queue->length == 0) {
		return NULL;
	}

	assert(queue->tail != NULL);
	return queue->tail->data;
}

FAKEGLIB_API gpointer g_queue_peek_nth(GQueue *queue, guint n)
{
	if(queue->length == 0 || n >= queue->length) {
		return NULL;
	}

	GList *nth = g_list_nth(queue->head, n);
	assert(nth != NULL);
	return nth->data;
}

FAKEGLIB_API gint g_queue_index(GQueue *queue, gconstpointer data)
{
	return g_list_index(queue->head, data);
}

FAKEGLIB_API gboolean g_queue_remove(GQueue *queue, gconstpointer data)
{
	for(GList *iter = queue->head; iter != NULL; iter = iter->next) {
		if(iter->data == data) {
			if(iter == queue->tail) {
				queue->tail = queue->tail->prev;
			}

			queue->head = g_list_delete_link(queue->head, iter);
			queue->length--;
			return true;
		}
	}

	return false;
}

FAKEGLIB_API guint g_queue_remove_all(GQueue *queue, gconstpointer data)
{
	guint removed = 0;

	for(GList *iter = queue->head; iter != NULL;) {
		if(iter->data == data) {
			GList *next = iter->next;

			if(iter == queue->tail) {
				queue->tail = queue->tail->prev;
			}

			queue->head = g_list_delete_link(queue->head, iter);
			queue->length--;
			removed++;

			iter = next;
		} else {
			iter = iter->next;
		}
	}

	return removed;
}

FAKEGLIB_API void g_queue_insert_before(GQueue *queue, GList *sibling, gpointer data)
{
	if(sibling == NULL) {
		g_queue_push_tail(queue, data);
	} else {
		queue->head = g_list_insert_before(queue->head, sibling, data);
		queue->length++;
	}
}

FAKEGLIB_API void g_queue_insert_after(GQueue *queue, GList *sibling, gpointer data)
{
	if(sibling == NULL) {
		g_queue_push_head(queue, data);
	} else {
		g_queue_insert_before(queue, sibling->next, data);
	}
}

FAKEGLIB_API void g_queue_insert_sorted(GQueue *queue, gpointer data, GCompareDataFunc func, gpointer userData)
{
	assert(queue->tail == NULL || queue->tail->next == NULL);

	queue->head = g_list_insert_sorted_with_data(queue->head, data, func, userData);

	if(queue->tail == NULL) {
		queue->tail = queue->head;
	} else if(queue->tail->next != NULL) {
		queue->tail = queue->tail->next;
	}

	queue->length++;
}
