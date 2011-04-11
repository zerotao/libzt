/*!
 * Filename: zt_log.c
 * Description: generic logging interface
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#include "zt.h"
#include "zt_internal.h"

#include "zt_log.h"
#include "zt_log/log_private.h"
#include "zt_atexit.h"

static void
log_atexit(void * data) {
    zt_log_ty   * logp = data;

    logp = zt_log_logger(NULL);

    if(logp) {
        zt_log_close(logp);
    }
}

zt_log_ty *
zt_log_logger(zt_log_ty *log)
{
    static zt_log_ty    * log_log_ptr = NULL;
    zt_log_ty           * last = NULL;

    if (!log) {
        /* caller wants the current logger */
        if(!log_log_ptr) {
            /* allocate a default logger */
            log_log_ptr = zt_log_stderr( ZT_LOG_WITH_LEVEL );
            zt_atexit(log_atexit, log_log_ptr);
        }
        return log_log_ptr;
    }

    last = log_log_ptr;
    log_log_ptr = log;
    return last;
}

zt_log_ty *
zt_log_debug_logger(zt_log_ty *log)
{
    static zt_log_ty *log_debug_ptr = NULL;
    static int        forced = 0;

    if (((!log) && (!log_debug_ptr))) {
        log_debug_ptr = zt_log_stderr( ZT_LOG_WITH_LEVEL );
        forced = 1;
    }

    if (log) {
        if (forced) {
            zt_log_close(log_debug_ptr), forced = 0;
        }
        log_debug_ptr = log;
    }

    return log_debug_ptr;
}

zt_log_level
zt_log_set_level(zt_log_ty *log, zt_log_level level)
{
    zt_log_level olevel;

    if (!log) {
        log = zt_log_logger(NULL);
    }

    olevel = log->level;
    log->level = level;
    return olevel;
}

zt_log_level
zt_log_get_level(zt_log_ty *log)
{
    if (!log) {
        log = zt_log_logger(NULL);
    }
    return log->level;
}

unsigned int
zt_log_set_opts(zt_log_ty *log, unsigned int opts)
{
    unsigned int oopts;

    if (!log) {
        log = zt_log_logger(NULL);
    }
    oopts = log->opts;
    log->opts = opts;
    return oopts;
}

unsigned int
zt_log_get_opts(zt_log_ty *log)
{
    if (!log) {
        log = zt_log_logger(NULL);
    }
    return log->opts;
}

void
zt_log_set_debug_info(zt_log_ty *log, const char *file, int line, const char *func)
{
    if (!log) {
        log = zt_log_logger(NULL);
    }

    log->file = (char *)file;
    log->line = line;
    log->function = (char *)func;
}

void
zt_log_get_debug_info(zt_log_ty *log, char **file, int *line, char **func)
{
    if (!log) {
        log = zt_log_logger(NULL);
    }

    *file = log->file;
    *line = log->line;
    *func = log->function;
}

void
zt_log_lprintf(zt_log_ty *log, zt_log_level level, char *fmt, ...)
{
    va_list ap;

    if (!log) {
        log = zt_log_logger(NULL);
    }

    if (level > log->level) {
        return;
    }
    va_start(ap, fmt);
    zt_log_lvprintf(log, level, fmt, ap);
    va_end(ap);
}

void
zt_log_lvprintf(zt_log_ty *log, zt_log_level level, char *fmt, va_list ap)
{
    if (!log) {
        log = zt_log_logger(NULL);
    }

    if (level > log->level) {
        return;
    }
    if (log->vtbl->print) {
        log->vtbl->print(log, level, fmt, ap);
    }
    zt_log_set_debug_info(log, NULL, -1, NULL);
}

void
zt_log_lstrerror(zt_log_ty *log, zt_log_level level, int errnum, char *fmt, ...)
{
    va_list   ap;
    size_t    llen;
    char    * nfmt;

    if (!log) {
        log = zt_log_logger(NULL);
    }

    if (level > log->level) {
        return;
    }

    llen = strlen(fmt);


    nfmt = (char *)malloc(llen + 256);
    memcpy(nfmt, fmt, llen);
    nfmt[llen] = ':';
    nfmt[llen + 1] = ' ';

    strerror_r(errnum, nfmt + (llen + 2), 255 - 2);

    va_start(ap, fmt);
    zt_log_lvprintf(log, level, nfmt, ap);
    va_end(ap);

    free(nfmt);
}

void
_zt_log_debug(char *fmt, ...)
{
    va_list      ap;
    zt_log_level level = zt_log_debug;
    zt_log_ty   *log = zt_log_debug_logger(NULL);

    if (level > log->level) {
        return;
    }
    va_start(ap, fmt);
    zt_log_lvprintf(log, level, fmt, ap);
    va_end(ap);
}

void
_zt_log_vdebug(char *fmt, va_list ap)
{
    zt_log_level level = zt_log_debug;
    zt_log_ty   *log = zt_log_debug_logger(NULL);

    if (level > log->level) {
        return;
    }
    if (log->vtbl->print) {
        log->vtbl->print(log, level, fmt, ap);
    }
    zt_log_set_debug_info(log, NULL, -1, NULL);
}

void
zt_log_close(zt_log_ty *log)
{
    if (!log) {
        log = zt_log_logger(NULL);
    }

    if (log->vtbl->destructor) {
        log->vtbl->destructor(log);
    }
}
