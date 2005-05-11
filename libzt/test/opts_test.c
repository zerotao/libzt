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

int integer = 0;
char *str = 0;
int bool = 0;
int flag = 0;

opt_function func (void)
{
	printf("func\n");
	return EXIT_SUCCESS;
}

opt_ofunction ofunc (char *arg)
{
	printf("ofunc %s\n", arg);
	return EXIT_SUCCESS;	
}
opt_rfunction rfunc (int argc, char **argv)
{
	int i = 0;
	printf("rfunc ");
	for(i=argc; argv[i] != NULL; i ++){
		printf("%s ", argv[i]);
	}
	printf("\n");
	return EXIT_SUCCESS;	
}


int
main(int argc, char* argv[])
{
	struct opt_args options[] =
		{
			{ 'h', "help", "This help text", opt_help, NULL, NULL},
			{ '[', "int", "integer test", opt_int, &integer, NULL},
			{ 'b', "bool", "boolean test", opt_bool, &bool, NULL},
			{ 's', "string", "This is a test of a very long string\n\t\tIt is intended to wrap around a\n\t\tcouple of times.", opt_string, &str, ""},
			{ 'f', "func", "func test", opt_func, func, NULL},
			{ 'o', "ofunc", "ofunc test", opt_ofunc, ofunc, NULL},
			{ 'f', "rfunc", "rfunc test", opt_rfunc, rfunc, NULL},
			{ 'F', "flag", "flag test", opt_flag, &flag, NULL},			
			{ 0,0,0,0 }
		};
				
	opts_process(argc, argv, options, "[options]");
	printf("integer = %d, bool = %d, string = %s flag = %d\n", integer, bool, str ? str : "string", flag);

	if(str) {
		XFREE(str);
	}
	return 0;
}
