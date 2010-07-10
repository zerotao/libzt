/*
 * zt_malloc.c        wrappers for malloc et. el.
 *
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: malloc.c,v 1.4 2003/06/09 16:55:09 jshiffer Exp $
 *
 */


#include <string.h>

#include "zt.h"
#include "zt_log.h"
#include "zt_exceptions.h"

void_p
xcalloc_p(size_t num, size_t size)
{
    void_p mem = NULL;

    if (!(mem = (void_p *)calloc((num), (size)))) {
        TRY_THROW(zt_exception.memory.no_mem);
    }
    return (mem);
}

void_p
xmalloc_p(size_t num)
{
    void_p mem = NULL;

    if (!(mem = (void_p *)malloc(num))) {
        TRY_THROW(zt_exception.memory.no_mem);
    }
    return (mem);
}

void_p
xrealloc_p(void_p p, size_t num)
{
    void_p mem = NULL;

    if (!(mem = (void_p *)realloc(p, num))) {
        TRY_THROW(zt_exception.memory.no_mem);
    }
    return (mem);
}

void
xfree_p(void_p stale)
{
    if (!stale) {
        TRY_THROW(zt_exception.memory.no_mem);
    }

    free(stale);
}

char *
xstrdup(const char *string)
{
    char *new_string = NULL;

    if ( string ) {
        if (!(new_string = strdup(string))) {
            /* zt_log_printf( zt_log_emerg, "Could not xstrdup string."); */
            TRY_THROW(zt_exception.memory.no_mem);
        }
    }
    return (new_string);
}

