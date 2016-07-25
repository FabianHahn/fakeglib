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