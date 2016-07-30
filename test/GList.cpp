#include <gtest/gtest.h>
#include <glib.h>

#include <vector>

static std::vector<gpointer> freeCallbacks;

class GListTest : public ::testing::Test {
public:
	virtual void SetUp()
	{
		list = NULL;
	}

	virtual void TearDown()
	{
		g_list_free(list);
	}

	GList *list;
};

gint test_compare_int(gconstpointer v1, gconstpointer v2)
{
	if(v1 == NULL && v2 == NULL) {
		return 0;
	}
	if(v1 == NULL) {
		return -1;
	}
	if(v2 == NULL) {
		return 1;
	}

	const int& firstInt = *reinterpret_cast<const int *>(v1);
	const int& secondInt = *reinterpret_cast<const int *>(v2);

	if(firstInt > secondInt) {
		return 1;
	}
	if(firstInt == secondInt) {
		return 0;
	}
	return -1;
}

static void test_free_callback(gpointer data)
{
	freeCallbacks.push_back(data);
}

TEST_F(GListTest, append)
{
	int testData1 = 42;
	int testData2 = 1337;

	list = g_list_append(list, &testData1);
	ASSERT_TRUE(list != NULL) << "list should not be NULL after appending an element";
	ASSERT_EQ(&testData1, list->data) << "first list element data should be set";
	ASSERT_TRUE(list->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(list->next == NULL) << "first list element should not have a next element";
	list = g_list_append(list, &testData2);
	ASSERT_TRUE(list != NULL) << "list should not be NULL after appending an element";
	ASSERT_EQ(&testData1, list->data) << "first list element data should be set";
	ASSERT_TRUE(list->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(list->next != NULL) << "first list element should have a next element";
	GList *secondElement = list->next;
	ASSERT_EQ(&testData2, secondElement->data) << "second list element data should be set";
	ASSERT_TRUE(secondElement->prev == list) << "second list element should have the first as previous element";
	ASSERT_TRUE(secondElement->next == NULL) << "second list element should not have a next element";
}

TEST_F(GListTest, prepend)
{
	int testData1 = 42;
	int testData2 = 1337;

	list = g_list_prepend(list, &testData1);
	GList *first = list;
	ASSERT_TRUE(list != NULL) << "list should not be NULL after prepending an element";
	ASSERT_EQ(&testData1, list->data) << "first list element data should be set";
	ASSERT_TRUE(list->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(list->next == NULL) << "first list element should not have a next element";
	list = g_list_prepend(list, &testData2);
	ASSERT_TRUE(list != NULL) << "list should not be NULL after prepending an element";
	ASSERT_EQ(&testData2, list->data) << "second list element data should be set";
	ASSERT_TRUE(list->prev == NULL) << "second list element should not have a previous element";
	ASSERT_EQ(first, list->next) << "second list element should have first as next element";
}

TEST_F(GListTest, insert)
{
	int testData1 = 42;
	int testData2 = 1337;
	int testData3 = 27;

	list = g_list_insert(list, &testData1, -1);
	GList *first = list;
	ASSERT_TRUE(list != NULL) << "list should not be NULL after inserting an element";
	ASSERT_EQ(&testData1, first->data) << "first list element data should be set";
	ASSERT_TRUE(first->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(first->next == NULL) << "first list element should not have a next element";

	list = g_list_insert(list, &testData2, 0);
	GList *second = list;
	ASSERT_TRUE(list != NULL) << "list should not be NULL after inserting an element";
	ASSERT_NE(first, second) << "second list element should not be equal to first list element";
	ASSERT_EQ(&testData2, second->data) << "second list element data should be set";
	ASSERT_TRUE(second->prev == NULL) << "second list element should not have a previous element";
	ASSERT_EQ(first, second->next) << "second list element should have first as next element";
	ASSERT_EQ(second, first->prev) << "first list element should have second as previous element";

	list = g_list_insert(list, &testData3, 1);
	GList *third = list->next;
	ASSERT_EQ(second, list) << "list head element should still be second after inserting third element";
	ASSERT_EQ(&testData3, third->data) << "third list element data should be set";
	ASSERT_EQ(first, third->next) << "third list element should have first as next element";
	ASSERT_EQ(second, third->prev) << "third list element should have second as previous element";
	ASSERT_EQ(third, second->next) << "second list element should have third as next element";
	ASSERT_EQ(third, first->prev) << "first list element should have third as previous element";
}

TEST_F(GListTest, insertBefore)
{
	int testData1 = 42;
	int testData2 = 1337;
	int testData3 = 27;

	list = g_list_insert_before(list, NULL, &testData1);
	GList *first = list;
	ASSERT_TRUE(list != NULL) << "list should not be NULL after inserting an element";
	ASSERT_EQ(&testData1, first->data) << "first list element data should be set";
	ASSERT_TRUE(first->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(first->next == NULL) << "first list element should not have a next element";

	list = g_list_insert_before(list, first, &testData2);
	GList *second = list;
	ASSERT_TRUE(list != NULL) << "list should not be NULL after inserting an element";
	ASSERT_NE(first, second) << "second list element should not be equal to first list element";
	ASSERT_EQ(&testData2, second->data) << "second list element data should be set";
	ASSERT_TRUE(second->prev == NULL) << "second list element should not have a previous element";
	ASSERT_EQ(first, second->next) << "second list element should have first as next element";
	ASSERT_EQ(second, first->prev) << "first list element should have second as previous element";

	list = g_list_insert_before(list, first, &testData3);
	GList *third = list->next;
	ASSERT_EQ(second, list) << "list head element should still be second after inserting third element";
	ASSERT_EQ(&testData3, third->data) << "third list element data should be set";
	ASSERT_EQ(first, third->next) << "third list element should have first as next element";
	ASSERT_EQ(second, third->prev) << "third list element should have second as previous element";
	ASSERT_EQ(third, second->next) << "second list element should have third as next element";
	ASSERT_EQ(third, first->prev) << "first list element should have third as previous element";
}

TEST_F(GListTest, insertSorted)
{
	int testData1 = 2;
	int testData2 = 0;
	int testData3 = 1;
	int testData4 = 3;

	list = g_list_insert_sorted(list, &testData1, test_compare_int);
	GList *first = list;
	ASSERT_TRUE(list != NULL) << "list should not be NULL after inserting an element";
	ASSERT_EQ(&testData1, first->data) << "first list element data should be set";
	ASSERT_TRUE(first->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(first->next == NULL) << "first list element should not have a next element";

	list = g_list_insert_sorted(list, &testData2, test_compare_int);
	GList *second = list;
	ASSERT_TRUE(list != NULL) << "list should not be NULL after inserting an element";
	ASSERT_NE(first, second) << "second list element should not be equal to first list element";
	ASSERT_EQ(&testData2, second->data) << "second list element data should be set";
	ASSERT_TRUE(second->prev == NULL) << "second list element should not have a previous element";
	ASSERT_EQ(first, second->next) << "second list element should have first as next element";
	ASSERT_EQ(second, first->prev) << "first list element should have second as previous element";

	list = g_list_insert_sorted(list, &testData3, test_compare_int);
	GList *third = list->next;
	ASSERT_EQ(second, list) << "list head element should still be second after inserting third element";
	ASSERT_EQ(&testData3, third->data) << "third list element data should be set";
	ASSERT_EQ(first, third->next) << "third list element should have first as next element";
	ASSERT_EQ(second, third->prev) << "third list element should have second as previous element";
	ASSERT_EQ(third, second->next) << "second list element should have third as next element";
	ASSERT_EQ(third, first->prev) << "first list element should have third as previous element";

	list = g_list_insert_sorted(list, &testData4, test_compare_int);
	GList *fourth = first->next;
	ASSERT_EQ(second, list) << "list head element should still be second after inserting fourth element";
	ASSERT_EQ(&testData4, fourth->data) << "fourth list element data should be set";
	ASSERT_TRUE(fourth->next == NULL) << "fourth list element should not have a next element";
	ASSERT_EQ(first, fourth->prev) << "fourth list element should have first as previous element";
	ASSERT_EQ(fourth, first->next) << "first list element should have fourth as next element";
}

TEST_F(GListTest, remove)
{
	int testData1 = 42;
	int testData2 = 1337;

	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	list = g_list_remove(list, &testData2);
	ASSERT_TRUE(list != NULL) << "list should not be NULL after removing second element";
	ASSERT_EQ(&testData1, list->data) << "first list element data should be set";
	ASSERT_TRUE(list->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(list->next == NULL) << "first list element should not have a next element";
	list = g_list_remove(list, &testData1);
	ASSERT_TRUE(list == NULL) << "list should be NULL after removing first element";
}

TEST_F(GListTest, removeLink)
{
	int testData1 = 42;
	int testData2 = 1337;

	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	GList *secondElement = list->next;
	list = g_list_remove_link(list, secondElement);
	ASSERT_TRUE(list != NULL) << "list should not be NULL after removing second element";
	ASSERT_EQ(&testData1, list->data) << "first list element data should be set";
	ASSERT_TRUE(list->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(list->next == NULL) << "first list element should not have a next element";
	ASSERT_EQ(&testData2, secondElement->data) << "second list element data should be set";
	ASSERT_TRUE(secondElement->prev == NULL) << "second list element should not have a previous element";
	ASSERT_TRUE(secondElement->next == NULL) << "second list element should not have a next element";
	GList *firstElement = list;
	list = g_list_remove_link(list, firstElement);
	ASSERT_TRUE(list == NULL) << "list should be NULL after removing first element";
	ASSERT_EQ(&testData1, firstElement->data) << "first list element data should be set";
	ASSERT_TRUE(firstElement->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(firstElement->next == NULL) << "first list element should not have a next element";
}

TEST_F(GListTest, deleteLink)
{
	int testData1 = 42;
	int testData2 = 1337;

	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	GList *secondElement = list->next;
	list = g_list_delete_link(list, secondElement);
	ASSERT_TRUE(list != NULL) << "list should not be NULL after removing second element";
	ASSERT_EQ(&testData1, list->data) << "first list element data should be set";
	ASSERT_TRUE(list->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(list->next == NULL) << "first list element should not have a next element";
	GList *firstElement = list;
	list = g_list_remove_link(list, firstElement);
	ASSERT_TRUE(list == NULL) << "list should be NULL after removing first element";
}

TEST_F(GListTest, removeAll)
{
	int testData1 = 42;
	int testData2 = 1337;

	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);

	list = g_list_remove_all(list, &testData2);
	ASSERT_TRUE(list != NULL) << "list should not be NULL after removing second element";
	ASSERT_EQ(&testData1, list->data) << "first list element data should be set";
	ASSERT_TRUE(list->prev == NULL) << "first list element should not have a previous element";
	ASSERT_TRUE(list->next != NULL) << "first list element should have a next element";
	ASSERT_EQ(&testData1, list->next->data) << "second list element data should be set";
	ASSERT_TRUE(list->next->prev == list) << "second list element should have first as previous element";
	ASSERT_TRUE(list->next->next == NULL) << "second list element should not have a next element";

	list = g_list_remove_all(list, &testData1);
	ASSERT_TRUE(list == NULL) << "list should be NULL after removing all elements";
}

TEST_F(GListTest, freeFull)
{
	int testData1 = 42;
	int testData2 = 1337;

	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);

	freeCallbacks.clear();
	g_list_free_full(list, test_free_callback);
	list = NULL;
	std::vector<gpointer> expectedCallbacks = {&testData1, &testData2, &testData1, &testData2};
	ASSERT_EQ(expectedCallbacks, freeCallbacks) << "actual callback list should match expected";
}

TEST_F(GListTest, alloc)
{
	GList *alloc = g_list_alloc();
	ASSERT_TRUE(alloc->data == NULL) << "allocated element data should not be set";
	ASSERT_TRUE(alloc->prev == NULL) << "allocated element should not have a previous element";
	ASSERT_TRUE(alloc->next == NULL) << "allocated element should not have a next element";
	g_list_free(alloc);
}

TEST_F(GListTest, length)
{
	guint length = g_list_length(list);
	ASSERT_EQ(0, length) << "list length should be zero before inserting elements";
	list = g_list_append(list, NULL);
	length = g_list_length(list);
	ASSERT_EQ(1, length) << "list length should be one after inserting an element";
	list = g_list_append(list, NULL);
	length = g_list_length(list);
	ASSERT_EQ(2, length) << "list length should be two after inserting another element";
	list = g_list_remove(list, NULL);
	length = g_list_length(list);
	ASSERT_EQ(1, length) << "list length should be one after deleting an element";
}

TEST_F(GListTest, copy)
{
	int testData1 = 42;
	int testData2 = 1337;

	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);

	GList *copiedList = g_list_copy(list);
	ASSERT_TRUE(copiedList != NULL) << "copied list should not be NULL";
	ASSERT_EQ(&testData1, copiedList->data) << "copied first list element data should be set";
	ASSERT_TRUE(copiedList->prev == NULL) << "copied first list element should not have a previous element";
	ASSERT_TRUE(copiedList->next != NULL) << "copied first list element should have a next element";
	GList *secondElement = copiedList->next;
	ASSERT_EQ(&testData2, secondElement->data) << "copied second list element data should be set";
	ASSERT_EQ(copiedList, secondElement->prev) << "copied second list element should have the first as previous element";
	ASSERT_TRUE(secondElement->next == NULL) << "copied second list element should not have a next element";
	ASSERT_NE(list, copiedList) << "copied list should not be equal to original list";
	ASSERT_NE(list->next, copiedList->next) << "copied list second element should not be equal to original list second element";
	g_list_free(copiedList);
}

TEST_F(GListTest, first)
{
	int testData = 42;

	GList *first = g_list_first(list);
	ASSERT_TRUE(first == NULL) << "first element of empty list should be NULL";
	list = g_list_append(list, &testData);
	first = g_list_first(list);
	ASSERT_TRUE(first != NULL) << "first element of appended list should not be NULL";
	ASSERT_EQ(list, first) << "first element of appended list should be equal to list";
	list = g_list_append(list, &testData);
	first = g_list_first(list);
	ASSERT_EQ(list, first) << "first element of list should be unchanged after appending an element";
}

TEST_F(GListTest, last)
{
	int testData = 42;

	GList *last = g_list_last(list);
	ASSERT_TRUE(last == NULL) << "last element of empty list should be NULL";
	list = g_list_append(list, &testData);
	last = g_list_last(list);
	ASSERT_TRUE(last != NULL) << "last element of appended list should not be NULL";
	ASSERT_EQ(list, last) << "last element of appended list should be equal to list";
	list = g_list_append(list, &testData);
	last = g_list_last(list);
	ASSERT_NE(list, last) << "last element of list should be changed after appending an element";
}