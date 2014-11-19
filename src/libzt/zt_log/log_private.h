/*
 * private.h        logging private definitions
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: zt_log_private.h,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/* Private interface to log
 * You should NOT access this directly
 * unless you are extending the component
 */
#ifndef _LOG_PRIVATE_H_
#define _LOG_PRIVATE_H_

#include <stdlib.h>
#include <zt_log.h>

#ifdef WITH_THREADS
#include <pthread.h>
#endif /* WITH_THREADS */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct zt_log_level_desc_ty zt_log_level_desc_ty;
struct zt_log_level_desc_ty {
    zt_log_level level;
    char       * desc;
};
extern zt_log_level_desc_ty zt_log_level_desc[];

#define ZT_LOG_LEVEL_MAX 8 + 1

struct zt_log_ty {
    struct zt_log_vtbl_ty * vtbl;
    unsigned int            opts;
    zt_log_level            level;
#ifdef WITH_THREADS
    pthread_mutex_t mutex;
#endif /*
        * WITH_THREADS
        * rest of opts
        */
};

typedef struct zt_log_vtbl_ty       zt_log_vtbl_ty;
struct zt_log_vtbl_ty {
    size_t size;
    /* virtual function pointers */
    void (* destructor)(zt_log_ty * log);
    void (* print)(zt_log_ty * log, zt_log_level level, const char * file, int line, const char * func, const char * fmt, va_list ap);
};

zt_log_ty * zt_log_new(zt_log_vtbl_ty * vptr, unsigned int opts);
char      * zt_log_gen_fmt(zt_log_ty * log, const char * fmt, const char * file, int line, const char * function, zt_log_level level, unsigned int opts);

#ifdef __cplusplus
}
#endif
#endif  /* _LOG_PRIVATE_H_ */