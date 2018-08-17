#ifdef HAVE_CONFIG_H
# include <libzt/zt_config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#include <stdio.h>

#define ZT_WITH_GC
#include "zt.h"
#include "zt_internal.h"

#define DEBUG_SCAN 0

enum {
    colour = 1,
    protected = 2,
};

struct zt_gc {
    int           enabled;
    int           current_allocs;
    int           marks_per_scan;
    int           allocs_before_scan;
    int         (*is_white)(zt_gc_collectable_t *mark);
    void        (*clear_white)(zt_gc_collectable_t *mark);
    void        (*set_white)(zt_gc_collectable_t *mark);
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

/*
 * Implements a form of Baker's treadmill with 'write protection'
 *
 */

static int one_is_white(zt_gc_collectable_t *mark)
{
    return ZT_BIT_ISSET(mark->colour, colour);             /* mark->colour == 1 ? 1 : 0; */
}
static void one_clear_white(zt_gc_collectable_t *mark)
{
    ZT_BIT_UNSET(mark->colour, colour);                    /* mark->colour = 0; */
}
static void one_set_white(zt_gc_collectable_t *mark)
{
    ZT_BIT_SET(mark->colour, colour);                      /* mark->colour = 1; */
}

static int zero_is_white(zt_gc_collectable_t *mark)
{
    return ZT_BIT_ISUNSET(mark->colour, colour);           /* mark->colour == 0 ? 1 : 0; */
}
static void zero_clear_white(zt_gc_collectable_t *mark)
{
    ZT_BIT_SET(mark->colour, colour);                      /* mark->colour = 1; */
}
static void zero_set_white(zt_gc_collectable_t *mark)
{
    ZT_BIT_UNSET(mark->colour, colour);                    /* mark->colour = 0; */
}

static int is_protected(zt_gc_collectable_t *mark)
{
    return ZT_BIT_ISSET(mark->colour, protected);
}

void
zt_gc_protect(zt_gc_t *gc, void *value)
{
    zt_gc_collectable_t * mark;

    mark = (zt_gc_collectable_t    *)value;

    gc->clear_white(mark);
    zt_elist_remove(&mark->list);
    zt_elist_add(gc->grey, &mark->list);
    ZT_BIT_SET(mark->colour, protected);
}

void
zt_gc_unprotect(zt_gc_t *gc UNUSED, void *value)
{
    zt_gc_collectable_t * mark = value;
    zt_assert(value);

    ZT_BIT_UNSET(mark->colour, protected);
}

static void switch_white(zt_gc_t *gc)
{
    if (gc->is_white == one_is_white) {
        gc->is_white = zero_is_white;
        gc->clear_white = zero_clear_white;
        gc->set_white = zero_set_white;
    } else {
        gc->is_white = one_is_white;
        gc->clear_white = one_clear_white;
        gc->set_white = one_set_white;
    }
}


static void
dump_elist(char *name, zt_elist_t *p)
{
    zt_elist_t * tmp;
    int        c;
    int        count;

    printf("%s %p [", name, (void *)p);
    c = 1;
    count = 0;
    zt_elist_for_each(p, tmp) {
        if (tmp->next == p) {
            c = 0;
        }
        if (is_protected(zt_elist_data(tmp, zt_gc_collectable_t, list))) {
            printf("p(%p)%s", (void *)tmp, c ? ", " : "");
        } else {
            printf("%p%s", (void *)tmp, c ? ", " : "");
        }
        if (++count >= 10) {
            printf("\n" BLANK, INDENT_TO(strlen(name) + 2, 1, 0));
            count = 0;
        }
    }
    printf("]\n");
}

void
zt_gc_enable(zt_gc_t *gc)
{
    gc->enabled++;
    zt_assert(gc->enabled <= 0);

    if (gc->enabled == 0 && gc->current_allocs >= gc->allocs_before_scan) {
        zt_gc_collect(gc, 0);
    }
}

void
zt_gc_disable(zt_gc_t *gc)
{
    gc->enabled--;
}


zt_gc_t *
zt_gc_init(void *private_data,
           void (*mark_fn)(struct zt_gc *, void *, void *),
           void (*release_fn)(struct zt_gc *, void *, void **),
           int marks_per_scan,
           int allocs_before_scan)
{
    zt_gc_t * gc = zt_calloc(zt_gc_t, 1);

    gc->enabled = 0;

    zt_elist_reset(&gc->list_1);
    zt_elist_reset(&gc->list_2);
    zt_elist_reset(&gc->list_3);

    gc->black = &gc->list_1;
    gc->grey = &gc->list_2;
    gc->white = &gc->list_3;

    gc->scan = gc->grey;

    gc->marks_per_scan = marks_per_scan;
    gc->allocs_before_scan = allocs_before_scan;
    gc->current_allocs = 0;

    /* initialize white */
    gc->is_white = one_is_white;
    gc->clear_white = one_clear_white;
    gc->set_white = one_set_white;

    gc->private_data = private_data;
    gc->mark_fn = mark_fn;
    gc->release_fn = release_fn;

    return gc;
}

void
zt_gc_destroy(zt_gc_t *gc)
{
    static zt_elist_t * elt;
    zt_elist_t * dont_use;

    if (gc->enabled != 0) {
        printf("# Warning: destroying gc while gc is disabled\n");
        gc->enabled = 0;
    }

    zt_gc_collect(gc, TRUE);

    zt_elist_for_each_safe(gc->white, elt, dont_use) {
        zt_elist_remove(elt);
        gc->release_fn(gc, gc->private_data, (void **)&elt);
    }

    zt_elist_for_each_safe(gc->grey, elt, dont_use) {
        zt_elist_remove(elt);
        gc->release_fn(gc, gc->private_data, (void **)&elt);
    }

    zt_elist_for_each_safe(gc->black, elt, dont_use) {
        zt_elist_remove(elt);
        gc->release_fn(gc, gc->private_data, (void **)&elt);
    }

    gc->enabled = 1;
}

void
zt_gc_prepare(zt_gc_t *gc UNUSED, void *v)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)v;

    zt_elist_reset(&mark->list);
}

void
zt_gc_unregister(zt_gc_t *gc UNUSED, void *v)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)v;

    zt_elist_remove(&mark->list);

    if (++gc->current_allocs < gc->allocs_before_scan) {
        zt_gc_collect(gc, 0);
    }
}

void
zt_gc_register(zt_gc_t *gc, void *v)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)v;

    zt_elist_reset(&mark->list);
    gc->clear_white(mark);
    /* The mutator is registering interest in this value so we must
     * place it in the the grey list not the white.
     */

    /* rather then place the object in white and require the need for a
     * write barrier we place the object in grey with the expectation that
     * it is immediatly used.  This means that we may hold a newly allocated
     * object longer then necessary (one cycle).
     */
    zt_elist_add_tail(gc->grey, &mark->list);

    if (++gc->current_allocs >= gc->allocs_before_scan) {
        zt_gc_collect(gc, 0);
    }
}

static void
zt_gc_free_white(zt_gc_t *gc)
{
    zt_elist_t          * elt = NULL;
    zt_elist_t          * dont_use = NULL;

    if (zt_elist_empty(gc->white)) {
        return;
    }

    zt_elist_for_each_safe(gc->white, elt, dont_use) {
        /* zt_gc_collectable_t * mark; */
        /* mark = zt_elist_data(elt, zt_gc_collectable_t, list); */

#if DEBUG_SCAN
        zt_assert(!is_protected(mark));
        zt_assert(elt != gc->grey);
        zt_assert(elt != gc->white);
        zt_assert(elt != gc->black);
#endif /* DEBUG_SCAN */

        zt_elist_remove(elt);
        gc->release_fn(gc, gc->private_data, (void **)&elt);
    }
}

static void
zt_gc_switch(zt_gc_t *gc)
{
    zt_elist_t * tmp_white;

    /* in a traditional GC we would scan and reset the values here we
     * free the white list and swap the white list with the black list
     */
    gc->enabled--;

    zt_gc_free_white(gc);

    tmp_white = gc->white;
    gc->white = gc->black;
    gc->black = tmp_white;

    switch_white(gc);

    gc->enabled++;
}

void
zt_gc_collect(zt_gc_t *gc, int full_scan)
{
    int current_marks = 0;

    if (gc->enabled < 0 ||
        (!full_scan && gc->current_allocs < gc->allocs_before_scan)) {
        return;
    }

    gc->enabled--;

#if DEBUG_SCAN
    printf("\n== Start Scan ==\n");
    zt_gc_print_heap(gc);
#endif /* DEBUG_SCAN */

    gc->current_allocs = 0;

    if (gc->scan == gc->grey) {
        /* beginning of a scan */
        gc->scan = gc->scan->next;
    }

    while (gc->scan != gc->grey) {
        /* move thru each object in the grey list moving it to the
         * black list and marking it as we go.
         */
        zt_elist_t          * next;
        zt_elist_t          * elt;
        zt_gc_collectable_t * mark;

        elt = gc->scan;

        mark = zt_elist_data(elt, zt_gc_collectable_t, list);

        gc->clear_white(mark);
        gc->mark_fn(gc, gc->private_data, elt);

        next = gc->scan->next;

        if (!is_protected(mark)) {
            /* if it is protected leave it in the grey list
             * otherwise move it to black
             */
            zt_elist_remove(elt);
            zt_elist_add(gc->black, elt);
        }

        gc->scan = next;

        if (!full_scan && ++current_marks >= gc->marks_per_scan) {
            if (gc->scan != gc->grey) {
                gc->enabled++;
#if DEBUG_SCAN
                printf("------\n");
                zt_gc_print_heap(gc);
                printf("== End Scan (Partial) ==\n\n");
#endif /* DEBUG_SCAN */
                return;
            }
        }
    }

    gc->enabled++;
#if DEBUG_SCAN
    printf("------\n");
    zt_gc_print_heap(gc);
    printf("== End Scan (Full) ==\n\n");
#endif /* DEBUG_SCAN */
    zt_gc_switch(gc);
} /* zt_gc_collect */

int
zt_gc_mark(zt_gc_t *gc, void *value)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)value;

    zt_assert(value != NULL);

    if (!is_protected(mark) && gc->is_white(mark)) {
        gc->clear_white(mark);
        zt_elist_remove(&mark->list);
        zt_elist_add_tail(gc->grey, &mark->list);
        return TRUE;
    }
    return FALSE;
}

void
zt_gc_print_heap(zt_gc_t *gc)
{
    printf("l1: %p l2: %p l3: %p\nblack: %p grey: %p white: %p\nscan: %p\n",
           (void *)&gc->list_1, (void *)&gc->list_2, (void *)&gc->list_3,
           (void *)gc->black, (void *)gc->grey, (void *)gc->white, (void *)gc->scan);


    dump_elist("Black", gc->black);
    dump_elist("Grey", gc->grey);
    dump_elist("White", gc->white);
}


void zt_gc_for_each(zt_gc_t * gc, void (*cb)(void * value, void * data, int ty), void * cb_data)
{
    zt_elist_t          * elt = NULL;
    zt_elist_t          * dont_use = NULL;
    zt_gc_collectable_t * mark;

    zt_elist_for_each_safe(gc->black, elt, dont_use) {
        mark = zt_elist_data(elt, zt_gc_collectable_t, list);
        cb(mark, cb_data, 0);
    }

    zt_elist_for_each_safe(gc->grey, elt, dont_use) {
        mark = zt_elist_data(elt, zt_gc_collectable_t, list);
        cb(mark, cb_data, 1);
    }

    zt_elist_for_each_safe(gc->white, elt, dont_use) {
        mark = zt_elist_data(elt, zt_gc_collectable_t, list);
        cb(mark, cb_data, 2);
    }
}
