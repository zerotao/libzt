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

#include <libzt/zt.h>
#include <libzt/zt_opts.h>
#include <libzt/zt_unit.h>

int integer = 0;
char *str = 0;
int bool = 0;
int flag = 0;

int
func (char *arg, void * data)
{
	printf("func\n");
	return EXIT_SUCCESS;
}

int
ofunc (char *arg, void * data)
{
	printf("ofunc %s\n", arg);
	return EXIT_SUCCESS;	
}

int
rfunc (char * arg, void * data)
{
	int i = 0;
	printf("rfunc %s\n", arg);
	return EXIT_SUCCESS;	
}

static char *s_argv[] = {"unit_test",
						 "--int",
						 "1",
						 "--bool=t",
						 "--string",
						 "hello",
						 "--flag",
						 NULL};
	
static void
basic_opts_tests(struct zt_unit_test *test, void *data)
{
	int				  offt;
	int				  argc = 7;
	char			**argv;
	char			**pargv;
	int				  i;
	
	struct opt_args   options[] =
		{
			{ 'h', "help", "This help text", opt_help, NULL, NULL, NULL},
			{ '[', "int", "integer test", opt_int, &integer, NULL, NULL},
			{ 'b', "bool", "boolean test", opt_bool, &bool, NULL, NULL},
			{  OPT_NSO, "string", "This is a test of a very long string\n\t\tIt is intended to wrap around a\n\t\tcouple of times.", opt_string, &str, NULL, "-s \"Some String\""},
			{ 'f', "func", "func test", opt_func, NULL, func, NULL},
			{ 'o', "ofunc", "ofunc test", opt_ofunc, NULL, ofunc, NULL},
			{ 'r', "rfunc", "rfunc test", opt_rfunc, NULL, rfunc, NULL},
			{  OPT_NSO, "flag", "flag test", opt_flag, &flag, NULL, NULL},
			{ 0,0,0,0 }
		};

	argv = XCALLOC(char *, argc+1);
	for(i=0; i < argc; i++){
		argv[i] = s_argv[i];
	}
	
	pargv = argv;
	opts_process(&argc, &pargv, options, "[options]", TRUE, TRUE, NULL);
	ZT_UNIT_ASSERT(test, integer == 1);
	ZT_UNIT_ASSERT(test, bool == 1);
	ZT_UNIT_ASSERT(test, str != 0 && strcmp(str, "hello") == 0);
	ZT_UNIT_ASSERT(test, flag == 1);

	XFREE(argv);
	
	if(str) {
		XFREE(str);
	}
}


int
register_opts_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "option parsing tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_opts_tests);
	return 0;
}
