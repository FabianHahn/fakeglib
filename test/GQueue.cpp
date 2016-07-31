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
