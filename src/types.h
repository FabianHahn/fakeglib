#ifndef FAKEGLIB_TYPES_H
#define FAKEGLIB_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int guint;
typedef void * gpointer;
typedef const void * gconstpointer;
typedef bool gboolean;
typedef void (*GDestroyNotify)(gpointer data);

#ifdef __cplusplus
}
#endif

#endif