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

static char *generateLongString(gsize size)
{
	char *longString = (char *) malloc((size + 1) * sizeof(char));
	for(gsize i = 0; i < size; i++) {
		longString[i] = (char) 48 + (i % 75);
	}
	longString[size] = '\0';
	return longString;
}

static void call_g_string_vprintf(GString *string, const gchar *format, ...)
{
	va_list args;
	va_start(args, format);
	g_string_vprintf(string, format, args);
	va_end(args);
}

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

TEST_F(GStringTest, sizedNew)
{
	gsize testSize = 27;

	g_string_free(string, true);
	string = g_string_sized_new(testSize);
	ASSERT_STREQ("", string->str) << "initialized string should be empty";
	ASSERT_EQ(0, string->len) << "initialized string should have length zero";
	ASSERT_GE(string->allocated_len, testSize) << "allocated length must be at least specified size";
}

TEST_F(GStringTest, vprintf)
{
	const char *testData = "asdf";

	g_string_free(string, true);
	string = g_string_new(testData);

	gsize oldAllocatedLen = string->allocated_len;
	char *longString = generateLongString(2 * oldAllocatedLen);
	call_g_string_vprintf(string, "%s", longString);
	ASSERT_STREQ(longString, string->str) << "printed string should match input";
	ASSERT_EQ(2 * oldAllocatedLen, string->len) << "new length should be double the old allocated size";
	ASSERT_LE(2 * oldAllocatedLen, string->allocated_len) << "allocated length should have at least doubled";

	oldAllocatedLen = string->allocated_len;
	call_g_string_vprintf(string, "%s", testData);
	ASSERT_STREQ(testData, string->str) << "printed string should match input";
	ASSERT_EQ(strlen(testData), string->len) << "string length should match input length";
	ASSERT_EQ(oldAllocatedLen, string->allocated_len) << "allocated length should be unchanged";

	free(longString);
}