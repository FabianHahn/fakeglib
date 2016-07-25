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