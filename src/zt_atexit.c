/*!
 * Filename: zt_atexit.c
 * Description: Hook to extend the system atexit
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *      Copyright (C) 2010, Jason L. Shiffer.
 *      See file COPYING for details
 *
 * Notes:
 *
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#include "zt.h"
#include "zt_internal.h"

typedef struct zt_atexit_cb zt_atexit_cb;
struct zt_atexit_cb {
    zt_stack_t    member;
    void        (*fn)(void *data);
    void        * data;
};

/* FIXME: global needs thread safety */
static zt_stack(cb_stack);

static void
_atexit_call(void) {
    zt_stack_t          * se;
    struct zt_atexit_cb * cb;

    while(!zt_stack_empty(&cb_stack)) {
        se = zt_stack_pop(&cb_stack);
        cb = zt_stack_data(se, struct zt_atexit_cb, member);
        if(cb && cb->fn) {
            cb->fn(cb->data);
            zt_free(cb);
        }
    }
}

void
zt_atexit(void (*fn)(void * data), void * data) {
    struct zt_atexit_cb * cb;

    zt_assert(fn);

    cb = zt_calloc(struct zt_atexit_cb, 1);

    if(zt_stack_empty(&cb_stack)) {
        atexit(_atexit_call);
    }

    cb->fn = fn;
    cb->data = data;

    zt_stack_push(&cb_stack, &cb->member);
}
