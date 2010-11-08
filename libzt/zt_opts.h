/*!
 * Filename: zt_opts.h
 * Description: Command line option parsing
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *      Copyright (C) 2000-2010, Jason L. Shiffer.
 *      See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_OPTS_H_
#define _ZT_OPTS_H_

#include <zt_internal.h>

BEGIN_C_DECLS

#define ZT_OPT_NSO -1
#define ZT_OPT_NLO NULL

typedef int (*zt_opt_function)(char *, void *);

enum zt_opt_types {
    zt_opt_bool = 0,
    zt_opt_flag,
    zt_opt_long,
    zt_opt_string,
    zt_opt_func,  /* any arg */
    zt_opt_ofunc, /* optional arg */
    zt_opt_rfunc, /* required arg */
    zt_opt_help
};
typedef enum zt_opt_types zt_opt_types;

typedef struct zt_opt_args zt_opt_args;
struct zt_opt_args {
    int          opt;
    char       * long_opt;
    char       * description;
    zt_opt_types type;
    void       * val;
    int          (*fn)(char *, void *);
    char       * usage;
};

extern int  zt_opts_process( int *argc, char **argv[], struct zt_opt_args *opts, char *option_string, int auto_usage, int show_defaults, void * data);
extern void zt_opts_usage(char *argv[], struct zt_opt_args *opts, char *option_string, int max_opts, int show_defaults);

END_C_DECLS
#endif /*_ZT_OPTS_H_*/
