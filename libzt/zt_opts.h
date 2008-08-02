/****h* libZT/OptionParsing
 * DESCRIPTION
 *   Command line option parsing routines
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 * 
 ****/

#ifndef _ZT_OPTS_H_
#define _ZT_OPTS_H_

#include <libzt/zt.h>

BEGIN_C_DECLS

#define OPT_NSO -1
#define OPT_NLO NULL

typedef int (*opt_function)(char *, void *);

typedef enum opt_types opt_types;
enum opt_types {
		opt_bool=0,
		opt_flag,
		opt_int,
		opt_string,
		opt_func,
		opt_ofunc,
		opt_rfunc,
		opt_help
};

typedef struct opt_args opt_args;
struct opt_args 
{
	int  	  opt;
	char 	* long_opt;
	char 	* description;
	opt_types type;
	
	void	* val;
	int 	(*fn)(char *, void *);
	
	char 	* usage;
};

extern int opts_process _(( int *argc, char **argv[], struct opt_args *opts, char *option_string, int auto_usage, int show_defaults, void * data));
extern void opts_usage _((char *argv[], struct opt_args *opts, char *option_string, int max_opts, int show_defaults));

END_C_DECLS
#endif /*_ZT_OPTS_H_*/
