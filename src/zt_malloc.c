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
zt_calloc_p(size_t num, size_t size)
{
    void_p mem = NULL;

    mem = (void_p)calloc((num), (size));

    zt_assert_always(mem);

    return mem;
}

void_p
zt_malloc_p(size_t num)
{
    void_p mem = NULL;

    mem = (void_p)malloc(num);
    zt_assert_always(mem);

    return mem;
}

void_p
zt_realloc_p(void_p p, size_t num)
{
    void_p mem = NULL;

    mem = (void_p)realloc(p, num);

    zt_assert_always(mem);
    return mem;
}


char *
zt_strdup_p(const char *string)
{
    char * volatile new_string = NULL;

    zt_assert_always(string);
    new_string = strdup(string);
    zt_assert_always(new_string);

    return new_string;
}
