/*
 * zt_opts.h        Option parsing routines
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 * 
 * $Id: opts.h,v 1.3 2003/06/09 18:57:39 jshiffer Exp $
 *
 */

#ifndef _ZT_OPTS_H_
#define _ZT_OPTS_H_

#include <libzt/zt.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif

typedef int (*opt_function)(void);
typedef int (*opt_ofunction)(char *);
typedef int (*opt_rfunction)(int, char**);

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
	int  opt;
	char *long_opt;
	char *description;
	opt_types type;
	void *val;
	char *usage;
};

extern int opts_process _(( int argc, char *argv[], struct opt_args *opts, char *option_string ));

#ifdef __cplusplus
}
#endif
#endif /*_ZT_OPTS_H_*/
