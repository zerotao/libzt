/*
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: macros_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description:
 */

#include <libzt/zt.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    /* test ABS */
    ZT_UNIT_ASSERT(test, (ABS(-1) == 1));

    ZT_UNIT_ASSERT(test, (CLAMP(10, 2, 5) == 5));
    ZT_UNIT_ASSERT(test, (CLAMP(1, 2, 5) == 2));

    ZT_UNIT_ASSERT(test, (MAX(5, 10) == 10));
    ZT_UNIT_ASSERT(test, (MAX(10, 5) == 10));

    ZT_UNIT_ASSERT(test, (MIN(5, 10) == 5));
    ZT_UNIT_ASSERT(test, (MIN(10, 5) == 5));

    {
        char size[10];
        ZT_UNIT_ASSERT(test, (sizeof_array(size) == 10));
        ZT_UNIT_ASSERT(test, (endof_array(size) == &size[10]));
    }
}

int
register_macros_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "macro tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return(0);
}
