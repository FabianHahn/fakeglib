#include <cassert> // assert
#include <climits> // UINT_MAX
#include <cstddef> // NULL

#include "GList.h"

static GList *findNodeByIndex(GList *list, guint index);
static GList *splitList(GList *list, guint firstHalfSize);
static GList *sortSizedList(GList *list, GCompareFunc compareFunc, guint length);

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

	GList *after = findNodeByIndex(list, position >= 0 ? position : UINT_MAX);
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

FAKEGLIB_API void g_list_free_1(GList *list)
{
	delete list;
}

FAKEGLIB_API guint g_list_length(GList *list)
{
	assert(list == NULL || list->prev == NULL);

	guint size = 0;
	for(GList *iter = list; iter != NULL; iter = iter->next) {
		size++;
	}
	return size;
}

FAKEGLIB_API GList *g_list_copy(GList *list)
{
	assert(list == NULL || list->prev == NULL);

	GList *newList = NULL;
	GList *newIter = NULL;
	for(GList *iter = list; iter != NULL; iter = iter->next) {
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

	return newList;
}

FAKEGLIB_API GList *g_list_copy_deep(GList *list, GCopyFunc func, gpointer userData)
{
	assert(list == NULL || list->prev == NULL);

	GList *newList = NULL;
	GList *newIter = NULL;
	for(GList *iter = list; iter != NULL; iter = iter->next) {
		GList *node = new GList{};
		node->data = func(iter->data, userData);
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

	return newList;
}

FAKEGLIB_API GList *g_list_reverse(GList *list)
{
	assert(list == NULL || list->prev == NULL);

	if(list == NULL) {
		return NULL;
	}

	GList *iter = list;
	while(true) {
		GList *next = iter->next;
		iter->next = iter->prev;
		iter->prev = next;

		if(next == NULL) {
			return iter;
		} else {
			iter = next;
		}
	}
}

FAKEGLIB_API GList *g_list_sort(GList *list, GCompareFunc compareFunc)
{
	assert(list == NULL || list->prev == NULL);

	guint length = g_list_length(list);
	return sortSizedList(list, compareFunc, length);
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

static GList *findNodeByIndex(GList *list, guint index)
{
	guint i = 0;
	GList *after;
	for(after = list; after != NULL && i < index; after = after->next, i++) {
		// nothing to do
	}

	return after;
}

static GList *splitList(GList *list, guint firstHalfSize)
{
	GList *after = findNodeByIndex(list, firstHalfSize);

	if(after != NULL) {
		assert(after->prev != NULL);
		after->prev->next = NULL;
		after->prev = NULL;
	}

	return after;
}

static GList *sortSizedList(GList *list, GCompareFunc compareFunc, guint length)
{
	if(length < 2) {
		return list;
	}

	guint firstHalfSize = length / 2;
	guint secondHalfSize = length - firstHalfSize;

	GList *secondHalf = splitList(list, length / 2);
	assert(g_list_length(list) == firstHalfSize);
	assert(g_list_length(secondHalf) == secondHalfSize);

	GList *sortedFirstHalf = sortSizedList(list, compareFunc, firstHalfSize);
	GList *sortedSecondHalf = sortSizedList(secondHalf, compareFunc, secondHalfSize);

	GList *sortedList = NULL;
	GList *sortedListIter = NULL;
	GList *firstHalfIter = sortedFirstHalf;
	GList *secondHalfIter = sortedSecondHalf;
	while(firstHalfIter != NULL || secondHalfIter != NULL) {
		GList *nextNode;
		if(firstHalfIter == NULL) {
			nextNode = secondHalfIter;
			secondHalfIter = secondHalfIter->next;
		} else if(secondHalfIter == NULL) {
			nextNode = firstHalfIter;
			firstHalfIter = firstHalfIter->next;
		} else {
			if(compareFunc(firstHalfIter->data, secondHalfIter->data) > 0) {
				nextNode = secondHalfIter;
				secondHalfIter = secondHalfIter->next;
			} else {
				nextNode = firstHalfIter;
				firstHalfIter = firstHalfIter->next;
			}
		}

		nextNode->prev = sortedListIter;

		if(sortedListIter != NULL) {
			sortedListIter->next = nextNode;
		} else {
			sortedList = nextNode;
		}

		sortedListIter = nextNode;
	}
	sortedListIter->next = NULL;

	return sortedList;
}