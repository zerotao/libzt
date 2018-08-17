/*
 * file.c        ZeroTao logging to a file
 *
 * Copyright (C) 2000, 2002, 2003, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: file.c,v 1.4 2003/11/26 05:05:30 jshiffer Exp $
 *
 */

/*
 * Description:
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "log_private.h"

typedef struct zt_log_file_ty zt_log_file_ty;
struct zt_log_file_ty {
    zt_log_ty inherited;
    FILE    * file;
};

static void
destructor(zt_log_ty * log) {
    zt_log_file_ty * this = (zt_log_file_ty*)log;

#ifdef WITH_THREADS
    pthread_mutex_lock(&log->mutex);
#endif /* WITH_THREADS */

    fflush(this->file);
    fclose(this->file);

#ifdef WITH_THREADS
    pthread_mutex_unlock(&log->mutex);
    pthread_mutex_destroy(&log->mutex);
#endif /* WITH_THREADS */

    zt_free(this);
}

static void
print(zt_log_ty * log, zt_log_level level, const char * file, int line, const char * func, const char * fmt, va_list ap) {
    char           * nfmt = NULL;

    zt_log_file_ty * this = (zt_log_file_ty*)log;

    nfmt = zt_log_gen_fmt(log, fmt, file, line, func, level, log->opts);

#ifdef WITH_THREADS
    pthread_mutex_lock(&log->mutex);
#endif /* WITH_THREADS */

    vfprintf(this->file, nfmt, ap);

#ifdef WITH_THREADS
    pthread_mutex_unlock(&log->mutex);
#endif /* WITH_THREADS */

    free(nfmt);
}

/* component data */
static zt_log_vtbl_ty vtbl = {
    sizeof(zt_log_file_ty),
    destructor,
    print,
};

zt_log_ty *
zt_log_file(char * file, int fopts, int lopts) {
    zt_log_file_ty * this;
    zt_log_ty      * result;

    result = zt_log_new(&vtbl, lopts);
    this   = (zt_log_file_ty*)result;

    if (fopts == ZT_LOG_FILE_APPEND) {
        this->file = fopen(file, "a");
    } else {
        this->file = fopen(file, "w");
    }
    if (!this->file) {
        fprintf(stderr, "Could not open file %s: %s\n", file, strerror(errno));
        return NULL;
    }

#ifdef WITH_THREADS
    pthread_mutex_init(&result->mutex, NULL);
#endif /* WITH_THREADS */

    return result;
}