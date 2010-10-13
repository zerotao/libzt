/*
 * Copyright (C) 2001, 2004, 2005, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 *
 *
 * $Id: tree_test.c,v 1.1 2002/11/10 23:36:59 jshiffer Exp $
 *
 */

/*
 * Description: tree tests
 */
#include <string.h>

#include <libzt/zt_internal.h>
#include <libzt/adt/zt_tree.h>
#include <libzt/zt_unit.h>

#define REMOVED 8
#define REMOVED2 6
#define MAX_OD 10
int ordered_dataset[MAX_OD];

struct int_set {
    zt_rbt_node node;
    int         i;
};

int int_compare( zt_rbt_node *x, zt_rbt_node  *x2)
{
    struct int_set * n1 = zt_rbt_data(x, struct int_set, node);
    struct int_set * n2 = zt_rbt_data(x2, struct int_set, node);

    if (n1->i < n2->i) {
        return -1;
    } else if (n1->i == n2->i) {
        return 0;
    } else {
        return 1;
    }
}



#define zt_rbt_node_init(x) do { \
        zt_rbt_right(x) = NULL;  \
        zt_rbt_left(x) = NULL;   \
        zt_rbt_parent(x) = NULL; \
} while (0)

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    zt_rbt         * br_root = NULL;
    zt_rbt_node    * iter;
    zt_rbt_node    * next;
    struct int_set * node;
    struct int_set   rem1;

    int              i;

    /* key(), key_size, data_size, compare_func (ala qsort) */
    /* br_root = tree_blackred( NULL, sizeof(int), 0, int_compare );  */

    /* insert the information randomly */
    for (i = 0; i < MAX_OD; i++) {
        long n = 0;
        int  done = 0;

        node = XCALLOC(struct int_set, 1);
        zt_rbt_node_init(&(node->node));
        /* make sure that we only insert an individual data
         * item only once.
         */
        do {
            n = random() % MAX_OD;
            if (ordered_dataset[n] == 0) {
                ordered_dataset[n] = 1;
                done = 1;
            }
        } while (!done);
        node->i = n;
        zt_rbt_insert(&br_root, &(node->node), int_compare);
    }

    for (i = 0; i < MAX_OD; i++) {
        ZT_UNIT_ASSERT(test, ordered_dataset[i] == 1);
    }

    rem1.i = REMOVED;
    iter = zt_rbt_find(&br_root, &rem1.node, int_compare);
    if (iter) {
        struct int_set *n = zt_rbt_data(iter, struct int_set, node);
        /* struct int_set *n2; */
        iter = zt_rbt_remove(&br_root, iter);

        ZT_UNIT_ASSERT(test, n->i == REMOVED);

        memset(n, 0, sizeof(struct int_set));
        XFREE(n);
    }

    rem1.i = REMOVED2;
    iter = zt_rbt_find(&br_root, &rem1.node, int_compare);
    if (iter) {
        struct int_set *n = zt_rbt_data(iter, struct int_set, node);
        /* struct int_set *n2; */
        iter = zt_rbt_remove(&br_root, iter);

        ZT_UNIT_ASSERT(test, n->i == REMOVED2);

        iter = zt_rbt_insert(&br_root, iter, int_compare);
        ZT_UNIT_ASSERT(test, iter == NULL);
    }


    /* now iterate over each item in order clearing them as we go. */
    zt_rbt_for_each_safe(&br_root, iter, next) {
        struct int_set * x = zt_rbt_data(iter, struct int_set, node);

        ordered_dataset[x->i] = 0;
        iter = zt_rbt_remove(&br_root, iter);
        XFREE(x);
    }


    for (i = 0; i < MAX_OD; i++) {
        ZT_UNIT_ASSERT(test, ordered_dataset[i] == 0 || i == REMOVED);
    }
} /* basic_tests */

int
register_tree_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "tree tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
