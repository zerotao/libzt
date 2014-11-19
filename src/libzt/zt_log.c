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
#include <libzt/zt_config.h>
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
    zt_log_ty * logp = data;

    if (logp) {
        zt_log_close(logp);
    }
}

/* #undef HAVE_PTHREADS */
#ifdef HAVE_PTHREADS
static pthread_mutex_t log_mutex    = PTHREAD_MUTEX_INITIALIZER;

static pthread_key_t   ctx_key;
static pthread_once_t  ctx_key_once = PTHREAD_ONCE_INIT;

static void
make_ctx_key(void) {
    (void)pthread_key_create(&ctx_key, free);
}

#define CTX_STATIC
#else /* ifdef HAVE_PTHREADS */
#define CTX_STATIC static
#endif /* HAVE_PTHREADS */

typedef struct zt_log_ctx_s zt_log_ctx_ty;
struct zt_log_ctx_s {
    const char * file;
    int          line;
    const char * function;
};


static zt_log_ty * log_default_ptr = NULL;
static int         forced          = 0;

static zt_log_ctx_ty *
zt_log_get_ctx(void) {
    CTX_STATIC zt_log_ctx_ty * log_ctx_ptr = NULL;

    if (!log_ctx_ptr) {
#ifdef HAVE_PTHREADS
        (void)pthread_once(&ctx_key_once, make_ctx_key);
        if ((log_ctx_ptr = pthread_getspecific(ctx_key)) == NULL) {
#endif /* HAVE_PTHREADS */
        if ((log_ctx_ptr = zt_calloc(zt_log_ctx_ty, 1)) == NULL) {
            fprintf(stderr, "Could not allocate memory for log context\n");
            exit(1);
        }
#ifdef HAVE_PTHREADS
        pthread_setspecific(ctx_key, log_ctx_ptr);
    }
#endif /* HAVE_PTHREADS */
    }
    return log_ctx_ptr;
}

static zt_log_ty *
zt_log_get_logger(void) {
    if (!log_default_ptr) {
#ifdef HAVE_PTHREADS
        pthread_mutex_lock(&log_mutex);
#endif
        /* second check is just to avoid having to have a mutex lock
         * around the default case of a logger being set
         */
        if (!log_default_ptr) {
            if ((log_default_ptr = zt_log_stderr(ZT_LOG_WITH_LEVEL)) == NULL) {
                fprintf(stderr, "Could not allocate stderr for logging\n");
                exit(1);
            }
            forced = 1;
            zt_atexit(log_atexit, log_default_ptr);
        }
#ifdef HAVE_PTHREADS
        pthread_mutex_unlock(&log_mutex);
#endif
    }

    return log_default_ptr;
}

static zt_log_ty *
zt_log_set_logger(zt_log_ty * log) {
    zt_log_ty   * last;

#ifdef HAVE_PTHREADS
    pthread_mutex_lock(&log_mutex);
#endif

    last = log_default_ptr;
    log_default_ptr = log;

#ifdef HAVE_PTHREADS
    pthread_mutex_unlock(&log_mutex);
#endif
    return last;
}

/* this function expects the traditional "If you allocated free it, if
 * you didn't allocate it leave it the fsck alone" model */
zt_log_ty *
zt_log_logger(zt_log_ty * log) {
    /* STATES:
     * log == NULL AND default == NULL           : create a new logger
     * log == NULL AND default == <logger>       : replace the current logger with log (NULL)
     * log == <logger> AND default == NULL       : replace the default logger with log
     * log == <logger> AND default == <logger>   : replace the default logger with log
     */

    /* if log is NULL and there is no default get one */
    if (log == NULL && log_default_ptr == NULL) {
        return zt_log_get_logger();
    }

    return zt_log_set_logger(log);
}

zt_log_level
zt_log_set_level(zt_log_ty * log, zt_log_level level) {
    zt_log_level olevel;

    if (!log && ((log = zt_log_get_logger()) == NULL)) {
        return 0;
    }

    olevel     = log->level;
    log->level = level;
    return olevel;
}

zt_log_level
zt_log_get_level(zt_log_ty * log) {
    if (!log && ((log = zt_log_get_logger()) == NULL)) {
        return 0;
    }

    return log->level;
}

unsigned int
zt_log_set_opts(zt_log_ty * log, unsigned int opts) {
    unsigned int oopts;

    if (!log && ((log = zt_log_get_logger()) == NULL)) {
        return 0;
    }

    oopts     = log->opts;
    log->opts = opts;
    return oopts;
}

unsigned int
zt_log_get_opts(zt_log_ty * log) {
    if (!log && ((log = zt_log_get_logger()) == NULL)) {
        return 0;
    }
    return log->opts;
}

void
zt_log_set_debug_info(const char * file, int line, const char * func) {
    zt_log_ctx_ty   * ctx;

    if ((ctx = zt_log_get_ctx()) == NULL) {
        return;
    }

    ctx->file     = (char*)file;
    ctx->line     = line;
    ctx->function = (char*)func;
}

void
zt_log_get_debug_info(const char ** file, int * line, const char ** func) {
    zt_log_ctx_ty   * ctx = NULL;

    if ((ctx = zt_log_get_ctx()) == NULL) {
        *file = NULL;
        *line = 0;
        *func = NULL;
        return;
    }

    *file = ctx->file;
    *line = ctx->line;
    *func = ctx->function;
}

void
zt_log_lprintf(zt_log_ty * log, zt_log_level level, const char * fmt, ...) {
    va_list ap;

    if (!log && ((log = zt_log_get_logger()) == NULL)) {
        zt_log_set_debug_info(NULL, -1, NULL);
        return;
    }

    if (level > log->level) {
        zt_log_set_debug_info(NULL, -1, NULL);
        return;
    }
    va_start(ap, fmt);
    zt_log_lvprintf(log, level, fmt, ap);
    va_end(ap);
}

void
zt_log_lvprintf(zt_log_ty * log, zt_log_level level, const char * fmt, va_list ap) {
    if (!log && ((log = zt_log_get_logger()) == NULL)) {
        goto EXIT;
    }

    if (level > log->level) {
        goto EXIT;
    }

    if (log->vtbl->print) {
        zt_log_ctx_ty   * ctx = zt_log_get_ctx();
        log->vtbl->print(log, level,
                         ctx ? ctx->file : NULL,
                         ctx ? ctx->line : 0,
                         ctx ? ctx->function : NULL,
                         fmt, ap);
    }

EXIT:
    /* debug info is only valid for a single call */
    zt_log_set_debug_info(NULL, -1, NULL);
}

void
zt_log_lstrerror(zt_log_ty * log, zt_log_level level, int errnum, const char * fmt, ...) {
    va_list ap;
    size_t llen;
    char    * nfmt;

    if (!log && ((log = zt_log_get_logger()) == NULL)) {
        zt_log_set_debug_info(NULL, -1, NULL);
        return;
    }

    if (level > log->level) {
        zt_log_set_debug_info(NULL, -1, NULL);
        return;
    }

    llen           = strlen(fmt);


    nfmt           = (char*)malloc(llen + 256);
    memcpy(nfmt, fmt, llen);
    nfmt[llen]     = ':';
    nfmt[llen + 1] = ' ';

    strerror_r(errnum, nfmt + (llen + 2), 255 - 2);

    va_start(ap, fmt);
    zt_log_lvprintf(log, level, nfmt, ap);
    va_end(ap);

    free(nfmt);
}

void
zt_log_close(zt_log_ty * log) {
    if (!log) {
        return;
    }

    if (log == log_default_ptr) {
        zt_log_set_logger(NULL);
    }

    if (log->vtbl->destructor) {
        log->vtbl->destructor(log);
    }
}
