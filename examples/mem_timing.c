/*
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 *
 */

/*
 * Description:
 */
#include <stdio.h>
#include <stdlib.h>

#if !defined(WIN32)
# include <sys/time.h>    /* gettimeofday */
# include <sys/resource.h>
#endif

#include <zt.h>

struct test_data_t {
    long                 size;
    int                  n;
    struct zt_mem_pool * pool;
    void               **parr;
};


void
timetest(char *name, int n, void *(*test)(void *), void *data)
{
    struct time_result result;

    zt_time(n, &result, test, data);
    zt_time_print_result(&result, name, n);
}

void *
test_malloc(void *data)
{
    struct test_data_t * td = data;
    size_t               size = td->size;
    int                  i;

    for (i = 0; i < td->n; i++) {
        td->parr[i] = malloc(size);
    }

    for (i = 0; i < td->n; i++) {
        free(td->parr[i]);
    }

    return 0;
}

void *
test_pool(void *data)
{
    struct test_data_t * td = data;
    struct zt_mem_pool * pool = td->pool;
    int                  i;

    for (i = 0; i < td->n; i++) {
        td->parr[i] = zt_mem_pool_alloc(pool);
    }

    for (i = 0; i < td->n; i++) {
        zt_mem_pool_release(&(td->parr[i]));
    }

    return 0;
}

#define NPTEST 10000
#define NPLOOP 500
int
main(int argc UNUSED, char *argv[] UNUSED)
{
    struct test_data_t td_small;
    struct test_data_t td_large;
    void             **parr;

    parr = (void **)calloc(NPTEST + 1, sizeof(void *));

    td_small.parr = parr;
    td_small.size = sizeof(int);
    td_small.n = NPTEST;

    td_large.parr = parr;
    td_large.size = 150 * sizeof(int);
    td_large.n = NPTEST;


    timetest("malloc_small", NPLOOP, test_malloc, (void *)&td_small);
    timetest("malloc_large", NPLOOP, test_malloc, (void *)&td_large);

    /* I do not do these until here so that malloc has free reign on memory
     * on at least one machine this made the difference between malloc taking
     * 27 seconds versus 16 after the move.
     */
    td_small.pool = zt_mem_pool_init("small_pool", 412, td_small.size, NULL, NULL, POOL_NEVER_FREE);
    td_large.pool = zt_mem_pool_init("large_pool", 100, td_large.size, NULL, NULL, POOL_NEVER_FREE);

    timetest("pool_alloc_small", NPLOOP, test_pool, (void *)&td_small);
    timetest("pool_alloc_large", NPLOOP, test_pool, (void *)&td_large);

    zt_mem_pools_display(0, DISPLAY_POOL_HEADER_ONLY);

    zt_mem_pool_destroy(&td_small.pool);
    zt_mem_pool_destroy(&td_large.pool);
    free(parr);
    return 0;
}
