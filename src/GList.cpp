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

FAKEGLIB_API GList *g_list_insert_sorted(GList *list, gpointer data, GCompareFunc func)
{
	assert(list == NULL || list->prev == NULL);

	if(list == NULL || func(list->data, data) > 0) {
		return g_list_prepend(list, data);
	}

	GList *after;
	for(after = list; after->next != NULL; after = after->next) {
		if(func(after->next->data, data) > 0) {
			break;
		}
	}

	if(after->next == NULL) {
		g_list_append(after, data);
		return list;
	} else {
		return g_list_insert_before(list, after->next, data);
	}
}

FAKEGLIB_API GList *g_list_remove(GList *list, gconstpointer data)
{
	assert(list == NULL || list->prev == NULL);

	for(GList *iter = list; iter != NULL; iter = iter->next) {
		if(iter->data == data) {
			return g_list_delete_link(list, iter);
		}
	}

	return list;
}

FAKEGLIB_API GList *g_list_remove_link(GList *list, GList *link)
{
	assert(list != NULL);
	assert(link != NULL);

	GList *newList = list;
	if(list == link) {
		newList = link->next;
	}

	if(link->next != NULL) {
		link->next->prev = link->prev;
	}
	if(link->prev != NULL) {
		link->prev->next = link->next;
	}

	link->next = NULL;
	link->prev = NULL;

	return newList;
}

FAKEGLIB_API GList *g_list_delete_link(GList *list, GList *link)
{
	assert(list != NULL);
	assert(link != NULL);

	GList *newList = list;
	if(list == link) {
		newList = link->next;
	}

	if(link->next != NULL) {
		link->next->prev = link->prev;
	}
	if(link->prev != NULL) {
		link->prev->next = link->next;
	}

	delete link;

	return newList;
}

FAKEGLIB_API GList *g_list_remove_all(GList *list, gconstpointer data)
{
	assert(list == NULL || list->prev == NULL);

	GList *newList = list;
	for(GList *iter = list; iter != NULL;) {
		if(iter->data == data) {
			GList *next = iter->next;
			newList = g_list_delete_link(newList, iter);
			iter = next;
		} else {
			iter = iter->next;
		}
	}

	return newList;
}

FAKEGLIB_API void g_list_free(GList *list)
{
	assert(list == NULL || list->prev == NULL);

	if(list == NULL) {
		return;
	}

	for(GList *iter = list; iter != NULL;) {
		GList *next = iter->next;
		delete iter;
		iter = next;
	}
}

FAKEGLIB_API void g_list_free_full(GList *list, GDestroyNotify freeFunc)
{
	assert(list == NULL || list->prev == NULL);

	if(list == NULL) {
		return;
	}

	for(GList *iter = list; iter != NULL;) {
		GList *next = iter->next;
		freeFunc(iter->data);
		delete iter;
		iter = next;
	}
}

FAKEGLIB_API GList *g_list_alloc()
{
	GList *node = new GList{};
	node->data = NULL;
	node->next = NULL;
	node->prev = NULL;
	return node;
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