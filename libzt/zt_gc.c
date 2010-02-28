#include "zt_macros.h"
#include "zt_assert.h"

#define ZT_EXCEPT_GC_DEFINE
#include "zt_gc.h"

/*
 * Implements a form of Baker's treadmill with 'write protection'
 *
 */

static int one_is_white(zt_gc_collectable_t *mark)
{
    return( ZT_BIT_ISSET(mark->colour, colour)) ;          /* mark->colour == 1 ? 1 : 0; */
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
    return( ZT_BIT_ISUNSET(mark->colour, colour)) ;        /* mark->colour == 0 ? 1 : 0; */
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
    return( ZT_BIT_ISSET(mark->colour, protected)) ;
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
zt_gc_unprotect(zt_gc_t *gc, void *value)
{
    zt_gc_collectable_t * mark;

    mark = (zt_gc_collectable_t    *)value;

    /*
     * zt_elist_remove(&mark->list);
     * zt_elist_add_tail(gc->grey, &mark->list);
     */

    ZT_BIT_UNSET(((zt_gc_collectable_t *)value)->colour, protected);
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
resize_rootset(zt_gc_t *gc, int new_size)
{
    if (gc->rootset == NULL) {
        gc->rootset = XCALLOC(zt_elist *, new_size);
        gc->rootset_next = 0;
    } else {
        int diff = new_size - gc->rootset_size;
        int i;
        gc->rootset = XREALLOC(zt_elist *, gc->rootset, new_size);
        for ( i = diff; i < new_size; i++) {
            gc->rootset[i] = NULL;
        }
    }
    gc->rootset_size = new_size;
}

static void
dump_elist(char *name, zt_elist *p)
{
    zt_elist * tmp;
    int        c;
    int        count;

    printf("%s %p [", name, (void *)p);
    c = 1;
    count = 0;
    zt_elist_for_each(p, tmp){
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
    if (gc->enabled > 0) {
        TRY_THROW(zt_exception.gc.unbalanced_enable);
    }

    if (gc->enabled == 0 && gc->current_allocs >= gc->allocs_before_scan) {
        zt_gc_scan(gc, 0);
    }
}

void
zt_gc_disable(zt_gc_t *gc)
{
    gc->enabled--;
}


void
zt_gc_init(zt_gc_t *gc,
           void *private_data,
           void (*mark_fn)(struct zt_gc *, void *, void *),
           void (*release_fn)(struct zt_gc *, void *, void **),
           int marks_per_scan,
           int allocs_before_scan)
{
    gc->enabled = 0;

    zt_elist_reset(&gc->list_1);
    zt_elist_reset(&gc->list_2);
    zt_elist_reset(&gc->list_3);

    gc->rootset = NULL;
    resize_rootset(gc, 1024);

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
}

void
zt_gc_destroy(zt_gc_t *gc)
{
    zt_elist * elt;
    zt_elist * dont_use;

    if (gc->enabled != 0) {
        printf("# Warning: destroying gc while gc is disabled\n");
        gc->enabled = 0;
    }

    zt_gc_scan(gc, TRUE);

    zt_elist_for_each_safe(gc->white, elt, dont_use){
        zt_elist_remove(elt);
        gc->release_fn(gc, gc->private_data, (void **)&elt);
    }

    zt_elist_for_each_safe(gc->grey, elt, dont_use){
        zt_elist_remove(elt);
        gc->release_fn(gc, gc->private_data, (void **)&elt);
    }

    zt_elist_for_each_safe(gc->black, elt, dont_use){
        zt_elist_remove(elt);
        gc->release_fn(gc, gc->private_data, (void **)&elt);
    }

    /* clear the rootset, this should force us to clear all objects in
     * the system
     */
    /*
     * for(i = 0; i < gc->rootset_next; i++){
     *     zt_gc_collectable_t    * mark = (zt_gc_collectable_t *)gc->rootset[i];
     *     zt_elist_remove(&mark->list);
     *     /\* zt_elist_add(gc->white, &mark->list); *\/
     * }
     */

    XFREE(gc->rootset);
    gc->rootset = NULL;
    gc->rootset_next = 0;

    gc->enabled = 1;
}

/*
 * TODO: turn this into a wrapped list so that objects can be removed
 * from the root set.
 */
void
zt_gc_register_root(zt_gc_t *gc, void *v)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)v;

    zt_elist_reset(&mark->list);

    zt_gc_protect(gc, v);
    /*
     * gc->clear_white(mark);
     * zt_elist_add(gc->grey, &mark->list);
     */

    /*
     * gc->rootset[gc->rootset_next++] = &mark->list;
     * if (gc->rootset_next >= gc->rootset_size) {
     *     resize_rootset(gc, gc->rootset_size + 1024);
     * }
     */
}

void
zt_gc_prepare_value(zt_gc_t *gc, void *v)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)v;

    zt_elist_reset(&mark->list);
}

void
zt_gc_unregister_value(zt_gc_t *gc, void *v)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)v;

    zt_elist_remove(&mark->list);

}

void
zt_gc_register_value(zt_gc_t *gc, void *v)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)v;

    zt_elist_reset(&mark->list);
    gc->clear_white(mark);
    /* The mutator is registering interest in this value so we must
     * place it in the the grey list not the white.
     */

    if (++gc->current_allocs >= gc->allocs_before_scan) {
        zt_gc_scan(gc, 0);
    }
    /* rather then place the object in white and require the need for a
     * write barrier we place the object in grey with the expectation that
     * it is immediatly used.  This means that we may hold a newly allocated
     * object longer then necessary (one cycle).
     */
    zt_elist_add_tail(gc->grey, &mark->list);
}

void
zt_gc_free_white(zt_gc_t *gc)
{
    zt_elist            * elt = NULL;
    zt_elist            * dont_use = NULL;
    zt_gc_collectable_t * mark;

    if (zt_elist_empty(gc->white)) {
        return;
    }

    zt_elist_for_each_safe(gc->white, elt, dont_use) {
        /*
         * if(is_protected((zt_gc_collectable_t *)elt)) {
         *     zt_elist_remove(elt);
         *     zt_elist_add(gc->grey, elt);
         * } else {
         */
        if (elt == gc->black || elt == gc->white || elt == gc->grey) {
            /* zt_gc_print_heap(gc); */
            zt_assert(elt != gc->grey);
            zt_assert(elt != gc->white);
            zt_assert(elt != gc->black);
        }

        mark = zt_elist_data(elt, zt_gc_collectable_t, list);
        zt_assert(!is_protected(mark));
        /* printf("Releasing: %p\n", (void *)elt); */


        zt_elist_remove(elt);
        gc->release_fn(gc, gc->private_data, (void **)&elt);
        /* } */
    }
    /* printf("Done\n"); */

}

void
zt_gc_switch(zt_gc_t *gc)
{
    zt_elist * tmp_white;

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
zt_gc_scan(zt_gc_t *gc, int full_scan)
{
    int current_marks = 0;

    if (gc->enabled < 0 ||
        (!full_scan && gc->current_allocs < gc->allocs_before_scan)) {
        return;
    }

    gc->enabled--;
    /* printf("Start Scan\n"); */
    gc->current_allocs = 0;

    if (gc->scan == gc->grey) {
        /* beginning of a scan */
        /*
         * int      i;
         * for(i = 0; i < gc->rootset_next; i++) {
         *     /\* add each object in the rootset into the grey list *\/
         *     zt_gc_collectable_t    * mark    = (zt_gc_collectable_t *) gc->rootset[i];
         *     zt_elist_remove(&mark->list);
         *     zt_elist_add(gc->grey, &mark->list);
         * }
         */
        gc->scan = gc->scan->next;
    }

    while (gc->scan != gc->grey) {
        /* move thru each object in the grey list moving it to the
         * black list and marking it as we go.
         */
        zt_elist            * next;
        zt_elist            * elt;
        zt_gc_collectable_t * mark;

        elt = gc->scan;

        mark = zt_elist_data(elt, zt_gc_collectable_t, list);

        gc->clear_white(mark);
        gc->mark_fn(gc, gc->private_data, elt);

        next = gc->scan->next;

        if (!is_protected(mark)) {
            /* if it is protected leave it in the grey list */
            zt_elist_remove(elt);
            zt_elist_add(gc->black, elt);
        }

        gc->scan = next;

        if (!full_scan && ++current_marks >= gc->marks_per_scan) {
            if (gc->scan != gc->grey) {
                /*
                 * zt_gc_print_heap(gc);
                 * printf("End Partial Scan\n");
                 */
                gc->enabled++;
                return;
            }
        }
    }

    /*
     * fprintf(stderr, "Completed Scan\n");
     * zt_gc_print_heap(gc);
     * printf("End Full Scan\n");
     */
    gc->enabled++;
    zt_gc_switch(gc);
} /* zt_gc_scan */

int
zt_gc_mark_value(zt_gc_t *gc, void *value)
{
    zt_gc_collectable_t * mark = (zt_gc_collectable_t *)value;

    zt_assert(value != NULL);

    /*
     * if (is_protected(mark)) {
     *     /\* printf("marking protected\n"); *\/
     *     gc->clear_white(mark);
     *     zt_elist_remove(&mark->list);
     *     zt_elist_add_tail(gc->grey, &mark->list);
     *     return TRUE;
     * } else
     */
    if (!is_protected(mark) && gc->is_white(mark)) {
        gc->clear_white(mark);
        zt_elist_remove(&mark->list);
        zt_elist_add_tail(gc->grey, &mark->list);
        return(TRUE);
    }
    return(FALSE);
}

void
zt_gc_print_heap(zt_gc_t *gc)
{
    printf("Heap Dump\n============\n");
    printf("l1: %p l2: %p l3: %p\nblack: %p grey: %p white: %p\nscan: %p\n",
           (void *)&gc->list_1, (void *)&gc->list_2, (void *)&gc->list_3,
           (void *)gc->black, (void *)gc->grey, (void *)gc->white, (void *)gc->scan);


    dump_elist("Black", gc->black);
    dump_elist("Grey", gc->grey);
    dump_elist("White", gc->white);
}


void zt_gc_for_each(zt_gc_t * gc, void (*cb)(void * value, void * data, int ty), void * cb_data)
{
    zt_elist            * elt = NULL;
    zt_elist            * dont_use = NULL;
    zt_gc_collectable_t * mark;

    zt_elist_for_each_safe(gc->black, elt, dont_use){
        mark = zt_elist_data(elt, zt_gc_collectable_t, list);
        cb(mark, cb_data, 0);
    }

    zt_elist_for_each_safe(gc->grey, elt, dont_use){
        mark = zt_elist_data(elt, zt_gc_collectable_t, list);
        cb(mark, cb_data, 1);
    }

    zt_elist_for_each_safe(gc->white, elt, dont_use){
        mark = zt_elist_data(elt, zt_gc_collectable_t, list);
        cb(mark, cb_data, 2);
    }

}
