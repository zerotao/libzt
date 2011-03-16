/*
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 *
 */

/*
 * Description: tests for the options interface
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#include <zt_internal.h>
#include <zt_opts.h>
#include <zt_unit.h>

int   integer = 0;
long  long_integer = 0;
char *str = 0;
int   boolean = 0;
int   flag = 0;

int
func(char *arg UNUSED, void * data UNUSED)
{
    printf("func\n");
    return EXIT_SUCCESS;
}

int
ofunc(char *arg, void * data UNUSED)
{
    printf("ofunc %s\n", arg);
    return EXIT_SUCCESS;
}

int
rfunc(char * arg, void * data UNUSED)
{
    printf("rfunc %s\n", arg);
    return EXIT_SUCCESS;
}

#ifdef HAVE_GETOPT_LONG
static char *s_argv[] = { "unit_test",
                          "--int", "1",
                          "--long", "1",
                          "--bool=t",
                          "--string", "hello",
                          "--flag",
                          "test_command",
                          "--int", "2",
                          NULL };
#else
static char *s_argv[] = { "unit_test",
                          "-i", "1",
                          "-l", "1",
                          "-b", "t",
                          "test_command",
                          "-i", "2",
                          NULL };
#endif

static void
basic_opts_tests(struct zt_unit_test *test, void *data UNUSED) {
    int    argc = sizeof_array(s_argv) - 1;             /* -1 for NULL */
    char **argv;
    char **pargv;
    int    i;

#define TEST_LONG_STRING "This is a test of a very long string\n\t\tIt is intended to wrap around a\n\t\tcouple of times."
    struct zt_opt_args options[] = {
        { 'h',        "help",   "This help text",    zt_opt_help,   NULL,          NULL,  NULL                 },
        { 'l',        "long",   "long integer test", zt_opt_long,   &long_integer, NULL,  NULL                 },
        { 'i',        "int",    "integer test",      zt_opt_int,    &integer,      NULL,  NULL                 },
        { 'b',        "bool",   "boolean test",      zt_opt_bool,   &boolean,      NULL,  NULL                 },
#ifdef HAVE_GETOPT_LONG
        { ZT_OPT_NSO, "string", TEST_LONG_STRING,    zt_opt_string, &str,          NULL,  "-s \"Some String\"" },
#endif /* HAVE_GETOPT_LONG */
        { 'f',        "func",   "func test",         zt_opt_func,   NULL,          func,  NULL                 },
        { 'o',        "ofunc",  "ofunc test",        zt_opt_ofunc,  NULL,          ofunc, NULL                 },
        { 'r',        "rfunc",  "rfunc test",        zt_opt_rfunc,  NULL,          rfunc, NULL                 },
#ifdef HAVE_GETOPT_LONG
        { ZT_OPT_NSO, "flag",   "flag test",         zt_opt_flag,   &flag,         NULL,  NULL                 },
#endif /* HAVE_GETOPT_LONG */
        { 0,          0,        0,                   0,             0,             0,     0                    }
    };

    struct zt_opt_args options2[] = {
        { 'h', "help", "This help text", zt_opt_help, NULL,     NULL, NULL },
        { 'i', "int",  "integer test",   zt_opt_int,  &integer, NULL, NULL },
        { 0,   0,      0,                0,           0,        0,    0    }
    };

    argv = zt_calloc(char *, argc + 1);
    for (i = 0; i < argc; i++) {
        argv[i] = s_argv[i];
    }

    pargv = argv;

    zt_opts_process(&argc, &pargv, options, "[options]", TRUE, TRUE, NULL);
    ZT_UNIT_ASSERT(test, integer == 1);
    ZT_UNIT_ASSERT(test, long_integer == 1);
    ZT_UNIT_ASSERT(test, boolean == 1);

#ifdef HAVE_GETOPT_LONG
    ZT_UNIT_ASSERT(test, str != 0 && strcmp(str, "hello") == 0);
    ZT_UNIT_ASSERT(test, flag == 1);
#endif

    ZT_UNIT_ASSERT(test, argc == 3);
    ZT_UNIT_ASSERT(test, pargv[0] != NULL);
    ZT_UNIT_ASSERT(test, strcmp(pargv[0], "test_command") == 0);

    zt_opts_process(&argc, &pargv, options2, "[options]", TRUE, TRUE, NULL);
    ZT_UNIT_ASSERT(test, integer == 2);
    ZT_UNIT_ASSERT(test, argc == 0);

    zt_free(argv);

    if (str) {
        zt_free(str);
    }
} /* basic_opts_tests */


int
register_opts_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "option parsing tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_opts_tests);
    return 0;
}
