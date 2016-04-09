#ifndef _ZT_GC_H_
#define _ZT_GC_H_

#include <zt.h>

BEGIN_C_DECLS

typedef struct zt_gc_collectable zt_gc_collectable_t;
struct zt_gc_collectable {
    zt_elist_t list;
    int      colour;
};

typedef struct zt_gc zt_gc_t;

/* initialize the garbage collector */
zt_gc_t * zt_gc_init(void *private_data,
                     void (*mark_fn)(struct zt_gc *, void *, void *),
                     void (*release_fn)(struct zt_gc *, void *, void **),
                     int marks_per_scan,
                     int allocs_to_scan);

/* destroy the garbage collector */
void zt_gc_destroy(zt_gc_t *gc);

/* enable collection (enabled by default) */
void zt_gc_enable(zt_gc_t *gc);

/* disable collection */
void zt_gc_disable(zt_gc_t *gc);

/* initialize the internal markers of a value */
void zt_gc_prepare(zt_gc_t *gc, void *value);

/* resiger a value with the GC */
void zt_gc_register(zt_gc_t *gc, void *value);

/* unregister a value with the GC */
void zt_gc_unregister(zt_gc_t *gc, void *value);

/* mark a value as un-releasable */
void zt_gc_protect(zt_gc_t *gc, void *value);

/* mark a value as releasable */
void zt_gc_unprotect(zt_gc_t *gc, void *value);

/* mark a value as in reachable by the GC */
int zt_gc_mark(zt_gc_t *gc, void *value);

/* run the collector on cycle */
void zt_gc_collect(zt_gc_t *gc, int full_scan);

/* utility */

/* dump a text representation of the GC internals */
void zt_gc_print_heap(zt_gc_t *gc);

/* loop over all the values in the internal tracking lists */
void zt_gc_for_each(zt_gc_t * gc, void (*cb)(void * value, void * data, int ty), void * cb_data);

END_C_DECLS

#endif /*_ZT_GC_H_*/
