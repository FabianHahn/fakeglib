#include <gtest/gtest.h>
#include <glib.h>

class GStringUtilTest : public ::testing::Test {
public:
	virtual void SetUp()
	{

	}

	virtual void TearDown()
	{

	}
};

TEST_F(GStringUtilTest, hasPrefix)
{
	const char *testData = "thebirdistheword";

	ASSERT_TRUE(g_str_has_prefix(testData, "the")) << "should pass happy case";
	ASSERT_TRUE(g_str_has_prefix(testData, "")) << "empty string should be a prefix";
	ASSERT_TRUE(g_str_has_prefix(testData, testData)) << "word should be a prefix of itself";
	ASSERT_FALSE(g_str_has_prefix(testData, "bird")) << "should fail unhappy case";
}

TEST_F(GStringUtilTest, hasSuffix)
{
	const char *testData = "thebirdistheword";

	ASSERT_TRUE(g_str_has_suffix(testData, "word")) << "should pass happy case";
	ASSERT_TRUE(g_str_has_suffix(testData, "")) << "empty string should be a suffix";
	ASSERT_TRUE(g_str_has_suffix(testData, testData)) << "word should be a suffix of itself";
	ASSERT_FALSE(g_str_has_suffix(testData, "bird")) << "should fail unhappy case";
}
