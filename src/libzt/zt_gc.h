#ifndef _ZT_GC_H_
#define _ZT_GC_H_

#include <zt.h>

BEGIN_C_DECLS

typedef struct zt_gc_collectable zt_gc_collectable_t;
struct zt_gc_collectable {
    zt_elist_t list;
    int      colour;
};

enum {
    colour = 1,
    protected = 2,
};

typedef struct zt_gc zt_gc_t;
struct zt_gc {
    int           enabled;
    int           current_allocs;
    int           marks_per_scan;
    int           allocs_before_scan;
    int           rootset_size;
    int           rootset_next;
    int         (*is_white)(zt_gc_collectable_t *mark);
    void        (*clear_white)(zt_gc_collectable_t *mark);
    void        (*set_white)(zt_gc_collectable_t *mark);
    zt_elist_t  **rootset;
    zt_elist_t    list_1;
    zt_elist_t    list_2;
    zt_elist_t    list_3;
    zt_elist_t  * black; /* scanned */
    zt_elist_t  * grey;  /* seen but not scanned */
    zt_elist_t  * white; /* not seen */
    zt_elist_t  * scan; /* current offset into the grey list */
    void        * private_data;
    void        (*mark_fn)(struct zt_gc *, void *, void *);
    void        (*release_fn)(struct zt_gc *, void *, void **);
};

void zt_gc_init(zt_gc_t * gc,
                void *private_data,
                void (*mark_fn)(struct zt_gc *, void *, void *),
                void (*release_fn)(struct zt_gc *, void *, void **),
                int marks_per_scan,
                int allocs_to_scan);

void zt_gc_destroy(zt_gc_t *gc);

void zt_gc_enable(zt_gc_t *gc);
void zt_gc_disable(zt_gc_t *gc);

void zt_gc_prepare_value(zt_gc_t *gc, void *value);
void zt_gc_register_value(zt_gc_t *gc, void *value);
void zt_gc_unregister_value(zt_gc_t *gc, void *value);
void zt_gc_register_root(zt_gc_t *gc, void *value);

void zt_gc_protect(zt_gc_t *gc, void *value);
void zt_gc_unprotect(zt_gc_t *gc, void *value);

void zt_gc_print_heap(zt_gc_t *gc);
int zt_gc_mark_value(zt_gc_t *gc, void *value);
void zt_gc_scan(zt_gc_t *gc, int full_scan);
void zt_gc_for_each(zt_gc_t * gc, void (*cb)(void * value, void * data, int ty), void * cb_data);
/* void zt_gc_mark(gc *gc); */

END_C_DECLS

#endif /*_ZT_GC_H_*/
