/*
 * opts.c        option parsing routines
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: opts.c,v 1.7 2003/11/26 17:37:16 jshiffer Exp $
 * 
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <unistd.h>
#include <ctype.h>

#ifdef HAVE_GETOPT_LONG
#include <getopt.h>
#endif

#include "zt.h"
#include "zt_opts.h"


static inline int isoptchar(int x);

struct
{
	opt_types type;
	char *desc;
} opts_usage_t[] = { { opt_bool, " [yes|no|true|false]" },
					 { opt_flag, NULL },
					 { opt_int, " [integer]" },
					 { opt_string, " [string]" },
					 { opt_func, NULL },
					 { opt_ofunc, NULL },
					 { opt_rfunc, NULL },
					 { opt_help, NULL }, };

static void
print_default(opt_types type, void *value) 
{
	switch (type) {
		case opt_bool:
			fprintf(stderr, " (default %s)", *(int *)value ? "true" : "false");
			break;
		case opt_flag:
		case opt_int:			
			fprintf(stderr, " (default %d)", *(int *)value);
			break;
		case opt_string:
			{
				char * c = *(char **)value;
				if (c != NULL) {
					fprintf(stderr, " (default \"%s\")", c);
				}
			}
			break;
		case opt_func:
		case opt_ofunc:
		case opt_rfunc:
		case opt_help:
			break;
	}
}

void
opts_usage(char *argv[], struct opt_args *opts, char *option_string, int max_opts, int show_defaults){
	int 	  i = 0;
	
	fprintf(stderr, "Usage: %s %s"NL, basename(argv[0]), option_string);

	if(max_opts > 0) {
		fprintf(stderr, NL"Options:"NL);
	}
	
	for(i=0; i < max_opts; i++){
		int		  offt = 0;
		if(opts[i].description != NULL){
			if(isoptchar(opts[i].opt))
				offt = fprintf(stderr, BLANK "-%c", INDENT(1), opts[i].opt);
#ifdef HAVE_GETOPT_LONG				
			if(opts[i].long_opt){
				if(isoptchar(opts[i].opt))
					offt += fprintf(stderr, ", --%s", opts[i].long_opt);
				else
					offt += fprintf(stderr, BLANK "    --%s", INDENT(1), opts[i].long_opt);
			}
#endif
			offt += fprintf(stderr, BLANK "%s", INDENT_TO(30, 5, offt), opts[i].description ? opts[i].description : "");
			
			if(show_defaults) {
				print_default(opts[i].type, opts[i].val);
			}
			
			if(opts[i].usage)
				fprintf(stderr, BLANK ": %s"NL, INDENT(1), opts[i].usage);
			else
				if(opts_usage_t[opts[i].type].desc) {
					if(isoptchar(opts[i].opt))
						fprintf(stderr, BLANK ": eg. -%c %s"NL, INDENT(1), opts[i].opt, opts_usage_t[opts[i].type].desc ? opts_usage_t[opts[i].type].desc : "");
					else
						fprintf(stderr, BLANK ": eg. --%s %s"NL, INDENT(1), opts[i].long_opt, opts_usage_t[opts[i].type].desc ? opts_usage_t[opts[i].type].desc : "");
				} else {
					fprintf(stderr, NL);
				}
		}
	}
}

int
opts_process( int *argc, char **argv[], struct opt_args *opts, char *option_string, int auto_usage, int show_defaults, void * cb_data)
{
	int i = 0;
#define OPT_MAX 255
#define OPT_MAX_DOUBLE 512
	char optstring[OPT_MAX_DOUBLE];
	int opt_index = 0;
	int max_opts = 0;
	int nopt_chars = -2;
	
		
#ifdef HAVE_GETOPT_LONG
	struct option *longopts = NULL;
#endif
	//		opterr = 0;												/* turn off the default error message */
	if(!opts)
		return -1;
	memset(optstring, '\0', OPT_MAX_DOUBLE);

	for(i=0; (opts[i].description != NULL); i++) {
		if (opts[i].opt <= 0) {
			opts[i].opt = nopt_chars--;
		}
	}
	
#ifdef HAVE_GETOPT_LONG
	longopts = XCALLOC(struct option, i+1);
#endif
		
	for(i=0; ((opts[i].description != NULL) || (opts[i].type !=0) || (opts[i].val != 0)) && opt_index < OPT_MAX_DOUBLE; i++){
		
		if(isoptchar(opts[i].opt))
			optstring[opt_index++] = opts[i].opt;
				
#ifdef HAVE_GETOPT_LONG
		longopts[i].name = opts[i].long_opt;
		longopts[i].has_arg = 0;
		/* FIXME: I need to do something here to handle longopts without short opts */
		longopts[i].flag = NULL;
		longopts[i].val = opts[i].opt;
#endif
				
		switch (opts[i].type){
			case opt_help:
				/* FALLTHRU */
			case opt_flag:
				/* FALLTHRU */
			case opt_func:
				break;
				
			case opt_bool:
				/* FALLTHRU */
			case opt_ofunc:
				if(isoptchar(opts[i].opt)) {
					optstring[opt_index++] = ':';			/* '::' means argument optional */
				}
				
#ifdef HAVE_GETOPT_LONG
				longopts[i].has_arg = 2;
#endif
				break;
				
			case opt_int:
				/* FALLTHRU */
			case opt_string:
				/* FALLTHRU */
			case opt_rfunc:
				if(isoptchar(opts[i].opt)) {
					optstring[opt_index++] = ':';			/* argument required */
				}
				
#ifdef HAVE_GETOPT_LONG
				longopts[i].has_arg = 1;
#endif
				break;
		}
	}
#ifdef HAVE_GETOPT_LONG
	longopts[i].name = 0;
	longopts[i].has_arg = 0;
	longopts[i].flag = NULL;
	longopts[i].val = 0;
#endif
	max_opts = i;
	while(1) {
#ifdef HAVE_GETOPT_LONG
		int c = getopt_long ( *argc, *argv, optstring, longopts, 0 );
#else
		int c = getopt ( *argc, *argv, optstring );
#endif				
		int i = 0;
		if ( c == -1 )
			break;
		else if ( c == '?' ){ /* unknown option */
			if(auto_usage) {
				opts_usage(*argv, opts, option_string, max_opts, show_defaults);
			}
			return EXIT_FAILURE;
		}
		for(i=0; i < max_opts; i++) {
			if(c != opts[i].opt) {
				continue;
			}
			/* else */
			switch (opts[i].type) {
				case opt_bool: /* set an int */
					if(optarg){
						int offset = 0;
						if(optarg[0] == '=')
							offset = 1;
						if((!(strcasecmp("true", &optarg[offset])))
						   || (!(strcasecmp("yes", &optarg[offset])))
						   || (!(strcasecmp("t", &optarg[offset])))){
							*(int *)opts[i].val = 1;
						}else if((!(strcasecmp("false", &optarg[offset])))
							 || (!(strcasecmp("no", &optarg[offset])))
							 || (!(strcasecmp("f", &optarg[offset])))){
							*(int *)opts[i].val = 0;
						} else {
							printf("Invalid value \"%s\" for %s (expecting [t|f|yes|no|true|false]).\n",
							       *argv[optind-1], *argv[optind-2] );
							if(auto_usage) {
								opts_usage(*argv, opts, option_string, max_opts, show_defaults);
							}
							return EXIT_FAILURE;
						}
					} else {
						*(int *)opts[i].val = !*(int *)opts[i].val;
					}										
					break;
				case opt_int:
					*(int *)opts[i].val = strtol(optarg, NULL, 0);
					/* can only fail if passed a string
					 * that does not start with a number
					 */
					if((*(int *)opts[i].val == 0) && (optarg[0] != '0')){ 
						printf("Invalid value \"%s\" for %s (expecting an integer).\n",
						       *argv[optind-1], *argv[optind-2] );
						if(auto_usage) {
							opts_usage(*argv, opts, option_string, max_opts, show_defaults);
						}
						return EXIT_FAILURE;
						}
					break;
				case opt_string:
					*(char **)opts[i].val = xstrdup(optarg); /* Should never fail!!! */
					if(opts[i].val == NULL){
						printf("Invalid value \"%s\" for %s (expecting a string).\n",
						       *argv[optind-1], *argv[optind-2] );
						if(auto_usage) {
							opts_usage(*argv, opts, option_string, max_opts, show_defaults);
						}
						return EXIT_FAILURE;
					}
					break;
				case opt_func:
					if( (((opt_function)opts[i].fn)(optarg, cb_data)) == EXIT_FAILURE){
						if(auto_usage) {
							opts_usage(*argv, opts, option_string, max_opts, show_defaults);
						}
						return EXIT_FAILURE;
					}
					break;	
				case opt_ofunc:
					if( ((opts[i].fn)(optarg, cb_data)) == EXIT_FAILURE){
						if(auto_usage) {
							opts_usage(*argv, opts, option_string, max_opts, show_defaults);
						}
						return EXIT_FAILURE;
					}
					break;									
				case opt_rfunc:										
					if( ((opts[i].fn)(optarg, cb_data)) == EXIT_FAILURE){
						if(auto_usage) {
							opts_usage(*argv, opts, option_string, max_opts, show_defaults);
						}
						return EXIT_FAILURE;
					}
					break;
				case opt_flag:
					if(*(int *)opts[i].val > 0){
						*(int *)opts[i].val = 0;
					}else{
						*(int *)opts[i].val = 1; 
					}
					break;
				case opt_help:
					if(auto_usage) {
						opts_usage(*argv, opts, option_string, max_opts, show_defaults);
					}
					return -1;
					break;
				default:
					printf("Unknown arg type %d\n", opts[i].opt);
					return EXIT_FAILURE;
			}
		}
	}
#ifdef HAVE_GETOPT_LONG
	XFREE(longopts);
#endif
	*argc -= optind;
	*argv += optind;
	return 0;
}

/*
 * local functions 
 */
static inline int isoptchar(int x)
{
	return  (x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9');
}
