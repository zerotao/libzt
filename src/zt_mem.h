/*!
 * Filename: zt_mem.h
 * Description:
 *      mem_tools defines a group of functions for managing memory as
 *      heaps, pools(or slabs), and sets.
 *
 *      a heap is mearly awrapper around malloc and are the basis for pools
 *
 *      pools are page oriented allocators they are intended to be used when
 *      there are many allocations of the same size.  They provide some
 *      referential locality and vast speedups, up to an order of magnitude,
 *      when they have been tuned properly.  They also provide a framework for
 *      gathering statistics of system use.
 *
 *      pool groups provide an interface to a group of different sized poold of
 *      different sizes that can be treated as a single entity a request from
 *      this group will provide an allocation from the pool whose elements are
 *      the closest size to the request.
 *
 *      sets provide a mechanism for releasing a number of associated memory
 *      allocations all at once.  They are used when you allocate a number of
 *      items from pools (either the same pool or various pools) and want them
 *      to be released at the same time.
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_MEM_H_
#define _ZT_MEM_H_

#include <stdlib.h>

#include <zt_internal.h>

#define POOL_NEVER_FREE    1

/* forward declarations */
typedef struct zt_mem_heap zt_mem_heap;
typedef struct zt_mem_pool zt_mem_pool;
typedef struct zt_mem_set zt_mem_set;
typedef struct zt_mem_pool_group zt_mem_pool_group;

/*!
 * Name: zt_page_release_test
 * Description: callback function that can be set on a pool.  It will be called whenever a page is about to be released.
 *
 */
typedef int (*zt_page_release_test)(size_t total_pages,
                                    size_t free_pages,
                                    size_t cache_size,
                                    int flags,
                                    void *cb_data);

typedef struct zt_mem_pool_desc zt_mem_pool_desc;
struct zt_mem_pool_desc {
    char               * name;
    size_t               elts;
    size_t               size;
    zt_page_release_test release_test;
    void               * cb_data;
    int                  flags;
};

typedef struct zt_mem_pool_stats zt_mem_pool_stats;
struct zt_mem_pool_stats {
    size_t    requested_elts;
    size_t    actual_elts;
    size_t    cached_pages;
    size_t    free_pages;
    size_t    pages;
    size_t    elts_per_page;
    size_t    elt_size;
    size_t    page_size;
    size_t    page_allocs;
    size_t    page_frees;
    ssize_t   flags;
};

zt_mem_heap *zt_mem_heap_init(char *name, size_t size);
void zt_mem_heap_destroy(zt_mem_heap **heap);
void *zt_mem_heap_get_data(zt_mem_heap *heap);
char *zt_mem_heap_get_name(zt_mem_heap *heap);

/************ Pools ************/
zt_mem_pool * zt_mem_pool_init(char * name,
                               ssize_t elts,
                               size_t size,
                               zt_page_release_test release_test,
                               void * cb_data,
                               int flags);

void * zt_mem_pool_alloc(zt_mem_pool *pool);
void zt_mem_pool_release(void **data);
int zt_mem_pool_release_free_pages(zt_mem_pool *pool);
int zt_mem_pool_destroy(zt_mem_pool **pool);
zt_mem_pool_group * zt_mem_pool_group_init(zt_mem_pool_desc * group, size_t len);
void *zt_mem_pool_group_alloc(zt_mem_pool_group * group, size_t size);
#define zt_mem_pool_group_release zt_mem_pool_release
/* void zt_mem_pool_group_release(void **data); */
int zt_mem_pool_group_destroy(zt_mem_pool_group *group);
#define DISPLAY_POOL_HEADER_ONLY 0
#define DISPLAY_POOL_FREE_LIST      1
#define DISPLAY_POOL_PAGES     2
#define DISPLAY_POOL_ALL (DISPLAY_POOL_FREE_LIST | DISPLAY_POOL_PAGES)
void zt_mem_pool_display(int offset, zt_mem_pool *pool, int flags);
void zt_mem_pool_group_display(int offset, zt_mem_pool_group *group, int flags);
void zt_mem_pools_display(int offset, int flags);
zt_mem_pool * zt_mem_pool_get(char *name);
int zt_mem_pool_get_stats(zt_mem_pool *pool, zt_mem_pool_stats *stats);

/************ Sets ************/
zt_mem_set * zt_mem_set_init(char *name);
int zt_mem_set_add(zt_mem_set *set, void *d);
int zt_mem_set_release(zt_mem_set *set);

#endif    /* _ZT_MEM_H_ */
