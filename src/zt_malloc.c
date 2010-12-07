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

#include "zt_internal.h"
#include "zt_assert.h"
#include "zt_log.h"

void_p
xcalloc_p(size_t num, size_t size)
{
    void_p mem = NULL;

    mem = (void_p)calloc((num), (size));

    zt_assert(mem);

    return mem;
}

void_p
xmalloc_p(size_t num)
{
    void_p mem = NULL;

    mem = (void_p)malloc(num);
    zt_assert(mem);

    return mem;
}

void_p
xrealloc_p(void_p p, size_t num)
{
    void_p mem = NULL;

    mem = (void_p)realloc(p, num);

    zt_assert(mem);

    return mem;
}

void
xfree_p(void_p stale)
{

    zt_assert(!stale);

    free(stale);
}

char *
xstrdup(const char *string)
{
    char * volatile new_string = NULL;

    zt_assert(string);
    new_string = strdup(string);
    zt_assert(new_string);

    return new_string;
}
