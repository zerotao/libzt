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

#include <libzt/zt_internal.h>
#include <libzt/zt_opts.h>
#include <libzt/zt_unit.h>

int   integer = 0;
char *str = 0;
int   boolean = 0;
int   flag = 0;

int
func(char *arg, void * data)
{
    printf("func\n");
    return EXIT_SUCCESS;
}

int
ofunc(char *arg, void * data)
{
    printf("ofunc %s\n", arg);
    return EXIT_SUCCESS;
}

int
rfunc(char * arg, void * data)
{
    printf("rfunc %s\n", arg);
    return EXIT_SUCCESS;
}

static char *s_argv[] = { "unit_test",
                          "--int",
                          "1",
                          "--bool=t",
                          "--string",
                          "hello",
                          "--flag",
                          "test_command",
                          "--int",
                          "2",
                          NULL };

static void
basic_opts_tests(struct zt_unit_test *test, void *data)
{
    int                argc = 10;
    char             **argv;
    char             **pargv;
    int                i;

    struct zt_opt_args options[] = {
        { 'h',        "help",   "This help text",                                                                                  zt_opt_help,   NULL,     NULL,  NULL                 },
        { 'i',        "int",    "integer test",                                                                                    zt_opt_int,    &integer, NULL,  NULL                 },
        { 'b',        "bool",   "boolean test",                                                                                    zt_opt_bool,   &boolean,    NULL,  NULL                 },
        { ZT_OPT_NSO, "string", "This is a test of a very long string\n\t\tIt is intended to wrap around a\n\t\tcouple of times.", zt_opt_string, &str,     NULL,  "-s \"Some String\"" },
        { 'f',        "func",   "func test",                                                                                       zt_opt_func,   NULL,     func,  NULL                 },
        { 'o',        "ofunc",  "ofunc test",                                                                                      zt_opt_ofunc,  NULL,     ofunc, NULL                 },
        { 'r',        "rfunc",  "rfunc test",                                                                                      zt_opt_rfunc,  NULL,     rfunc, NULL                 },
        { ZT_OPT_NSO, "flag",   "flag test",                                                                                       zt_opt_flag,   &flag,    NULL,  NULL                 },
        { 0,          0,        0,                                                                                                 0,             0,        0,     0                    }
    };

    struct zt_opt_args options2[] = {
        { 'h', "help", "This help text", zt_opt_help, NULL,     NULL, NULL },
        { 'i', "int",  "integer test",   zt_opt_int,  &integer, NULL, NULL },
        { 0,   0,      0,                0,           0,        0,    0    }
    };

    argv = XCALLOC(char *, argc + 1);
    for (i = 0; i < argc; i++) {
        argv[i] = s_argv[i];
    }

    pargv = argv;

    zt_opts_process(&argc, &pargv, options, "[options]", TRUE, TRUE, NULL);
    ZT_UNIT_ASSERT(test, integer == 1);
    ZT_UNIT_ASSERT(test, boolean == 1);
    ZT_UNIT_ASSERT(test, str != 0 && strcmp(str, "hello") == 0);
    ZT_UNIT_ASSERT(test, flag == 1);

    ZT_UNIT_ASSERT(test, argc == 3);
    ZT_UNIT_ASSERT(test, pargv[0] != NULL);
    ZT_UNIT_ASSERT(test, strcmp(pargv[0], "test_command") == 0);

    zt_opts_process(&argc, &pargv, options2, "[options]", TRUE, TRUE, NULL);
    ZT_UNIT_ASSERT(test, integer == 2);
    ZT_UNIT_ASSERT(test, argc == 0);

    XFREE(argv);

    if (str) {
        XFREE(str);
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
