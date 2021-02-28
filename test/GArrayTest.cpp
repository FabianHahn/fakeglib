#include <gtest/gtest.h>
#include <glib.h>

#include <vector>

static constexpr int testValue1 = 27;
static constexpr int testValue2 = 42;
static constexpr int testValue3 = 9001;

class GArrayTest : public ::testing::Test {
public:
	virtual void SetUp()
	{
		array = NULL;
	}

	virtual void TearDown()
	{
		g_array_free(array, /* free_segment */ true);
	}

	GArray *array;
};

TEST_F(GArrayTest, Fill)
{
	static constexpr guint numElements = 50;

	array = g_array_new(/* zero_terminated */ false, /* clear */ false, sizeof(guint));
	ASSERT_EQ(g_array_get_element_size(array), sizeof(int));
	ASSERT_EQ(array->len, 0);

	for (guint i = 0; i < numElements; i++) {
		g_array_append_val(array, i);
		ASSERT_EQ(array->len, i + 1);

		for (guint j = 0; j <= i; j++) {
			ASSERT_EQ(g_array_index(array, guint, j), j);
		}
	}
}

TEST_F(GArrayTest, FillZeroTerminated)
{
	static constexpr guint numElements = 50;

	array = g_array_new(/* zero_terminated */ true, /* clear */ false, sizeof(guint));
	ASSERT_EQ(g_array_get_element_size(array), sizeof(int));
	ASSERT_EQ(array->len, 0);

	for (guint i = 0; i < numElements; i++) {
		g_array_append_val(array, i);
		ASSERT_EQ(array->len, i + 1);

		for (guint j = 0; j <= i; j++) {
			ASSERT_EQ(g_array_index(array, guint, j), j);
		}
		ASSERT_EQ(g_array_index(array, guint, i + 1), 0);
	}
}

TEST_F(GArrayTest, Sized)
{
	static constexpr guint numElements = 50;

	array = g_array_sized_new(/* zero_terminated */ false, /* clear */ true, sizeof(guint), numElements);
	ASSERT_EQ(g_array_get_element_size(array), sizeof(int));
	ASSERT_EQ(array->len, 0);

	gchar *originalData = array->data;

	for (guint i = 0; i < numElements; i++) {
		g_array_append_val(array, i);
		ASSERT_EQ(array->len, i + 1);
		ASSERT_EQ(array->data, originalData) << "assume no reallocation";
	}
}

TEST_F(GArrayTest, Copy)
{
	static constexpr guint numElements = 50;

	array = g_array_new(/* zero_terminated */ true, /* clear */ false, sizeof(guint));
	for (guint i = 0; i < numElements; i++) {
		g_array_append_val(array, i);
	}

	GArray *copy = g_array_copy(array);
	ASSERT_EQ(copy->len, numElements);
	for (guint i = 0; i < numElements; i++) {
		ASSERT_EQ(g_array_index(array, guint, i), g_array_index(copy, guint, i));
	}

	g_array_index(array, guint, 17) = testValue1;
	ASSERT_NE(g_array_index(copy, guint, 17), testValue1);

	g_array_free(copy, /* free_segment */ true);

	ASSERT_EQ(array->len, numElements);
}

TEST_F(GArrayTest, Steal)
{
	static constexpr guint numElements = 50;

	array = g_array_new(/* zero_terminated */ true, /* clear */ false, sizeof(guint));
	for (guint i = 0; i < numElements; i++) {
		g_array_append_val(array, i);
	}

	gchar *originalData = array->data;
	guint originalLength = array->len;

	gsize stolenLength = 0;
	gpointer stolenData = g_array_steal(array, &stolenLength);
	ASSERT_EQ(stolenData, originalData);
	ASSERT_EQ(stolenLength, originalLength);
	ASSERT_NE(array->data, stolenData);
	ASSERT_EQ(array->len, 0);

	g_array_append_val(array, testValue1);
	ASSERT_NE(*reinterpret_cast<guint *>(stolenData), testValue1);

	free(stolenData);
}
