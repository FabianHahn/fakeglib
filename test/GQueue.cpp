#include <gtest/gtest.h>
#include <glib.h>

#include <vector>

static std::vector<gpointer> freeCallbacks;

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

static void test_free_callback(gpointer data)
{
	freeCallbacks.push_back(data);
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
