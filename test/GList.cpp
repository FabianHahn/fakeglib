#include <gtest/gtest.h>
#include <glib.h>

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