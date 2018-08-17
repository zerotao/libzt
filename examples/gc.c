#include <stdlib.h>
#define ZT_WITH_GC
#include <zt.h>

#undef DEBUG_GC

/*
 * this demo covers the following:
 *
 * 1. setting up and using the GCs primary interfaces:
 *   - zt_gc_init
 *   - zt_gc_destroy
 *   - zt_gc_register / unregister_value
 *   - zt_gc_enable / disable
 *   - zt_gc_protect / unprotect
 */

/*
 * I am going to use a vtable setup for this although if there are a limited number of GC-able values
 * they could be hard coded into mark/release with some type or flags variable.
 */
typedef struct value value_t;
typedef struct value_vtbl value_vtbl_t;
struct value_vtbl {
    size_t  size;
    void    (*mark)(zt_gc_t * gc, value_t * value);
    void    (*release)(zt_gc_t * gc, value_t * value);
};

struct value {
    zt_gc_collectable_t       mark; /* must be the first value in the structure
                                     * OR must be passed in to the GC calls directly*/
    value_vtbl_t            * vtbl;
};

static void release_value(zt_gc_t * gc, void * pdata UNUSED, void **value);
static void mark_value(zt_gc_t * gc, void * pdata UNUSED, void * value);

static value_t *
alloc_value(zt_gc_t * gc, value_vtbl_t * vtbl) {
    value_t     * value;
    if (vtbl == NULL) {
        return NULL;
    }
    value = zt_callocs(vtbl->size, 1);
    value->vtbl = vtbl;

    zt_gc_prepare(gc, (void *)value); /*  */
    printf("Allocating: %p\n", (void *)value);
    return value;
}

/*
 * Definition of our test list
 */
typedef struct list list_t;
struct list {
    value_t       base;

    value_t     * tail;
    value_t     * value;
};

static void
mark_list(zt_gc_t * gc, value_t * value) {
    list_t  * list = (list_t *)value;

    /*  we want to mark any child "objects" that are managed
     *  by the GC
     */
    if (list->value) {
        mark_value(gc, NULL, list->value);
    }

    if (list->tail) {
        mark_value(gc, NULL, list->tail);
    }
}
static void
release_list(zt_gc_t * gc, value_t * value) {
    printf("Releasing List: %p\n", (void *)value);
}
value_vtbl_t list_vtbl = {
    sizeof(list_t),
    mark_list,
    release_list,  /*  we only free non GCable data so no need for a release callback */
};

/*
 * definition of our test atom
 */
#define ATOM_PTR 0
#define ATOM_INT 1
typedef struct atom atom_t;
struct atom {
    value_t       base;

    int           type;
    union {
        void    * ptr;
        int       integer;
    }value;
};

static void
release_atom(zt_gc_t * gc, value_t * value) {
    atom_t  * atom = (atom_t *)value;

    printf("Releasing Atom: %d\n", atom->value.integer);
    if(atom->type == ATOM_PTR && atom->value.ptr) {
        /* this makes the assumption that any ptr is allocated
         * It is only here as an example.
         */
        free(atom->value.ptr);
        atom->value.ptr = NULL;
    }
}

value_vtbl_t atom_vtbl = {
    sizeof(atom_t),
    NULL,
    release_atom,
};

/*
 * This is our opportunity to tell the GC about any child values
 * to the current value (which would happen in the vtable for that value)
 */
static void
mark_value(zt_gc_t * gc, void * pdata UNUSED, void * value) {
    if (value) {
        value_t * vval = (value_t *)value;

        if (vval && vval->vtbl && vval->vtbl->mark != NULL) {
            vval->vtbl->mark(gc, vval);
        }

        zt_gc_mark(gc, value);
    }
}

/*
 * when we are going to release we need to call the vtable release function
 * if available otherwise we just free the object and set it to null
 */
static void
release_value(zt_gc_t * gc, void * pdata UNUSED, void **value) {
    if(value && *value) {
        value_t * vval = *(value_t **)value;
        if ((vval)->vtbl && (vval)->vtbl->release) {
            (vval)->vtbl->release(gc, *value);
        }

        zt_gc_unregister(gc, *value);
        free(*value);
        *value = NULL;
    }
}

int
main(int argc, char *argv[]) {
    zt_gc_t * gc;
    list_t  * root1;
    list_t  * root2;


/* these are set really low just so we can see the GC happen */
# define ALLOCS_BEFORE_GC_SCAN 1 /* how many allocations before we do an incremental scan */
# define MARKS_PER_GC_SCAN 100    /* how many nodes to mark whenever we do an incremental scan */
    /*
     *  you can pass callback data to the mark/release functions if it is useful (e.g. you are allocating from
     *  a custom heap)
     */
    gc = zt_gc_init(NULL, mark_value, release_value, MARKS_PER_GC_SCAN, ALLOCS_BEFORE_GC_SCAN);

    printf("allocate the root lists\n");
    root1 = (list_t *)alloc_value(gc, &list_vtbl);
    root2 = (list_t *)alloc_value(gc, &list_vtbl);

    /*  protected values are be scanned but never released until
     *  unprotected OR the gc is destroyed; they must have already
     *  been prepare (zt_gc_prepare) see: alloc_value
     */
    zt_gc_protect(gc, root1);
    zt_gc_protect(gc, root2);

    printf("setting up the list\n");

    list_t  * prev = NULL;
    for(int i = 0; i < 20; i++) {
        list_t  * curr = (list_t *)alloc_value(gc, &list_vtbl);
        atom_t  * atom = (atom_t *)alloc_value(gc, &atom_vtbl);

        atom->value.integer = i;
        atom->type = ATOM_INT;

        curr->value = (value_t *)atom;
        if (prev == NULL) {
            root1->tail = (value_t *)curr;
        } else {
            prev->tail = (value_t *)curr;
        }

        zt_gc_register(gc, curr);
        zt_gc_register(gc, atom);

        prev = curr;
    }
    root2->tail = root1->tail;

    printf("root1: %p\n", (void *)root1);
    printf("root2: %p\n", (void *)root2);

    printf("Full gc (should not see any releases)...");
    /* so many just to show nothing getting released when it should not */
    zt_gc_collect(gc, true);
    zt_gc_collect(gc, true);
    zt_gc_collect(gc, true);
    zt_gc_collect(gc, true);
    zt_gc_collect(gc, true);
    zt_gc_collect(gc, true);
    printf("complete\n");

    zt_gc_unprotect(gc, root2);

    printf("Full gc (should only see root2 be released)...\n");
    /* takes 2 gc cycles because we have not registered anything new... */
    zt_gc_collect(gc, true);
    zt_gc_collect(gc, true);
    printf("complete\n");

    /* zt_gc_unprotect(gc, root1); */

    /* free everything */
    printf("Release the world\n");
    zt_gc_destroy(gc);

    return 0;
}
