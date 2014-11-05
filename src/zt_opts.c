/*
 * opts.c        option parsing routines
 *
 * Copyright (C) 2000-2011, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: opts.c,v 1.7 2003/11/26 17:37:16 jshiffer Exp $
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#ifdef HAVE_STRING_H
# include <string.h> /* memset, strdup */
#endif

#ifdef HAVE_LIBGEN_H
# include <libgen.h> /* basename */
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif
/* #include <getopt.h> [> getopt, getopt_long <] */

#include "zt.h"
#include "zt_opts.h"

char *
zt_opt_error_str(int code, char * fmt, ...) {
    char  * ret;
    va_list ap;

    va_start(ap, fmt);
    ret = zt_opt_verror_str(code, fmt, ap);
    va_end(ap);
    return ret;
}

char *
zt_opt_verror_str(int code, char * fmt, va_list ap) {
    char * msg          = NULL;
    char * user_message = NULL;
    char * final        = NULL;

    asprintf(&msg, "error: { code: %d, string: \"%s", code, code ? strerror(code) : "");
    if (fmt) {
        vasprintf(&user_message, fmt, ap);
    }
    asprintf(&final, "%s: %s\" }", msg, user_message);

    zt_free(user_message);
    zt_free(msg);
    return final;
}

int
zt_opt_verror_default(int code, char * fmt, va_list ap) {
    char * msg = NULL;

    if (!code && !fmt) {
        /* no code or format equates to calling exit */
        return -1;
    }

    msg = zt_opt_verror_str(code, fmt, ap);
    fprintf(stderr, "%s\n", msg);
    zt_free(msg);
    return code;
}

int
zt_opt_error_default(int code, char * fmt, ...) {
    int     ret;
    va_list ap;

    va_start(ap, fmt);
    ret = zt_opt_verror_default(code, fmt, ap);
    va_end(ap);

    return ret;
}

char *
zt_opt_get_value(int argn, int defn, char ** argv, zt_opt_def_t * def, int *args_consumed, zt_opt_error error) {
    char * p      = argv[argn];
    char * pp     = NULL;
    char * result = NULL;
    char * lopt   = def[defn].lopt;

    *args_consumed = 0;

    /* skip past any leading '-' */
    while(*p == '-') { p++; }

    pp = p;
    do {
        if (*pp == '=') {
            result = ++pp;
            break;
        }
    } while (*pp++);

    if (!result) {
        pp = p;
        /* there was no '=' embedded
         * try matching the long opt name
         */
        if (strcmp(lopt, pp) == 0) {
            result = argv[argn + 1]; /* default to the next option */
            *args_consumed = 1;
        } else {
            /* short opt */
            pp++;
            if(*pp) {
                result = pp;
            } else {
                result = argv[argn + 1];
                *args_consumed = 1;
            }
        }
    }

    if (!result) {
        error(EINVAL, "while processing arg '%s'", argv[argn]);
    }

    return result;
}

int
zt_opt_null(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    return 0;
}

#ifdef HAVE_INTTYPES_H
int
zt_opt_intmax(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    intmax_t result;
    char   * end = NULL;
    int      args_consumed = 0;
    char   * arg = zt_opt_get_value(argn, defn, argv, def, &args_consumed, error);

    errno  = 0;
    result = strtoimax(arg, &end, 0);

    if (errno) {
        /* check errno */
        error(errno, "invalid number '%s'", arg);
    }

    if (end && *end) {
        /* invalid character */
        error(EINVAL, "'%s' is not a number", arg);
    }

    if (def[defn].cb_data) {
        *(intmax_t *)def[defn].cb_data = result;
    }

    return args_consumed;
}

#endif /* HAVE_INTTYPES_H */

int
zt_opt_long(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    long   result;
    char * end = NULL;
    int    args_consumed = 0;
    char * arg = zt_opt_get_value(argn, defn, argv, def, &args_consumed, error);

    errno  = 0;
    result = strtol(arg, &end, 0);

    if (errno) {
        /* check errno */
        error(errno, "invalid number '%s'", arg);
    }

    if (end && *end) {
        /* invalid character */
        error(EINVAL, "'%s' is not a number", arg);
    }

    if (def[defn].cb_data) {
        *(long *)def[defn].cb_data = result;
    }

    return args_consumed;
}

int
zt_opt_bool_int(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    int    result;
    size_t len;
    int    args_consumed = 0;
    char * arg = zt_opt_get_value(argn, defn, argv, def, &args_consumed, error);

    len = strlen(arg);

    if (len == 1) {
        switch (*arg) {
            case 't':
            case 'T':
            case 'y':
            case 'Y':
                result = 1;
                break;
            case 'f':
            case 'F':
            case 'n':
            case 'N':
                result = 0;
                break;
            default:
                error(EINVAL, "'%s' is not a boolean expression", argv[argn]);
                return 0;
        }
    } else {
        if (strcasecmp(arg, "true") == 0 ||
            strcasecmp(arg, "yes") == 0) {
            result = 1;
        } else if (strcasecmp(arg, "false") == 0 ||
                   strcasecmp(arg, "no") == 0) {
            result = 0;
        } else {
            error(EINVAL, "'%s' is not a boolean expression", argv[argn]);
            return 0;
        }
    }

    if (def[defn].cb_data) {
        *(int *)def[defn].cb_data = result;
    }

    return args_consumed;
} /* zt_opt_bool_int */

int
zt_opt_flag_int(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    if (def[defn].cb_data) {
        *(int *)def[defn].cb_data += 1;
    }

    return 0;
}

int
zt_opt_string(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    int    args_consumed = 0;
    char * arg = zt_opt_get_value(argn, defn, argv, def, &args_consumed, error);

    if (arg) {
        *(char **)def[defn].cb_data = (void *)strdup(arg);
    }

    return args_consumed;
}

static int
_zt_opt_help_stdout_printer(zt_opt_def_t * def) {
    char   sopt = def->sopt;
    char * lopt = def->lopt;
    char * arg  = def->arg;

    if (sopt != ZT_OPT_NSO || lopt != ZT_OPT_NLO) {
        int depth = 0;

        depth = printf("   %c%c%c %s%s %s",
                       sopt != ZT_OPT_NSO ? '-' : ' ',
                       sopt != ZT_OPT_NSO ? sopt : ' ',
                       sopt != ZT_OPT_NSO && lopt != ZT_OPT_NLO ? ',' : ' ',
                       lopt != ZT_OPT_NLO ? "--" : "  ",
                       lopt != ZT_OPT_NLO ? lopt : "",
                       arg != NULL ? arg : ""
                       );

        size_t len = strlen(def->help);

/*
 * target a total width of < 80 columns
 * later we may look at getting the current width of the sreen
 */
#define BASE_INDENT 30
#define BASE_WIDTH (80 - BASE_INDENT)

        if (len > BASE_WIDTH) {
            size_t offt = 0;
            while (offt < len) {
                size_t width = BASE_WIDTH;

                if (offt != 0) {
                    /* indent every line other then the first, as it is already indented */
                    printf(BLANK, depth, " ");
                }

                /* ignore leading whitespace */
                while(def->help[offt] == ' ' && offt < len) {
                    offt++;
                }

                size_t sublen = len - offt;

                if (width > sublen) {
                    // if we have reached the length of the remaining string then
                    // use all of it.
                    width = sublen;
                } else {
                    // try to break between words
                    while(def->help[offt+width] != ' ') {
                        width--;
                    }
                }

                printf(BLANK "%.*s" NL, INDENT_TO(BASE_INDENT, 5, depth), (int)width, &def->help[offt]);
                offt+=width;
            }

        } else {
            printf(BLANK "%s" NL, INDENT_TO(BASE_INDENT, 5, depth), def->help);
        }
    } else {
        return -1;
    }

    return 0;
}

int
zt_opt_help_stdout(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    int    i;
    char * help = def[defn].cb_data;

    printf("Usage: %s %s\nOptions:\n", argv[0], help ? help : "");

    for (i = 0; def[i].cb; i++) {
        if (_zt_opt_help_stdout_printer(&def[i]) < 0) {
            error(0, "Programmer Error: must have a short or long opt at least", NULL);
            return -1;
        }
    }

    error(0, NULL);

    return 0;
}

int
zt_opt_usage(const char * name, const char * help, zt_opt_def_t * opts) {
    int i;

    printf("Usage: %s %s\nOptions:\n", name, help ? help : "");

    for (i = 0; opts[i].cb != NULL; i++) {
        if (_zt_opt_help_stdout_printer(&opts[i]) < 0) {
            return -1;
        }
    }

    return 0;
}

void
zt_opt_validate_default(zt_opt_def_t * args, zt_opt_error error) {
    int i;
    int x;

    for (i = 0; args[i].cb; i++) {
        if (args[i].sopt == ZT_OPT_NSO && args[i].lopt == ZT_OPT_NLO) {
            error(EINVAL, "Invalid argdef initialization #%d (must include at least one of short opt or long opt)", i);
        }

        if (!args[i].help) {
            error(EINVAL, "Invalid argdef initialization #%d (must include help definition)", i);
        }

        if (args[i].sopt != ZT_OPT_NSO &&
            ((args[i].sopt < '0' || args[i].sopt > '9') &&   /* 0x30-0x39 */
             (args[i].sopt < 'A' || args[i].sopt > 'Z') &&   /* 0x41-0x5A */
             (args[i].sopt < 'a' || args[i].sopt > 'z'))) {  /* 0x61-0x7A */
            error(EINVAL, "Invalid short option definition #%d (char must be [0-9A-Za-z] not '0x%x')", i, args[i].sopt);
        }

        /* slow and does extra work but functional */
        for (x = 0; args[x].cb; x++) {
            if (x == i) {
                continue;
            }

            if (args[x].sopt != ZT_OPT_NSO && args[x].sopt == args[i].sopt) {
                error(EINVAL, "Duplicate short options: #%d and #%d", i, x);
            }

            if (args[x].lopt != ZT_OPT_NLO && args[i].lopt != ZT_OPT_NLO &&
                    (strcmp(args[x].lopt, args[i].lopt) == 0)) {
                error(EINVAL, "Duplicate long options: #%d and #%d", i, x);
            }
        }
    }
}

static int
_zt_opt_error_wrapper(int code, char * fmt, ...) {
    int     ret;
    va_list ap;

    va_start(ap, fmt);
    ret = zt_opt_verror_default(code, fmt, ap);
    va_end(ap);

    if (ret != 0) {
        exit(ret);
    }

    return ret;
}

int
zt_opt_process_args(int * argc, char ** argv, zt_opt_def_t * args, zt_opt_validate validate, zt_opt_error error) {
    int i;

    error    = error ? error : _zt_opt_error_wrapper;
    validate = validate ? validate : zt_opt_validate_default;

    validate(args, error);

    for (i = 1; i < *argc; i++) {
        if (argv[i][0] == '-') {
            unsigned int x;
            size_t       len          = 0;
            char       * p            = &argv[i][1];
            int          combined_opt = 0;
            int          found        = 0;
            int          long_opt     = 0;

            if (*p == '-') { /* long opt */
                long_opt = 1;
                ++p;
                for (len = 0; p[len] != '\0' && p[len] != '='; len++) {
                    ;
                }

                if (p[len] == '=') {
                    combined_opt = 1;
                }
            }


AGAIN:
            for (x = 0; args[x].cb; x++) {
                if ((len == 0 && args[x].sopt != ZT_OPT_NSO && *p == args[x].sopt) || /* short opt */
                    /* long opt */
                    (args[x].lopt != ZT_OPT_NLO && strlen(args[x].lopt) == len &&
                     strncmp(p, args[x].lopt, len) == 0)) {
                    int consumed = 0;

                    found = 1;
                    if ((consumed = args[x].cb(i, x, argc, argv, args, error)) < 0) {
                        /* error */
                        return consumed;
                    }

                    if (!combined_opt) {
                        i += consumed;
                    }

                    if(!consumed && !long_opt && *(++p)) {
                        /* if we did not consume extra args AND
                         * we are a short option AND
                         * there are more short options to consume
                         */
                        goto AGAIN;
                    }

                    break;
                }
            }

            if (!found) {
                error(EINVAL, "'%.*s'", len ? len : 1, p);
            }
        } else {
            break;
        }
    }

    *argc = i;

    return 0;
} /* zt_opt_process_args */

