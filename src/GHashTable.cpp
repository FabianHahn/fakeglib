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
	unsigned int refcount;
};

struct GHashTableIterPrivate {
	enum State {
		kUnknown,
		kInitialized,
		kIterating,
		kIteratingForwarded
	};

	GHashTable *hashTable;
	GHashTable::Map::iterator iter;
	State state;
};
static_assert(sizeof(GHashTableIterPrivate) <= sizeof(GHashTableIter), "the size of GHashTableIterPrivate exceeds the size of GHashTableIter on your platform");

template <typename T>
inline void placementDelete(T *x)
{
	x->~T();
}

FAKEGLIB_API GHashTable *g_hash_table_new(GHashFunc hash_func, GEqualFunc key_equal_func)
{
	GHashTable *hashTable = new GHashTableStruct();
	hashTable->functions.hash = hash_func;
	hashTable->functions.equal = key_equal_func;
	hashTable->functions.keyDestroy = NULL;
	hashTable->functions.mappedDestroy = NULL;
	hashTable->refcount = 1;
	return hashTable;
}

FAKEGLIB_API GHashTable *g_hash_table_new_full(GHashFunc hash_func, GEqualFunc key_equal_func, GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func)
{
	GHashTable *hashTable = new GHashTableStruct();
	hashTable->functions.hash = hash_func;
	hashTable->functions.equal = key_equal_func;
	hashTable->functions.keyDestroy = key_destroy_func;
	hashTable->functions.mappedDestroy = value_destroy_func;
	hashTable->refcount = 1;
	return hashTable;
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

FAKEGLIB_API gpointer g_hash_table_lookup(GHashTable *hashTable, gconstpointer keyValue)
{
	GHashTableKey key = { const_cast<void *>(keyValue), &hashTable->functions };

	GHashTable::Map::iterator query = hashTable->map.find(key);
	if(query != hashTable->map.end()) {
		return query->second.value;
	} else {
		return NULL;
	}
}

FAKEGLIB_API gboolean g_hash_table_lookup_extended(GHashTable *hashTable, gconstpointer lookupKeyValue, gpointer *origKeyValue, gpointer *mappedValue)
{
	GHashTableKey key = { const_cast<void *>(lookupKeyValue), &hashTable->functions };

	GHashTable::Map::iterator query = hashTable->map.find(key);
	if(query != hashTable->map.end()) {
		if(origKeyValue != NULL) {
			*origKeyValue = query->first.value;
		}
		if(mappedValue != NULL) {
			*mappedValue = query->second.value;
		}
		return true;
	} else {
		return false;
	}
}

FAKEGLIB_API void g_hash_table_foreach(GHashTable *hashTable, GHFunc callback, gpointer userData)
{
	GHashTable::Map::iterator end = hashTable->map.end();
	for(GHashTable::Map::iterator iter = hashTable->map.begin(); iter != end; ++iter) {
		callback(iter->first.value, iter->second.value, userData);
	}
}

FAKEGLIB_API gpointer g_hash_table_find(GHashTable *hashTable, GHRFunc predicate, gpointer userData)
{
	GHashTable::Map::iterator end = hashTable->map.end();
	for(GHashTable::Map::iterator iter = hashTable->map.begin(); iter != end; ++iter) {
		if(predicate(iter->first.value, iter->second.value, userData)) {
			return iter->second.value;
		}
	}
	return NULL;
}

FAKEGLIB_API gboolean g_hash_table_remove(GHashTable *hashTable, gconstpointer keyValue)
{
	GHashTableKey key = { const_cast<void *>(keyValue), &hashTable->functions };

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
		return true;
	} else {
		return false;
	}
}

FAKEGLIB_API gboolean g_hash_table_steal(GHashTable *hashTable, gconstpointer keyValue)
{
	GHashTableKey key = { const_cast<void *>(keyValue), &hashTable->functions };

	GHashTable::Map::iterator query = hashTable->map.find(key);
	if(query != hashTable->map.end()) {
		hashTable->map.erase(query);
		return true;
	}
	else {
		return false;
	}
}

FAKEGLIB_API guint g_hash_table_foreach_remove(GHashTable *hashTable, GHRFunc func, gpointer userData)
{
	guint removed = 0;

	for(GHashTable::Map::iterator iter = hashTable->map.begin(); iter != hashTable->map.end();) {
		if(func(iter->first.value, iter->second.value, userData)) {
			assert(&hashTable->functions == iter->first.functions);
			assert(&hashTable->functions == iter->second.functions);
			if(hashTable->functions.keyDestroy != NULL) {
				hashTable->functions.keyDestroy(iter->first.value);
			}
			if(hashTable->functions.mappedDestroy != NULL) {
				hashTable->functions.mappedDestroy(iter->second.value);
			}
			hashTable->map.erase(iter++);
			removed++;
		} else {
			++iter;
		}
	}

	return removed;
}

FAKEGLIB_API guint g_hash_table_foreach_steal(GHashTable *hashTable, GHRFunc func, gpointer userData)
{
	guint removed = 0;

	for(GHashTable::Map::iterator iter = hashTable->map.begin(); iter != hashTable->map.end();) {
		if(func(iter->first.value, iter->second.value, userData)) {
			hashTable->map.erase(iter++);
			removed++;
		} else {
			++iter;
		}
	}

	return removed;
}

FAKEGLIB_API void g_hash_table_remove_all(GHashTable *hashTable)
{
	for(GHashTable::Map::iterator iter = hashTable->map.begin(); iter != hashTable->map.end();) {
		assert(&hashTable->functions == iter->first.functions);
		assert(&hashTable->functions == iter->second.functions);
		if (hashTable->functions.keyDestroy != NULL) {
			hashTable->functions.keyDestroy(iter->first.value);
		}
		if (hashTable->functions.mappedDestroy != NULL) {
			hashTable->functions.mappedDestroy(iter->second.value);
		}
		hashTable->map.erase(iter++);
	}
}

FAKEGLIB_API void g_hash_table_steal_all(GHashTable *hashTable)
{
	hashTable->map.clear();
}

FAKEGLIB_API gpointer *g_hash_table_get_keys_as_array(GHashTable *hashTable, guint *length)
{
	*length = (guint) hashTable->map.size();
	gpointer *keys = (gpointer *) malloc((*length + 1) * sizeof(gpointer));

	guint i = 0;
	GHashTable::Map::iterator end = hashTable->map.end();
	for(GHashTable::Map::iterator iter = hashTable->map.begin(); iter != end; ++iter, ++i) {
		keys[i] = iter->first.value;
	}
	keys[*length] = NULL;

	return keys;
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

FAKEGLIB_API GHashTable *g_hash_table_ref(GHashTable *hashTable)
{
	hashTable->refcount++;
	return hashTable;
}

FAKEGLIB_API void g_hash_table_unref(GHashTable *hashTable)
{
	hashTable->refcount--;
	if(hashTable->refcount == 0) {
		g_hash_table_destroy(hashTable);
	}
}

FAKEGLIB_API void g_hash_table_iter_init(GHashTableIter *iter, GHashTable *hashTable)
{
	GHashTableIterPrivate *privateIter = reinterpret_cast<GHashTableIterPrivate *>(iter);
	privateIter->hashTable = hashTable;
	privateIter->state = GHashTableIterPrivate::kInitialized;
}

FAKEGLIB_API gboolean g_hash_table_iter_next(GHashTableIter *iter, gpointer *key, gpointer *value)
{
	GHashTableIterPrivate *privateIter = reinterpret_cast<GHashTableIterPrivate *>(iter);
	
	switch(privateIter->state) {
		case GHashTableIterPrivate::kInitialized: {
			new (&privateIter->iter) GHashTable::Map::iterator(privateIter->hashTable->map.begin());
			break;
		}
		case GHashTableIterPrivate::kIterating: {
			++privateIter->iter;
			break;
		}
		case GHashTableIterPrivate::kIteratingForwarded: {
			// iterator already forwarded
			break;
		}
		default: {
			assert(!"g_hash_table_iter_next: iterator in unknown state");
			return false;
		}
	}

	privateIter->state = GHashTableIterPrivate::kIterating;

	if(privateIter->iter != privateIter->hashTable->map.end()) {
		*key = privateIter->iter->first.value;
		*value = privateIter->iter->second.value;
		return true;
	} else {
		placementDelete(&privateIter->iter);
		privateIter->state = GHashTableIterPrivate::kUnknown;
		return false;
	}
}

FAKEGLIB_API gboolean g_str_equal(gconstpointer v1, gconstpointer v2)
{
	if(v1 == NULL && v2 == NULL) {
		return true;
	}
	if(v1 == NULL || v2 == NULL) {
		return false;
	}

	return strcmp((const char *) v1, (const char *) v2) == 0;
}

FAKEGLIB_API guint g_str_hash(gconstpointer v)
{
	if(v == NULL) {
		return 0;
	}

	std::string vString((const char *) v);
	return (guint) std::hash<std::string>{}(vString);
}