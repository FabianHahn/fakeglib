#include <gtest/gtest.h>
#include <glib.h>

#include <vector>

static std::vector<gpointer> freeCallbacks;
static std::vector<gpointer> foreachCallbacks;
static gpointer foreachLastUserData;
static gpointer compareLastUserData;

class GQueueTest : public ::testing::Test {
public:
	virtual void SetUp()
	{
		queue = g_queue_new();
	}

	virtual void TearDown()
	{
		if(queue != NULL) {
			g_queue_free(queue);
		}
	}

	GQueue *queue;
};

static gint test_compare_int_with_data(gconstpointer v1, gconstpointer v2, gpointer userData)
{
	compareLastUserData = userData;

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

static gint test_compare_int(gconstpointer v1, gconstpointer v2)
{
	return test_compare_int_with_data(v1, v2, NULL);
}

static void test_free_callback(gpointer data)
{
	freeCallbacks.push_back(data);
}

static void test_foreach_clear()
{
	foreachCallbacks.clear();
	foreachLastUserData = NULL;
}

static void test_foreach_callback(gpointer data, gpointer userData)
{
	foreachLastUserData = userData;
	foreachCallbacks.push_back(data);
}

TEST_F(GQueueTest, freeFull)
{
	int testData1 = 42;
	int testData2 = 1337;

	GList *list = NULL;
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	queue->head = list;
	queue->tail = list->next;
	queue->length = 2;

	g_queue_free_full(queue, test_free_callback);
	std::vector<gpointer> expectedCallbacks = {&testData1, &testData2};
	ASSERT_EQ(expectedCallbacks, freeCallbacks) << "actual callback list should match expected";

	queue = NULL;
}

TEST_F(GQueueTest, init)
{
	GList dummyList1;
	GList dummyList2;

	GQueue staticQueue;
	staticQueue.head = &dummyList1;
	staticQueue.tail = &dummyList2;
	staticQueue.length = 27;

	g_queue_init(&staticQueue);
	ASSERT_TRUE(staticQueue.head == NULL) << "init should set queue head to NULL";
	ASSERT_TRUE(staticQueue.tail == NULL) << "init should set queue tail to NULL";
	ASSERT_EQ(0, staticQueue.length) << "init should set queue length to zero";
}

TEST_F(GQueueTest, clear)
{
	int testData1 = 42;
	int testData2 = 1337;

	GList *list = NULL;
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	queue->head = list;
	queue->tail = list->next;
	queue->length = 2;

	g_queue_clear(queue);
	ASSERT_TRUE(queue->head == NULL) << "clear should set queue head to NULL";
	ASSERT_TRUE(queue->tail == NULL) << "clear should set queue tail to NULL";
	ASSERT_EQ(0, queue->length) << "clear should set queue length to zero";
}

TEST_F(GQueueTest, isEmpty)
{
	int testData = 42;

	gboolean empty = g_queue_is_empty(queue);
	ASSERT_TRUE(empty) << "initial queue should be empty";

	GList *list = NULL;
	list = g_list_append(list, &testData);
	queue->head = list;
	queue->tail = list;
	queue->length = 1;

	empty = g_queue_is_empty(queue);
	ASSERT_FALSE(empty) << "queue with element should not be empty";
}

TEST_F(GQueueTest, getLength)
{
	int testData = 42;

	guint length = g_queue_get_length(queue);
	ASSERT_EQ(0, length) << "initial queue should have length zero";

	GList *list = NULL;
	list = g_list_append(list, &testData);
	queue->head = list;
	queue->tail = list;
	queue->length = 1;

	length = g_queue_get_length(queue);
	ASSERT_EQ(1, length) << "queue with element should have length one";
}

TEST_F(GQueueTest, reverse)
{
	int testData1 = 42;
	int testData2 = 1337;
	int testData3 = 27;

	GList *list = NULL;
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	list = g_list_append(list, &testData3);
	queue->head = list;
	queue->tail = list->next->next;
	queue->length = 3;

	g_queue_reverse(queue);
	ASSERT_EQ(3, queue->length) << "reversed queue should still have length three";
	ASSERT_EQ(list, queue->tail) << "reversed queue tail should be original head";
	ASSERT_EQ(&testData3, queue->head->data) << "first reversed queue element data should be set";
	ASSERT_TRUE(queue->head->prev == NULL) << "first reversed queue element should not have a previous element";
	ASSERT_TRUE(queue->head->next != NULL) << "first reversed queue element should have a next element";
	ASSERT_EQ(&testData2, queue->head->next->data) << "second reversed queue element data should be set";
	ASSERT_EQ(queue->head, queue->head->next->prev) << "second reversed queue element should have first element as previous";
	ASSERT_TRUE(queue->head->next->next != NULL) << "second queue element should have a next element";
	ASSERT_EQ(queue->tail, queue->head->next->next) << "last element should be equal to queue tail";
	ASSERT_EQ(&testData1, queue->tail->data) << "third reversed queue element data should be set";
	ASSERT_EQ(queue->head->next, queue->tail->prev) << "third reversed queue element should have second element as previous";
	ASSERT_TRUE(queue->tail->next == NULL) << "third reversed queue element should not have a next element";
	g_queue_reverse(queue);
	ASSERT_EQ(list, queue->head) << "reverse of reversed queue should result in original queue";
}

TEST_F(GQueueTest, copy)
{
	int testData1 = 42;
	int testData2 = 1337;

	GList *list = NULL;
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	queue->head = list;
	queue->tail = list->next->next;
	queue->length = 2;

	GQueue *copiedQueue = g_queue_copy(queue);
	ASSERT_EQ(&testData1, copiedQueue->head->data) << "copied first queue element data should be set";
	ASSERT_TRUE(copiedQueue->head->prev == NULL) << "copied first queue element should not have a previous element";
	ASSERT_TRUE(copiedQueue->head->next != NULL) << "copied first queue element should have a next element";
	ASSERT_EQ(copiedQueue->tail, copiedQueue->head->next) << "copied first queue element next should be equal to queue tail";
	ASSERT_EQ(&testData2, copiedQueue->tail->data) << "copied second queue element data should be set";
	ASSERT_EQ(copiedQueue->head, copiedQueue->tail->prev) << "copied second queue element should have the first as previous element";
	ASSERT_TRUE(copiedQueue->tail->next == NULL) << "copied second queue element should not have a next element";
	ASSERT_NE(queue->head, copiedQueue->head) << "copied queue should not be equal to original queue";
	ASSERT_NE(queue->tail, copiedQueue->tail) << "copied queue second element should not be equal to original queue second element";
	g_queue_free(copiedQueue);
}

TEST_F(GQueueTest, foreach)
{
	int testData1 = 42;
	int testData2 = 1337;
	int testUserData = 27;

	GList *list = NULL;
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	list = g_list_append(list, &testData2);
	queue->head = list;
	queue->tail = list->next->next;
	queue->length = 3;

	test_foreach_clear();
	g_queue_foreach(queue, test_foreach_callback, &testUserData);
	ASSERT_EQ(&testUserData, foreachLastUserData) << "passed user data should have expected value";
	std::vector<gpointer> expectedCallbacks = {&testData1, &testData2, &testData2};
	ASSERT_EQ(expectedCallbacks, foreachCallbacks) << "actual callback list should match expected";
}

TEST_F(GQueueTest, find)
{
	int testData1 = 42;
	int testData2 = 1337;
	int testData3 = 27;

	GList *list = NULL;
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	queue->head = list;
	queue->tail = list->next->next;
	queue->length = 2;

	GList *find = g_queue_find(queue, &testData1);
	ASSERT_EQ(list, find) << "first element should be found";
	find = g_queue_find(queue, &testData2);
	ASSERT_EQ(list->next, find) << "second element should be found";
	find = g_queue_find(queue, &testData3);
	ASSERT_TRUE(find == NULL) << "unknown element should not be found";
}

TEST_F(GQueueTest, findCustom)
{
	int testData1 = 42;
	int testData2 = 1337;
	int searchData1 = testData1;
	int searchData2 = testData2;
	int searchData3 = 27;

	GList *list = NULL;
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	queue->head = list;
	queue->tail = list->next->next;
	queue->length = 2;

	GList *find = g_queue_find_custom(queue, &searchData1, test_compare_int);
	ASSERT_EQ(list, find) << "first element should be found";
	find = g_queue_find_custom(queue, &searchData2, test_compare_int);
	ASSERT_EQ(list->next, find) << "second element should be found";
	find = g_queue_find_custom(queue, &searchData3, test_compare_int);
	ASSERT_TRUE(find == NULL) << "unknown element should not be found";
}

TEST_F(GQueueTest, sort)
{
	int testData1 = 42;
	int testData2 = 1;
	int testData3 = 27;
	int testData4 = 27;
	int testData5 = 5;
	int testData6 = 3;
	int testData7 = 2;
	int testUserData = 1337;

	GList *list = NULL;
	list = g_list_append(list, &testData1);
	list = g_list_append(list, &testData2);
	list = g_list_append(list, &testData3);
	list = g_list_append(list, &testData4);
	list = g_list_append(list, &testData5);
	list = g_list_append(list, &testData6);
	list = g_list_append(list, &testData7);
	queue->head = list;
	queue->tail = g_list_last(list);
	queue->length = 7;

	compareLastUserData = NULL;
	g_queue_sort(queue, test_compare_int_with_data, &testUserData);
	ASSERT_EQ(7, queue->length) << "sorting queue should not change its length";
	ASSERT_EQ(&testUserData, compareLastUserData) << "passed user data should have expected value";
	std::vector<gpointer> expectedSolution = {&testData2, &testData7, &testData6, &testData5, &testData3, &testData4, &testData1};
	GList *previous = NULL;
	int i = 0;
	for(GList *iter = queue->head; iter != NULL; previous = iter, iter = iter->next, i++) {
		ASSERT_EQ(expectedSolution[i], iter->data) << "sorted list element " << i << " should have expected value";
		ASSERT_EQ(previous, iter->prev) << "sorted list element " << i << " previous pointer should be correct";
	}

	compareLastUserData = NULL;
	g_queue_sort(queue, test_compare_int_with_data, &testUserData);
	ASSERT_EQ(7, queue->length) << "sorting queue should not change its length";
	ASSERT_EQ(&testUserData, compareLastUserData) << "passed user data should have expected value";
	previous = NULL;
	i = 0;
	for(GList *iter = queue->head; iter != NULL; previous = iter, iter = iter->next, i++) {
		ASSERT_EQ(expectedSolution[i], iter->data) << "resorted list element " << i << " should have expected value";
		ASSERT_EQ(previous, iter->prev) << "resorted list element " << i << " previous pointer should be correct";
	}
}

TEST_F(GQueueTest, pushHead)
{
	int testData1 = 42;
	int testData2 = 1337;

	g_queue_push_head(queue, &testData1);
	ASSERT_TRUE(queue->head != NULL) << "queue head should not be NULL after inserting an element";
	ASSERT_EQ(queue->head, queue->tail) << "queue head should be equal to tail after inserting a single element";
	ASSERT_EQ(&testData1, queue->head->data) << "first queue element data should be set correctly";
	ASSERT_TRUE(queue->head->next == NULL) << "queue element should not have a next element";
	ASSERT_TRUE(queue->head->prev == NULL) << "queue element should not have a previous element";
	ASSERT_EQ(1, queue->length) << "queue length should be one after inserting an element";
	GList *oldHead = queue->head;
	g_queue_push_head(queue, &testData2);
	ASSERT_NE(oldHead, queue->head) << "queue head should have changed after inserting another element";
	ASSERT_EQ(oldHead, queue->tail) << "queue tail should have become tail after pushing another element to head";
	ASSERT_EQ(&testData1, queue->tail->data) << "first queue element data should be set correctly";
	ASSERT_TRUE(queue->tail->next == NULL) << "queue tail should not have a next element";
	ASSERT_EQ(queue->head, queue->tail->prev) << "queue tail should have head as previous element";
	ASSERT_EQ(queue->tail, queue->head->next) << "queue head should have tail as next element";
	ASSERT_TRUE(queue->head->prev == NULL) << "queue head should not have a previous element";
	ASSERT_EQ(&testData2, queue->head->data) << "second queue element data should be set correctly";
	ASSERT_EQ(2, queue->length) << "queue length should be two after inserting another element";
}

TEST_F(GQueueTest, pushTail)
{
	int testData1 = 42;
	int testData2 = 1337;

	g_queue_push_tail(queue, &testData1);
	ASSERT_TRUE(queue->head != NULL) << "queue head should not be NULL after inserting an element";
	ASSERT_EQ(queue->head, queue->tail) << "queue head should be equal to tail after inserting a single element";
	ASSERT_EQ(&testData1, queue->head->data) << "first queue element data should be set correctly";
	ASSERT_TRUE(queue->head->next == NULL) << "queue element should not have a next element";
	ASSERT_TRUE(queue->head->prev == NULL) << "queue element should not have a previous element";
	ASSERT_EQ(1, queue->length) << "queue length should be one after inserting an element";
	GList *oldHead = queue->head;
	g_queue_push_tail(queue, &testData2);
	ASSERT_EQ(oldHead, queue->head) << "queue head should not have changed after inserting another element";
	ASSERT_NE(oldHead, queue->tail) << "queue tail should have changed after pushing another element to tail";
	ASSERT_EQ(&testData2, queue->tail->data) << "second queue element data should be set correctly";
	ASSERT_TRUE(queue->tail->next == NULL) << "queue tail should not have a next element";
	ASSERT_EQ(queue->head, queue->tail->prev) << "queue tail should have head as previous element";
	ASSERT_EQ(queue->tail, queue->head->next) << "queue head should have tail as next element";
	ASSERT_TRUE(queue->head->prev == NULL) << "queue head should not have a previous element";
	ASSERT_EQ(&testData1, queue->head->data) << "first queue element data should be set correctly";
	ASSERT_EQ(2, queue->length) << "queue length should be two after inserting another element";
}

TEST_F(GQueueTest, pushNth)
{
	int testData1 = 42;
	int testData2 = 1337;
	int testData3 = 27;

	g_queue_push_nth(queue, &testData1, 0);
	ASSERT_TRUE(queue->head != NULL) << "queue head should not be empty ater inserting an element";
	ASSERT_EQ(queue->head, queue->tail) << "queue head should be equal to tail after inserting a single element";
	ASSERT_EQ(&testData1, queue->head->data) << "first queue element data should be set correctly";
	ASSERT_TRUE(queue->head->next == NULL) << "single queue element should not have a next element";
	ASSERT_TRUE(queue->head->prev == NULL) << "single queue element should not have a previous element";
	ASSERT_EQ(1, queue->length) << "queue length should be zero after inserting an element";
	GList *oldHead = queue->head;
	g_queue_push_nth(queue, &testData2, 1);
	ASSERT_EQ(oldHead, queue->head) << "queue head should not have changed after inserting another element";
	ASSERT_NE(oldHead, queue->tail) << "queue tail should have changed after pushing another element to tail";
	ASSERT_EQ(&testData2, queue->tail->data) << "second queue element data should be set correctly";
	ASSERT_TRUE(queue->tail->next == NULL) << "queue tail should not have a next element";
	ASSERT_EQ(queue->head, queue->tail->prev) << "queue tail should have head as previous element";
	ASSERT_EQ(queue->tail, queue->head->next) << "queue head should have tail as next element";
	ASSERT_TRUE(queue->head->prev == NULL) << "queue head should not have a previous element";
	ASSERT_EQ(&testData1, queue->head->data) << "first queue element data should be set correctly";
	ASSERT_EQ(2, queue->length) << "queue length should be two after inserting another element";
	GList *oldTail = queue->tail;
	g_queue_push_nth(queue, &testData3, 1);
	ASSERT_EQ(oldHead, queue->head) << "queue head should not have changed after inserting another element";
	ASSERT_EQ(oldTail, queue->tail) << "queue tail should not have changed after inserting another element";
	ASSERT_EQ(oldHead->next, oldTail->prev) << "third element should have been inserted between head and tail";
	ASSERT_EQ(&testData3, oldTail->prev->data) << "third element data should be set correctly";
	ASSERT_EQ(oldTail, oldTail->prev->next) << "third element next should point to tail";
	ASSERT_EQ(oldHead, oldTail->prev->prev) << "third element previous should point to head";
	ASSERT_EQ(3, queue->length) << "queue length should be three after inserting another element";
}

TEST_F(GQueueTest, popHead)
{
	int testData1 = 42;
	int testData2 = 1337;

	g_queue_push_tail(queue, &testData1);
	g_queue_push_tail(queue, &testData2);

	gpointer data = g_queue_pop_head(queue);
	ASSERT_EQ(&testData1, data) << "popped queue head should have correct value";
	ASSERT_TRUE(queue->head != NULL) << "queue head should not be NULL after removing one element";
	ASSERT_EQ(queue->head, queue->tail) << "queue tail should be equal to head after removing one element";
	ASSERT_TRUE(queue->head->next == NULL) << "queue element should not have a next element";
	ASSERT_TRUE(queue->head->prev == NULL) << "queue element should not have a previous element";
	ASSERT_EQ(1, queue->length) << "queue should have one element left after popping one";
	data = g_queue_pop_head(queue);
	ASSERT_EQ(&testData2, data) << "popped queue head should have correct value";
	ASSERT_TRUE(queue->head == NULL) << "queue head should be NULL after popping another element";
	ASSERT_TRUE(queue->tail == NULL) << "queue tail should be NULL after popping another element";
	ASSERT_EQ(0, queue->length) << "queue length should be zero after popping another element";
}

TEST_F(GQueueTest, popTail)
{
	int testData1 = 42;
	int testData2 = 1337;

	g_queue_push_tail(queue, &testData1);
	g_queue_push_tail(queue, &testData2);

	gpointer data = g_queue_pop_tail(queue);
	ASSERT_EQ(&testData2, data) << "popped queue tail should have correct value";
	ASSERT_TRUE(queue->head != NULL) << "queue head should not be NULL after removing one element";
	ASSERT_EQ(queue->head, queue->tail) << "queue tail should be equal to head after removing one element";
	ASSERT_TRUE(queue->head->next == NULL) << "queue element should not have a next element";
	ASSERT_TRUE(queue->head->prev == NULL) << "queue element should not have a previous element";
	ASSERT_EQ(1, queue->length) << "queue should have one element left after popping one";
	data = g_queue_pop_tail(queue);
	ASSERT_EQ(&testData1, data) << "popped queue tail should have correct value";
	ASSERT_TRUE(queue->head == NULL) << "queue head should be NULL after popping another element";
	ASSERT_TRUE(queue->tail == NULL) << "queue tail should be NULL after popping another element";
	ASSERT_EQ(0, queue->length) << "queue length should be zero after popping another element";
}

TEST_F(GQueueTest, popNth)
{
	int testData1 = 42;
	int testData2 = 1337;
	int testData3 = 27;

	g_queue_push_tail(queue, &testData1);
	g_queue_push_tail(queue, &testData2);
	g_queue_push_tail(queue, &testData3);

	gpointer data = g_queue_pop_nth(queue, 1);
	ASSERT_EQ(&testData2, data) << "popped queue head should have correct value";
	ASSERT_TRUE(queue->head != NULL) << "queue head should not be NULL after removing one element";
	ASSERT_NE(queue->head, queue->tail) << "queue tail should not be equal to head after removing one element";
	ASSERT_EQ(&testData1, queue->head->data) << "queue head data should be set correctly";
	ASSERT_EQ(queue->tail, queue->head->next) << "queue head element next should be tail";
	ASSERT_TRUE(queue->head->prev == NULL) << "queue head should not have a previous element";
	ASSERT_EQ(&testData3, queue->tail->data) << "queue tail data should be set correctly";
	ASSERT_EQ(queue->head, queue->tail->prev) << "queue tail element previous should be head";
	ASSERT_TRUE(queue->tail->next == NULL) << "queue tail should not have a next element";
	ASSERT_EQ(2, queue->length) << "queue should have one element left after popping one";
}

TEST_F(GQueueTest, peek)
{
	int testData1 = 42;
	int testData2 = 1337;

	g_queue_push_tail(queue, &testData1);
	g_queue_push_tail(queue, &testData2);

	gpointer data = g_queue_peek_head(queue);
	ASSERT_EQ(&testData1, data) << "peeking at the head of the queue should produce expected value";
	data = g_queue_peek_tail(queue);
	ASSERT_EQ(&testData2, data) << "peeking at the tail of the queue should produce expected value";
	data = g_queue_peek_nth(queue, 0);
	ASSERT_EQ(&testData1, data) << "peeking at the first element of the queue should produce expected value";
	data = g_queue_peek_nth(queue, 1);
	ASSERT_EQ(&testData2, data) << "peeking at the second element of the queue should produce expected value";
}
