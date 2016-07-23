#include <gtest/gtest.h>
#include <glib.h>

static guint test_hash(gconstpointer key);
static gboolean test_equal(gconstpointer a, gconstpointer b);
static void test_free_key(gpointer data);
static void test_free_value(gpointer data);

class GHashTableTest;
static GHashTableTest *currentTest = NULL;

class GHashTableTest : public ::testing::Test {
public:
	virtual void SetUp()
	{
		hash = g_str_hash;
		equal = g_str_equal;
		keyDestroy = NULL;
		valueDestroy = NULL;
		hashTable = NULL;
		created = false;

		currentTest = this;
	}

	virtual void Create()
	{
		hashTable = g_hash_table_new_full(test_hash, test_equal, test_free_key, test_free_value);
		created = true;
	}

	virtual void Destroy()
	{
		if(created) {
			g_hash_table_destroy(hashTable);
			hashTable = NULL;
			created = false;
		}
	}

	virtual void TearDown()
	{
		Destroy();

		hashed.clear();
		equaled.clear();
		keyDestroyed.clear();
		valueDestroyed.clear();

		currentTest = NULL;
	}

	GHashFunc hash;
	GEqualFunc equal;
	GDestroyNotify keyDestroy;
	GDestroyNotify valueDestroy;
	GHashTable *hashTable;
	bool created;
	std::vector<gconstpointer> hashed;
	std::vector< std::pair<gconstpointer, gconstpointer> > equaled;
	std::vector<gpointer> keyDestroyed;
	std::vector<gpointer> valueDestroyed;
};

static guint test_hash(gconstpointer key)
{
	currentTest->hashed.push_back(key);
	return currentTest->hash(key);
}

static gboolean test_equal(gconstpointer a, gconstpointer b)
{
	if(std::less<gconstpointer>{}(a, b)) {
		currentTest->equaled.push_back(std::make_pair(a, b));
	} else {
		currentTest->equaled.push_back(std::make_pair(b, a));
	}
	
	return currentTest->equal(a, b);
}

static void test_free_key(gpointer data)
{
	currentTest->keyDestroyed.push_back(data);

	if(currentTest->keyDestroy != NULL) {
		currentTest->keyDestroy(data);
	}
}

static void test_free_value(gpointer data)
{
	currentTest->valueDestroyed.push_back(data);

	if(currentTest->valueDestroy != NULL) {
		currentTest->valueDestroy(data);
	}
}

TEST_F(GHashTableTest, insert)
{
	Create();
	const char *testKey = "foo";
	int testValue = 1337;

	gboolean inserted = g_hash_table_insert(hashTable, (gpointer) testKey, &testValue);
	ASSERT_TRUE(inserted) << "inserted elemented should not have existed yet";
	ASSERT_EQ(hashed.size(), 1) << "inserted element should have been hashed";
	ASSERT_EQ(hashed[0], testKey) << "inserted element should have been hashed";
}

TEST_F(GHashTableTest, insertTwice)
{
	Create();
	const char *testFirstKey = "foo";
	const char *testSecondKey = "foo";
	int testFirstValue = 1337;
	int testSecondValue = 42;

	g_hash_table_insert(hashTable, (gpointer) testFirstKey, &testFirstValue);
	gboolean inserted = g_hash_table_insert(hashTable, (gpointer) testSecondKey, &testSecondValue);
	ASSERT_FALSE(inserted) << "twice inserted elemented should already exist";
	ASSERT_EQ(equaled.size(), 1) << "inserted elements should have been equaled";
	ASSERT_EQ(equaled[0], std::make_pair((gconstpointer) testFirstKey, (gconstpointer) testSecondKey)) << "inserted elements should have been equaled";
	ASSERT_EQ(keyDestroyed.size(), 1) << "inserted second element should have had its key destroyed";
	ASSERT_EQ(keyDestroyed[0], (gpointer) testSecondKey) << "inserted second element should have had its key destroyed";
	ASSERT_EQ(valueDestroyed.size(), 1) << "inserted first element should have had its value destroyed";
	ASSERT_EQ(valueDestroyed[0], (gpointer) &testFirstValue) << "inserted first element should have had its value destroyed";
}

TEST_F(GHashTableTest, insertReplace)
{
	Create();
	const char *testFirstKey = "foo";
	const char *testSecondKey = "foo";
	int testFirstValue = 1337;
	int testSecondValue = 42;

	g_hash_table_insert(hashTable, (gpointer) testFirstKey, &testFirstValue);
	gboolean inserted = g_hash_table_replace(hashTable, (gpointer) testSecondKey, &testSecondValue);
	ASSERT_FALSE(inserted) << "twice inserted elemented should already exist";
	ASSERT_GE(equaled.size(), 1) << "inserted elements should have been equaled at least once";
	ASSERT_EQ(equaled[0], std::make_pair((gconstpointer) testFirstKey, (gconstpointer) testSecondKey)) << "inserted elements should have been equaled";
	ASSERT_EQ(keyDestroyed.size(), 1) << "inserted second element should have replaced key of first element";
	ASSERT_EQ(keyDestroyed[0], (gpointer) testFirstKey) << "inserted second element should have replaced key of first element";
	ASSERT_EQ(valueDestroyed.size(), 1) << "inserted first element should have had its value destroyed";
	ASSERT_EQ(valueDestroyed[0], (gpointer) &testFirstValue) << "inserted first element should have had its value destroyed";
}

TEST_F(GHashTableTest, contains)
{
	Create();
	const char *testKey = "foo";
	int testValue = 1337;

	gboolean contains = g_hash_table_contains(hashTable, (gpointer) testKey);
	ASSERT_FALSE(contains) << "hash table should not contain element before it is inserted";
	g_hash_table_insert(hashTable, (gpointer) testKey, &testValue);
	contains = g_hash_table_contains(hashTable, (gpointer) testKey);
	ASSERT_TRUE(contains) << "hash table should contain element after it was inserted";
}

TEST_F(GHashTableTest, freeInserted)
{
	Create();
	const char *testKey = "foo";
	int testValue = 1337;

	g_hash_table_insert(hashTable, (gpointer) testKey, &testValue);
	ASSERT_EQ(keyDestroyed.size(), 0) << "inserted element should not have its key destroyed yet";
	ASSERT_EQ(valueDestroyed.size(), 0) << "inserted element should not have its value destroyed yet";

	Destroy();
	ASSERT_EQ(keyDestroyed.size(), 1) << "inserted element should have had its key destroyed";
	ASSERT_EQ(keyDestroyed[0], (gpointer) testKey) << "inserted element should have had its key destroyed";
	ASSERT_EQ(valueDestroyed.size(), 1) << "inserted element should have had its value destroyed";
	ASSERT_EQ(valueDestroyed[0], (gpointer) &testValue) << "inserted element should have had its value destroyed";
}