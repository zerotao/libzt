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


static INLINE int isoptchar(int x);

struct {
    zt_opt_types type;
    char        *desc;
} zt_opts_usage_t[] = { { zt_opt_bool,   " [yes|no|true|false]" },
                        { zt_opt_flag,   NULL                   },
                        { zt_opt_int,    " [integer]"           },
                        { zt_opt_string, " [string]"            },
                        { zt_opt_func,   NULL                   },
                        { zt_opt_ofunc,  NULL                   },
                        { zt_opt_rfunc,  NULL                   },
                        { zt_opt_help,   NULL                   },};

static void
print_default(zt_opt_types type, void *value)
{
    switch (type) {
        case zt_opt_bool:
            fprintf(stderr, " (default %s)", *(int *)value ? "true" : "false");
            break;
        case zt_opt_flag:
            fprintf(stderr, " (default %s)", *(int *)value ? "enabled" : "disabled");
            break;
        case zt_opt_int:
            fprintf(stderr, " (default %d)", *(int *)value);
            break;
        case zt_opt_string:
        {
            char * c = *(char **)value;
            if (c != NULL) {
                fprintf(stderr, " (default \"%s\")", c);
            }
        }
        break;
        case zt_opt_func:
            /* FALLTHRU */
        case zt_opt_ofunc:
            /* FALLTHRU */
        case zt_opt_rfunc:
            /* FALLTHRU */
        case zt_opt_help:
            break;
    } /* switch */
}

void
zt_opts_usage(char *argv[], struct zt_opt_args *opts, char *option_string, int max_opts, int show_defaults)
{
    int i = 0;

    fprintf(stderr, "usage: %s %s" NL, basename(argv[0]), option_string);

    if (max_opts > 0) {
        fprintf(stderr, NL "Options:" NL);
    }

    for (i = 0; i < max_opts; i++) {
        int offt = 0;
        if (opts[i].description != NULL) {
            if (isoptchar(opts[i].opt)) {
                offt = fprintf(stderr, BLANK "-%c", INDENT(1), opts[i].opt);
            }
#ifdef HAVE_GETOPT_LONG
            if (opts[i].long_opt) {
                if (isoptchar(opts[i].opt)) {
                    offt += fprintf(stderr, ", --%s", opts[i].long_opt);
                } else{
                    offt += fprintf(stderr, BLANK "    --%s", INDENT(1), opts[i].long_opt);
                }
            }
#endif /* ifdef HAVE_GETOPT_LONG */
            offt += fprintf(stderr, BLANK "%s", INDENT_TO(30, 5, offt), opts[i].description ? opts[i].description : "");

            if (show_defaults) {
                print_default(opts[i].type, opts[i].val);
            }

            if (opts[i].usage) {
                fprintf(stderr, BLANK ": %s" NL, INDENT(1), opts[i].usage);
            } else
            if (zt_opts_usage_t[opts[i].type].desc) {
                if (isoptchar(opts[i].opt)) {
                    fprintf(stderr, 
                            BLANK ": eg. -%c %s" NL,
                            INDENT(1),
                            opts[i].opt,
                            zt_opts_usage_t[opts[i].type].desc ? zt_opts_usage_t[opts[i].type].desc : "");
                } else{
                    fprintf(stderr,
                            BLANK ": eg. --%s %s" NL,
                            INDENT(1),
                            opts[i].long_opt,
                            zt_opts_usage_t[opts[i].type].desc ? zt_opts_usage_t[opts[i].type].desc : "");
                }
            } else {
                fprintf(stderr, NL);
            }
        }
    }
} /* zt_opts_usage */

int
zt_opts_process( int *argc, char **argv[], struct zt_opt_args *opts, char *option_string, int auto_usage, int show_defaults, void * cb_data)
{
#define OPT_MAX 512
    int  i = 0;
    char optstring[OPT_MAX];
    int  opt_index = 0;
    int  max_opts = 0;
    int  nopt_chars = -2;


#ifdef HAVE_GETOPT_LONG
    struct option *longopts = NULL;
#endif
    /* opterr = 0; */ /* turn off the default error message */
    if (!opts) {
        return(-1);
    }
    memset(optstring, '\0', OPT_MAX);

    for (i = 0; (opts[i].description != NULL); i++) {
        if (opts[i].opt <= 0) {
            opts[i].opt = nopt_chars--;
        }
    }

#ifdef HAVE_GETOPT_LONG
    longopts = XCALLOC(struct option, i + 1);
    optstring[0] = '+';
    opt_index++;
#endif


    for (i = 0; ((opts[i].description != NULL) || (opts[i].type != 0) || (opts[i].val != 0)) && opt_index < OPT_MAX; i++) {

        if (isoptchar(opts[i].opt)) {
            optstring[opt_index++] = opts[i].opt;
        }

#ifdef HAVE_GETOPT_LONG
        longopts[i].name = opts[i].long_opt;
        longopts[i].has_arg = 0;
        /* FIXME: I need to do something here to handle longopts without short opts */
        longopts[i].flag = NULL;
        longopts[i].val = opts[i].opt;
#endif /* ifdef HAVE_GETOPT_LONG */

        switch (opts[i].type) {
            case zt_opt_help:
            /* FALLTHRU */
            case zt_opt_flag:
            /* FALLTHRU */
            case zt_opt_func:
                break;

            case zt_opt_bool:
            /* FALLTHRU */
            case zt_opt_ofunc:
                if (isoptchar(opts[i].opt)) {
                    optstring[opt_index++] = ':'; /* '::' means argument optional */
                }

#ifdef HAVE_GETOPT_LONG
                longopts[i].has_arg = 2;
#endif
                break;

            case zt_opt_int:
            /* FALLTHRU */
            case zt_opt_string:
            /* FALLTHRU */
            case zt_opt_rfunc:
                if (isoptchar(opts[i].opt)) {
                    optstring[opt_index++] = ':'; /* argument required */
                }

#ifdef HAVE_GETOPT_LONG
                longopts[i].has_arg = 1;
#endif
                break;
        } /* switch */
    }
#ifdef HAVE_GETOPT_LONG
    longopts[i].name = 0;
    longopts[i].has_arg = 0;
    longopts[i].flag = NULL;
    longopts[i].val = 0;
#endif
    max_opts = i;
    while (1) {
#ifdef HAVE_GETOPT_LONG
        int c = getopt_long( *argc, *argv, optstring, longopts, 0 );
#else
        int c = getopt( *argc, *argv, optstring );
#endif
        int i = 0;
        if ( c == -1 ) {
            break;
        } else if ( c == '?' ) { /* unknown option */
            if (auto_usage) {
                zt_opts_usage(*argv, opts, option_string, max_opts, show_defaults);
            }
            return(EXIT_FAILURE);
        }
        for (i = 0; i < max_opts; i++) {
            if (c != opts[i].opt) {
                continue;
            }
            /* else */
            switch (opts[i].type) {
                case zt_opt_bool: /* set an int */
                    if (optarg) {
                        int offset = 0;
                        if (optarg[0] == '=') {
                            offset = 1;
                        }
                        if ((!(strcasecmp("true", &optarg[offset])))
                            || (!(strcasecmp("yes", &optarg[offset])))
                            || (!(strcasecmp("t", &optarg[offset])))) {
                            *(int *)opts[i].val = 1;
                        }else if ((!(strcasecmp("false", &optarg[offset])))
                                  || (!(strcasecmp("no", &optarg[offset])))
                                  || (!(strcasecmp("f", &optarg[offset])))) {
                            *(int *)opts[i].val = 0;
                        } else {
                            printf("Invalid value \"%s\" for %s (expecting [t|f|yes|no|true|false]).\n",
                                   *argv[optind - 1], *argv[optind - 2] );
                            if (auto_usage) {
                                zt_opts_usage(*argv, opts, option_string, max_opts, show_defaults);
                            }
                            return( EXIT_FAILURE) ;
                        }
                    } else {
                        *(int *)opts[i].val = !*(int *)opts[i].val;
                    }
                    break;
                case zt_opt_int:
                    *(int *)opts[i].val = strtol(optarg, NULL, 0);
                    /* can only fail if passed a string
                     * that does not start with a number
                     */
                    if ((*(int *)opts[i].val == 0) && (optarg[0] != '0')) {
                        printf("Invalid value \"%s\" for %s (expecting an integer).\n",
                               *argv[optind - 1], *argv[optind - 2] );
                        if (auto_usage) {
                            zt_opts_usage(*argv, opts, option_string, max_opts, show_defaults);
                        }
                        return( EXIT_FAILURE) ;
                    }
                    break;
                case zt_opt_string:
                    *(char **)opts[i].val = xstrdup(optarg); /* Should never fail!!! */
                    if (opts[i].val == NULL) {
                        printf("Invalid value \"%s\" for %s (expecting a string).\n",
                               *argv[optind - 1], *argv[optind - 2] );
                        if (auto_usage) {
                            zt_opts_usage(*argv, opts, option_string, max_opts, show_defaults);
                        }
                        return( EXIT_FAILURE) ;
                    }
                    break;
                case zt_opt_func:
                    if ( (((zt_opt_function)opts[i].fn)(optarg, cb_data)) == EXIT_FAILURE) {
                        if (auto_usage) {
                            zt_opts_usage(*argv, opts, option_string, max_opts, show_defaults);
                        }
                        return( EXIT_FAILURE) ;
                    }
                    break;
                case zt_opt_ofunc:
                    if ( ((opts[i].fn)(optarg, cb_data)) == EXIT_FAILURE) {
                        if (auto_usage) {
                            zt_opts_usage(*argv, opts, option_string, max_opts, show_defaults);
                        }
                        return( EXIT_FAILURE) ;
                    }
                    break;
                case zt_opt_rfunc:
                    if ( ((opts[i].fn)(optarg, cb_data)) == EXIT_FAILURE) {
                        if (auto_usage) {
                            zt_opts_usage(*argv, opts, option_string, max_opts, show_defaults);
                        }
                        return( EXIT_FAILURE) ;
                    }
                    break;
                case zt_opt_flag:
                    if (*(int *)opts[i].val > 0) {
                        *(int *)opts[i].val = 0;
                    }else{
                        *(int *)opts[i].val = 1;
                    }
                    break;
                case zt_opt_help:
                    if (auto_usage) {
                        zt_opts_usage(*argv, opts, option_string, max_opts, show_defaults);
                    }
                    return( -1) ;
                    break;
                default:
                    printf("Unknown arg type %d\n", opts[i].opt);
                    return( EXIT_FAILURE) ;
            } /* switch */
        }
    }
#ifdef HAVE_GETOPT_LONG
    XFREE(longopts);
#endif
    *argc -= optind;
    *argv += optind;
    return(0);
} /* zt_opts_process */

/*
 * local functions
 */
static INLINE int isoptchar(int x)
{
    return((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9'));
}
