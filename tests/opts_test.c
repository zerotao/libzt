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
# include "zt_config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#define ZT_WITH_UNIT
#include <zt.h>
#include <zt_opts.h>

long   long_integer  = 0;
long   long_integer2 = 0;
char * str           = 0;
int    bool_type     = 0;
int    flag          = 0;
int    flag2         = 0;
int    local_data    = 0;

static char * s_argv[] = {
    "unit_test",
    "--long",      "1",
    "--bool=t",
    "--flag",
    "--string",    "hello",
    "-qqq",
    "test_command",
    "-l2",
    "-L=4",
    "-b",          "f",
    NULL
};

static int
local_func(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    return 0; /* return # of args consumed */
}

int
local_error(int code, char * fmt, ...) {
    /* we could capture any errors here
     * instead we will fail explicitly if
     * we get any.
     */
    int     ret;
    va_list ap;

    va_start(ap, fmt);
    ret = zt_opt_verror_default(code, fmt, ap);
    va_end(ap);

    if(ret != 0) {
        exit(ret);
    }

    return 0;
}

#define TEST_LONG_STRING "This is a really long string intended to overflow the screen and make things look all wack"
static void
basic_opts_tests(struct zt_unit_test * test, void * data UNUSED) {
    int               argc = sizeof_array(s_argv) - 1;  /* -1 for NULL */
    int               nargc = argc;
    char           ** pargv;
    int               ret;
    char            * err;

    struct zt_opt_def options[] = {
        { 'h',        "help",     NULL, zt_opt_help_stdout, "[options]",    "This help text"     },
        { 'b',        "bool",     NULL, zt_opt_bool_int,    &bool_type,     "boolean_test"       },
        { ZT_OPT_NSO, "string",   NULL, zt_opt_string,      &str,           TEST_LONG_STRING     },
        { 'f',        "func",     NULL, local_func,         &local_data,    "generic func test"  },
        { 'l',        "long",     NULL, zt_opt_long,        &long_integer,  "long integer test"  },
        { 'L',        "long2",    NULL, zt_opt_long,        &long_integer2, "long integer2 test" },
        { ZT_OPT_NSO, "flag",     NULL, zt_opt_flag_int,    &flag,          "flag test"          },
        { 'q',        ZT_OPT_NLO, NULL, zt_opt_flag_int,    &flag2,         "flag2 test"         },
        { ZT_OPT_END() }
    };

    pargv = s_argv;


    err = zt_opt_error_str(22, "test");

    /* printf("%s\n", err); */
    ZT_UNIT_ASSERT(test, strcmp("error: { code: 22, string: \"Invalid argument: test\" }", err) == 0);

    ret = zt_opt_process_args(&nargc, pargv, options, NULL, local_error);

    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, nargc < argc); /* stopped on "test_command" */

    ZT_UNIT_ASSERT(test, nargc == 8);
    ZT_UNIT_ASSERT(test, pargv[nargc] != NULL);
    ZT_UNIT_ASSERT(test, strcmp(pargv[nargc], "test_command") == 0);

    ZT_UNIT_ASSERT(test, long_integer == 1);
    ZT_UNIT_ASSERT(test, bool_type == 1);
    ZT_UNIT_ASSERT(test, str != 0 && strcmp(str, "hello") == 0);
    ZT_UNIT_ASSERT(test, flag == 1);

    ZT_UNIT_ASSERT(test, flag2 == 3);

    /* reset for next argv */
    pargv = &pargv[nargc];
    nargc = argc - nargc;

    ZT_UNIT_ASSERT(test, nargc == 5);

    ret = zt_opt_process_args(&nargc, pargv, options, NULL, NULL);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, nargc == 5);
    ZT_UNIT_ASSERT(test, long_integer == 2);
    ZT_UNIT_ASSERT(test, long_integer2 == 4);
    ZT_UNIT_ASSERT(test, bool_type == 0);
    ZT_UNIT_ASSERT(test, str != 0 && strcmp(str, "hello") == 0);
    ZT_UNIT_ASSERT(test, flag == 1);

    if (str) {
        zt_free(str);
    }
} /* basic_opts_tests */

int
register_opts_suite(struct zt_unit * unit) {
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "option parsing tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_opts_tests);
    return 0;
}

