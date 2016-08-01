#include <gtest/gtest.h>
#include <glib.h>

class GStringTest : public ::testing::Test {
public:
	virtual void SetUp()
	{
		string = g_string_new("");
	}

	virtual void TearDown()
	{
		if(string != NULL) {
			g_string_free(string, true);
		}
	}

	GString *string;
};

TEST_F(GStringTest, newFree)
{
	const char *testData = "asdf";

	ASSERT_STREQ("", string->str) << "initial string should be equal to empty string";
	ASSERT_EQ(0, string->len) << "initial string length should be zero";
	ASSERT_GE(string->allocated_len, string->len) << "allocated length must be larger than length";

	gchar *segment = g_string_free(string, false);
	ASSERT_STREQ("", segment) << "preserved segment from freed initial string should be equal to empty string";
	g_free(segment);

	string = g_string_new(testData);
	ASSERT_NE(testData, string->str) << "initializing a string should copy input";
	ASSERT_STREQ(testData, string->str) << "initialized string should be equal to what it was initialized with";
	ASSERT_EQ(strlen(testData), string->len) << "initialized string should have correct length";
	ASSERT_GE(string->allocated_len, string->len) << "allocated length must be larger than length";
}

TEST_F(GStringTest, newLen)
{
	gchar testData[4] = {'a', 's', 'd', 'f'};

	g_string_free(string, true);
	string = g_string_new_len(testData, 4);
	ASSERT_NE(testData, string->str) << "initializing a string should copy input";
	ASSERT_STREQ("asdf", string->str) << "initialized string should be equal to what it was initialized with";
	ASSERT_EQ(4, string->len) << "initialized string should have correct length";
	ASSERT_GE(string->allocated_len, string->len) << "allocated length must be larger than length";
}
