#ifndef FAKEGLIB_G_HASH_TABLE_H
#define FAKEGLIB_G_HASH_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"
#include "types.h"

typedef guint (*GHashFunc)(gconstpointer key);
typedef gboolean (*GEqualFunc)(gconstpointer a, gconstpointer b);

struct GHashTableStruct;
typedef struct GHashTableStruct GHashTable;

FAKEGLIB_API GHashTable *g_hash_table_new(GHashFunc hash_func, GEqualFunc key_equal_func);
FAKEGLIB_API GHashTable *g_hash_table_new_full(GHashFunc hash_func, GEqualFunc key_equal_func, GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func);
FAKEGLIB_API gboolean g_hash_table_insert(GHashTable *hash_table, gpointer key, gpointer value);
FAKEGLIB_API gboolean g_hash_table_replace(GHashTable *hash_table, gpointer key, gpointer value);
#define g_hash_table_add(hash_table, key) g_hash_table_replace(hash_table, key, key)
FAKEGLIB_API gboolean g_hash_table_contains(GHashTable *hash_table, gconstpointer key);
FAKEGLIB_API guint g_hash_table_size(GHashTable *hash_table);
FAKEGLIB_API gpointer g_hash_table_lookup(GHashTable *hash_table, gconstpointer key);
FAKEGLIB_API gboolean g_hash_table_lookup_extended(GHashTable *hash_table, gconstpointer lookup_key, gpointer *orig_key, gpointer *value);
FAKEGLIB_API void g_hash_table_destroy(GHashTable *hash_table);
FAKEGLIB_API gboolean g_str_equal(gconstpointer v1, gconstpointer v2);
FAKEGLIB_API guint g_str_hash(gconstpointer v);

#ifdef __cplusplus
}
#endif

#endif