#include <cassert> // assert
#include <cstddef> // NULL
#include <cstring> // strcmp
#include <unordered_map>
#include <string>

#include "GHashTable.h"

struct GHashTableFunctions {
	GHashFunc hash;
	GEqualFunc equal;
	GDestroyNotify keyDestroy;
	GDestroyNotify mappedDestroy;
};

struct GHashTableKey {
	void *value;
	GHashTableFunctions *functions;
};

struct GHashTableMapped {
	void *value;
	GHashTableFunctions *functions;
};

struct GHashTableHash
{
	size_t operator()(const GHashTableKey& key) const
	{
		return key.functions->hash(key.value);
	}
};

struct GHashTableEqualTo
{
	bool operator()(const GHashTableKey& x, const GHashTableKey& y) const
	{
		assert(x.functions == y.functions);
		return x.functions->equal(x.value, y.value);
	}
};

struct GHashTableStruct {
	typedef std::pair<const GHashTableKey, GHashTableMapped> Entry;
	typedef std::unordered_map< GHashTableKey, GHashTableMapped, GHashTableHash, GHashTableEqualTo> Map;
	Map map;
	GHashTableFunctions functions;
};

FAKEGLIB_API GHashTable *g_hash_table_new(GHashFunc hash_func, GEqualFunc key_equal_func)
{
	GHashTable *hashtable = new GHashTableStruct();
	hashtable->functions.hash = hash_func;
	hashtable->functions.equal = key_equal_func;
	hashtable->functions.keyDestroy = NULL;
	hashtable->functions.mappedDestroy = NULL;
	return hashtable;
}

FAKEGLIB_API GHashTable *g_hash_table_new_full(GHashFunc hash_func, GEqualFunc key_equal_func, GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func)
{
	GHashTable *hashtable = new GHashTableStruct();
	hashtable->functions.hash = hash_func;
	hashtable->functions.equal = key_equal_func;
	hashtable->functions.keyDestroy = key_destroy_func;
	hashtable->functions.mappedDestroy = value_destroy_func;
	return hashtable;
}

FAKEGLIB_API gboolean g_hash_table_insert(GHashTable *hashTable, gpointer keyValue, gpointer mappedValue)
{
	GHashTableKey key = {keyValue, &hashTable->functions};
	GHashTableMapped mapped = {mappedValue, &hashTable->functions};
	GHashTable::Entry entry = std::make_pair(key, mapped);

	std::pair<GHashTable::Map::iterator, bool> ret = hashTable->map.insert(entry);
	if(ret.second) {
		return true;
	} else if(hashTable->functions.mappedDestroy != NULL) {
		assert(&hashTable->functions == ret.first->first.functions);
		assert(&hashTable->functions == ret.first->second.functions);
		if(hashTable->functions.keyDestroy != NULL) {
			hashTable->functions.keyDestroy(keyValue);
		}
		if(hashTable->functions.mappedDestroy != NULL) {
			hashTable->functions.mappedDestroy(ret.first->second.value);
		}
	}
	ret.first->second = mapped;
	return false;
}

FAKEGLIB_API gboolean g_hash_table_replace(GHashTable *hashTable, gpointer keyValue, gpointer mappedValue)
{
	GHashTableKey key = { keyValue, &hashTable->functions };
	GHashTableMapped mapped = { mappedValue, &hashTable->functions };

	bool newlyInserted = true;
	GHashTable::Map::iterator query = hashTable->map.find(key);
	if(query != hashTable->map.end()) {
		assert(&hashTable->functions == query->first.functions);
		assert(&hashTable->functions == query->second.functions);
		if(hashTable->functions.keyDestroy != NULL) {
			hashTable->functions.keyDestroy(query->first.value);
		}
		if(hashTable->functions.mappedDestroy != NULL) {
			hashTable->functions.mappedDestroy(query->second.value);
		}
		hashTable->map.erase(query);
		newlyInserted = false;
	}
	hashTable->map[key] = mapped;
	return newlyInserted;
}

FAKEGLIB_API gboolean g_hash_table_contains(GHashTable *hashTable, gconstpointer keyValue)
{
	GHashTableKey key = { const_cast<void *>(keyValue), &hashTable->functions };

	GHashTable::Map::iterator query = hashTable->map.find(key);
	return query != hashTable->map.end();
}

FAKEGLIB_API guint g_hash_table_size(GHashTable *hashTable)
{
	return (guint) hashTable->map.size();
}

FAKEGLIB_API void g_hash_table_destroy(GHashTable *hashTable)
{
	GHashTable::Map::iterator end = hashTable->map.end();
	for(GHashTable::Map::iterator iter = hashTable->map.begin(); iter != end; ++iter) {
		assert(&hashTable->functions == iter->first.functions);
		assert(&hashTable->functions == iter->second.functions);
		if(hashTable->functions.keyDestroy != NULL) {
			hashTable->functions.keyDestroy(iter->first.value);
		}
		if(hashTable->functions.mappedDestroy != NULL) {
			hashTable->functions.mappedDestroy(iter->second.value);
		}
	}
	delete hashTable;
}

FAKEGLIB_API gboolean g_str_equal(gconstpointer v1, gconstpointer v2)
{
	return strcmp((const char *) v1, (const char *) v2) == 0;
}

FAKEGLIB_API guint g_str_hash(gconstpointer v)
{
	std::string vString((const char *) v);
	return (guint) std::hash<std::string>{}(vString);
}