#include <cassert> // assert
#include <cstddef> // NULL

#include "GList.h"

FAKEGLIB_API GList *g_list_append(GList *list, gpointer data)
{
	GList *last = g_list_last(list);

	GList *node = new GList{};
	node->data = data;
	node->next = NULL;
	node->prev = last;

	if(last != NULL) {
		last->next = node;
		return list;
	} else {
		return node;
	}
}

FAKEGLIB_API GList *g_list_prepend(GList *list, gpointer data)
{
	assert(list == NULL || list->prev == NULL);

	GList *node = new GList{};
	node->data = data;
	node->next = list;
	node->prev = NULL;

	if(list != NULL) {
		list->prev = node;
	}

	return node;
}

FAKEGLIB_API GList *g_list_insert(GList *list, gpointer data, gint position)
{
	assert(list == NULL || list->prev == NULL);

	guint unsignedPosition = (guint) position;
	guint i = 0;
	GList *after;
	for(after = list; after != NULL && i < unsignedPosition; after = after->next, i++) {
		// nothing to do
	}

	return g_list_insert_before(list, after, data);
}

FAKEGLIB_API GList *g_list_insert_before(GList *list, GList *after, gpointer data)
{
	assert(list == NULL || list->prev == NULL);

	GList *node = new GList{};
	node->data = data;
	node->next = after;

	GList *before;
	if(after == NULL) {
		before = g_list_last(list);
	} else {
		before = after->prev;
		after->prev = node;
	}

	node->prev = before;

	if(before != NULL) {
		before->next = node;
		return list;
	} else {
		return node;
	}
}

FAKEGLIB_API void g_list_free(GList *list)
{
	if(list == NULL) {
		return;
	}

	GList *last;
	for(last = list; last->next != NULL; last = last->next) {
		delete last->prev;
	}
	delete last;
}

FAKEGLIB_API GList *g_list_first(GList *list)
{
	if(list == NULL) {
		return NULL;
	}

	GList *first;
	for(first = list; first->prev != NULL; first = first->prev) {
		// nothing to do
	}
	return first;
}

FAKEGLIB_API GList *g_list_last(GList *list)
{
	if(list == NULL) {
		return NULL;
	}

	GList *last;
	for(last = list; last->next != NULL; last = last->next) {
		// nothing to do
	}
	return last;
}