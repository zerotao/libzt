/*
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 *
 */

/*
 * Description:
 */
#include <stdlib.h>
#include <stdio.h>

#include <adt/zt_list.h>
#include <zt_unit.h>

static int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
#define VALUES_MAX sizeof_array(values) - 1

typedef struct list_elt list_elt;
struct list_elt {
    zt_elist_t list;
    int      value;
};


static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    list_elt * al;
    list_elt * al2;

    zt_elist_t * volatile tmp;
    zt_elist_t * tmp2;

    size_t       i;

    zt_elist(list1);
    zt_elist(list2);

    ZT_UNIT_ASSERT(test, list1.prev == &list1);
    ZT_UNIT_ASSERT(test, list1.next == &list1);

    for (i = 0; i < sizeof_array(values); i++) {
        al = XCALLOC(list_elt, 1);
        al->value = values[i];

        zt_elist_add(&list1, &al->list);

        al = XCALLOC(list_elt, 1);
        al->value = (int)(VALUES_MAX - values[i]);

        zt_elist_add(&list2, &al->list);
    }

    tmp2 = &list2;
    zt_elist_for_each(&list1, tmp) {
        tmp2 = zt_elist_get_next(tmp2);

        al = zt_elist_data(tmp, list_elt, list);
        al2 = zt_elist_data(tmp2, list_elt, list);

        ZT_UNIT_ASSERT(test, al->value + al2->value == 9);

    }

    zt_elist_for_each_safe(&list1, tmp, tmp2) {
        al = zt_elist_data(tmp, list_elt, list);
        XFREE(al);
    }

    zt_elist_for_each_safe(&list2, tmp, tmp2) {
        al = zt_elist_data(tmp, list_elt, list);
        XFREE(al);
    }
}

int
register_list_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "list tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
