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

#include <zt_progname.h>
#include <zt_cstr.h>
#include <zt_unit.h>

static char * argv[] = { "test_app", NULL };

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    char    * name = NULL;
    char    * path = NULL;
    char    * npath = NULL;
    char      nname[PATH_MAX+1];
    char      cwd[PATH_MAX+1];

    name = zt_progname(argv[0], 0);
    ZT_UNIT_ASSERT(test, (!strcmp(name, argv[0])));

    name = zt_progname(NULL, 0);
    ZT_UNIT_ASSERT(test, (!strcmp(name, argv[0])));

    name = zt_progname("foo", 0);
    ZT_UNIT_ASSERT(test, (!strcmp(name, "foo")));

    /* test the progpath */
    getcwd(cwd, PATH_MAX);

    path = zt_progpath(NULL);
    ZT_UNIT_ASSERT(test, (!strcmp(path, "*UNKNOWN*")));

    /* us the path that is already available */
    path = zt_progpath(argv[0]);
    ZT_UNIT_ASSERT(test, strcmp(path, cwd) == 0);

    /* return whatever is already set */
    path = zt_progpath(NULL);
    ZT_UNIT_ASSERT(test, strcmp(path, cwd) == 0);

    /* use the PATH env var to find the appname */
    npath = zt_cstr_catv(getenv("PATH"), 0, -1,
                         ENV_SEPERATOR, 0, -1,
                         getenv("PWD"), 0, -1, NULL);

    setenv("PATH", npath, 1);
    name = zt_cstr_basename(nname, PATH_MAX, argv[0], NULL);
    path = zt_progpath(name);
    ZT_UNIT_ASSERT(test, strcmp(path, getenv("PWD")) == 0);

    /* empty or null (after set) is the old path */
    path = zt_progpath("");
    ZT_UNIT_ASSERT(test, strcmp(path, getenv("PWD")) == 0);

}

int
register_progname_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "progname tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
