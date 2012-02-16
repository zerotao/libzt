/*
 * path_test.c          path utils tests
 *
 * Copyright (C) 2010-2011 Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See File COPYING for details.
 */

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif /* HAVE_SYS_STAT_H */

#include <string.h>

#define ZT_WITH_UNIT
#include <zt.h>

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    struct stat       stat_buf;

    ZT_UNIT_ASSERT(test, zt_path_exists("foo/bar/baz") == -1);
    ZT_UNIT_ASSERT(test, zt_mkdir("foo/bar/baz", 0777, zt_mkdir_create_parent) == 0);
    ZT_UNIT_ASSERT(test, stat("foo/bar/baz", &stat_buf) == 0);
    ZT_UNIT_ASSERT(test, zt_path_exists("foo/bar/baz") == 0);
    ZT_UNIT_ASSERT(test, zt_mkdir("foo/bar/baz", 0777, zt_mkdir_create_parent) == 0);

    ZT_UNIT_ASSERT(test, strcmp(zt_find_basedir("foo/bar/baz/spin", "baz"), "foo/bar/") == 0);
    ZT_UNIT_ASSERT(test, zt_find_basedir("foo/bar/baz/spin", ".baz") == NULL);

    rmdir("foo/bar/baz");
    rmdir("foo/bar");
    rmdir("foo");
}

int
register_path_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "path tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
