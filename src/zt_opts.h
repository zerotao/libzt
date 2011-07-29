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

#include <zt.h>
#include <stdarg.h>
BEGIN_C_DECLS

#define ZT_OPT_NSO -1
#define ZT_OPT_NLO NULL

typedef int (*zt_opt_error)(int code, char * fmt, ...);
typedef struct zt_opt_def zt_opt_def_t;
typedef int (*zt_opt_callback)(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);
struct zt_opt_def {
    char                  sopt;
    char                * lopt;
    zt_opt_callback       cb;
    void                * cb_data;
    char                * help;
};
typedef void (*zt_opt_validate)(zt_opt_def_t * args, zt_opt_error error);

int zt_opt_null(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);

int zt_opt_intmax(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);
int zt_opt_int(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);
int zt_opt_long(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);
int zt_opt_flag_int(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);
int zt_opt_bool_int(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);
int zt_opt_string(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);

int zt_opt_help_stdout(int argn, int defn, int * argc, char **argv, zt_opt_def_t * def, zt_opt_error error);

char * zt_opt_get_value(int argn, char **argv, zt_opt_error error);

void zt_opt_validate_default(zt_opt_def_t * args, zt_opt_error error);
char * zt_opt_error_str(int code, char * fmt, ...);
char * zt_opt_verror_str(int code, char * fmt, va_list ap);
int zt_opt_error_default(int code, char * fmt, ...);
int zt_opt_verror_default(int code, char * fmt, va_list ap);
#define zt_opt_process(argc, argv, args)             \
    zt_opt_process_args(argc, argv, args, NULL, NULL)
int zt_opt_process_args(int * argc, char **argv, zt_opt_def_t * args, zt_opt_validate validate, zt_opt_error error);


END_C_DECLS
#endif /*_ZT_OPTS_H_*/
