#define ZT_WITH_UNIT
#define ZT_WITH_GC
#include <zt.h>

#define DEBUG_GC 0

#define INT  1
#define ATOM 2

typedef struct atom atom;
struct atom {
    zt_gc_collectable_t mark;
    int                 type;
    union {
        struct atom * atom;
        int           number;
    } value;
};

static int ints_marked = 0;
static int atoms_marked = 0;
static int objects_freed = 0;

static void mark_atom(zt_gc_t *gc, void *pdata UNUSED, void *v)
{
    atom *a = (atom *)v;

    if (a->type == INT) {
        ints_marked++;
    } else {
        atoms_marked++;
        if (a->value.atom != NULL) {
            zt_gc_mark(gc, a->value.atom);
        }
    }
    return;
}

static void
release_atom(zt_gc_t *gc UNUSED, void *pdata UNUSED, void **v)
{
    atom **a = (atom **)v;

    objects_freed++;
    free(*a);
    *a = NULL;
}

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    zt_gc_t * gc;
    atom    * root;
    int       i;
    atom    * protected;

#define RSTART 1
#define REND 10
#define MARKS_PER_SCAN 10
#define ALLOCS_BEFORE_SCAN 1

    gc = zt_gc_init(NULL, mark_atom, release_atom, MARKS_PER_SCAN, ALLOCS_BEFORE_SCAN);

    root = zt_calloc(atom, 1);
    protected = zt_calloc(atom, 1);
    zt_gc_prepare(gc, root);
    root->type = ATOM;
    root->value.atom = NULL;
    zt_gc_protect(gc, root);


    /* allocate REND atoms the first is assigned to the root object
     * the second is treated as a protected object
     * the rest are free floating and will thus get reaped quickly
     */
    for (i = RSTART; i <= REND; i++) {
        atom * a = zt_calloc(atom, 1);
        zt_gc_prepare(gc, a);

        /* you can register a value immediatly but it must be setup
         * and reachable thru a root object (or protected directly) before you
         * register another otherwise it can be reaped.
         */
        zt_gc_register(gc, a);

        if (i == RSTART ) {
            a->type = INT;
            a->value.number = i;
            root->value.atom = a; /* assign the first one to the root object so it can be "found" */
        } else if (i == RSTART + 1) {
            a->type = INT;
            a->value.number = i;
            zt_gc_protect(gc, a); /* protect the second one so that it is found directly */
            protected = a;        /* so we can reference it later */
        } else {
            a->type = INT;
            a->value.number = i;
            /* let the rest be reapable */
        }
    }

    /* zt_gc_print_heap(gc); */

    {
        /* we have registered REND + 1 objects: 1 root and REND numbers
         * after every registration we are performing a full gc (because
         * there are fewer objects then ALLOCS_BEFORE_SCAN) so we will have
         * marked every object once (when they are first registered) and
         * will have marked the root, root->value.a and protected objects
         * once per allocation.
         */
        int atoms_mark_base  = (REND * 2);
        int ints_marked_base = (REND * 2) - 3;
        int objects_freed_base = (REND - 3);     /* root->value.a, protected, last object added */

#if DEBUG_GC
        printf("Base  ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked_base, atoms_mark_base, objects_freed_base);
        printf("First ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked, atoms_marked, objects_freed);
#endif
        ZT_UNIT_ASSERT(test, ints_marked == ints_marked_base);
        ZT_UNIT_ASSERT(test, atoms_marked == atoms_mark_base);
        ZT_UNIT_ASSERT(test, objects_freed == objects_freed_base);

        /* second pass */
        zt_gc_collect(gc, true);

        ints_marked_base += 2;                                /* root->value.a + protected + last_alloc */
        atoms_mark_base += 1;                                 /* root->value.a */
        objects_freed_base += 1;
#if DEBUG_GC
        printf("Base  ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked_base, atoms_mark_base, objects_freed_base);
        printf("Second ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked, atoms_marked, objects_freed);
#endif
        ZT_UNIT_ASSERT(test, ints_marked == ints_marked_base);
        ZT_UNIT_ASSERT(test, atoms_marked == atoms_mark_base);
        ZT_UNIT_ASSERT(test, objects_freed == objects_freed_base);

        /* root->value.atom will now be released */
        root->value.atom = NULL;

        /* third pass */
        zt_gc_collect(gc, 1);

        ints_marked_base += 1;                                /* protected */
        atoms_mark_base += 1;                                 /* root */
        objects_freed_base += 1;                              /* root->value.a */
#if DEBUG_GC
        printf("Base  ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked_base, atoms_mark_base, objects_freed_base);
        printf("Third ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked, atoms_marked, objects_freed);
#endif
        ZT_UNIT_ASSERT(test, ints_marked == ints_marked_base);
        ZT_UNIT_ASSERT(test, atoms_marked == atoms_mark_base);
        ZT_UNIT_ASSERT(test, objects_freed == objects_freed_base);

        zt_gc_unprotect(gc, protected); /* unprotected values will take 2 passes to be released */

        /* fourth  pass */
        zt_gc_collect(gc, 1);

        ints_marked_base += 1;                                /* protected */
        atoms_mark_base += 1;                                 /* root */
        objects_freed_base += 0;                              /* protected */
#if DEBUG_GC
        printf("Base  ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked_base, atoms_mark_base, objects_freed_base);
        printf("Fourth ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked, atoms_marked, objects_freed);
#endif
        ZT_UNIT_ASSERT(test, ints_marked == ints_marked_base);
        ZT_UNIT_ASSERT(test, atoms_marked == atoms_mark_base);
        ZT_UNIT_ASSERT(test, objects_freed == objects_freed_base);

        /* fifth  pass */
        zt_gc_collect(gc, 1);

        ints_marked_base += 0;                                /* protected */
        atoms_mark_base += 1;                                 /* root */
        objects_freed_base += 1;                              /* protected */
#if DEBUG_GC
        printf("Base  ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked_base, atoms_mark_base, objects_freed_base);
        printf("Fifth ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked, atoms_marked, objects_freed);
#endif
        ZT_UNIT_ASSERT(test, ints_marked == ints_marked_base);
        ZT_UNIT_ASSERT(test, atoms_marked == atoms_mark_base);
        ZT_UNIT_ASSERT(test, objects_freed == objects_freed_base);

        /* final */
        zt_gc_destroy(gc);

        ints_marked_base += 0;                                   /* */
        atoms_mark_base += 1;                                    /* root */
        objects_freed_base += 1;                                 /* root */
#if DEBUG_GC
        printf("Base  ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked_base, atoms_mark_base, objects_freed_base);
        printf("Final ints_marked: %d atoms_marked: %d objects_freed: %d\n", ints_marked, atoms_marked, objects_freed);
#endif
        ZT_UNIT_ASSERT(test, ints_marked == ints_marked_base);
        ZT_UNIT_ASSERT(test, atoms_marked == atoms_mark_base);
        ZT_UNIT_ASSERT(test, objects_freed == objects_freed_base);
    }
} /* basic_tests */

int
register_gc_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "gc tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
