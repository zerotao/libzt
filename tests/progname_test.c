/*
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: progname_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description:
 */

#include <string.h>

#include <libzt/zt_progname.h>
#include <libzt/zt_unit.h>

static char * argv[] = { "test_app", NULL };

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    char *name = NULL;

    name = zt_progname(argv[0], 0);
    ZT_UNIT_ASSERT(test, (!strcmp(name, argv[0])));

    name = zt_progname(NULL, 0);
    ZT_UNIT_ASSERT(test, (!strcmp(name, argv[0])));

    name = zt_progname("foo", 0);
    ZT_UNIT_ASSERT(test, (!strcmp(name, "foo")));
}

int
register_progname_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "progname tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
