/****h* libZT/Memory
 * DESCRIPTION
 *   mem_tools defines a group of functions for managing memory as
 *   heaps, pools(or slabs), and sets.
 *
 *   a heap is mearly awrapper around malloc and are the basis for pools
 * 
 *   pools are page oriented allocators they are intended to be used
 *   when there are many allocations of the same size.  They provide
 *   some referential locality and vast speedups, up to an order of
 *   magnitude, when they have been tuned properly.  They also provide
 *   a framework for gathering statistics of system use.
 *
 *   pool groups provide an interface to a group of different sized
 *   poold of different sizes that can be treated as a single entity a
 *   request from this group will provide an allocation from the pool
 *   whose elements are the closest size to the request.
 * 
 *   sets provide a mechanism for releasing a number of associated
 *   memory allocations all at once.  They are used when you allocate
 *   a number of items from pools (either the same pool or various
 *   pools) and want them to be released at the same time.
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/
#ifndef _ZT_MEM_H_
#define _ZT_MEM_H_

/*
 *
 */
#include <libzt/zt_except.h>
EXCEPT_DESC(zt_mem, "ZT Mem Exceptions",
			EXCEPTION(no_mem, "no memory available")
			EXCEPT_GROUP(pool, "pool exceptions",
						 EXCEPTION(does_not_exist, "pool does not exist")
						 EXCEPT_GROUP(group, "group exceptions",
									  EXCEPTION(does_not_exist, "group does not exist")
						 )));

#undef EXCEPT_DEFINE

/****d* Memory/POOL_NEVER_FREE
 *  NAME
 *    POOL_NEVER_FREE
 *****/
#define POOL_NEVER_FREE		1


/* forward declarations */
typedef struct zt_mem_heap zt_mem_heap;
typedef struct zt_mem_pool zt_mem_pool;
typedef struct zt_mem_set zt_mem_set;
typedef struct zt_mem_pool_group zt_mem_pool_group;

/****f* Memory/zt_page_release_test
 * NAME
 *   zt_page_release_test - Can be called whenever a page is about to be released.
 *
 * INPUTS
 *   o total_pages - total number of pages currently allocated
 *   o free_pages  - # of currently free pages
 *   o cache_size  - current size of the page_cache
 *   o flags       - the mem_pool's flags
 *   o cb_data     - callback data registered with the pool
 ****/
typedef int(*zt_page_release_test)(int	  total_pages,
								   int 	  free_pages,
								   int 	  cache_size,
								   int 	  flags,
								   void	* cb_data);

typedef struct zt_mem_pool_desc {
	char					* name;
	long					  elts;
	size_t					  size;	
	zt_page_release_test	  release_test;
	void					* cb_data;
	int						  flags;
	
} zt_mem_pool_desc;

typedef struct zt_mem_pool_stats {
	long		  requested_elts;
	long		  actual_elts;
	long		  cached_pages;
	long		  free_pages;
	long		  pages;
	long		  elts_per_page;
	size_t		  elt_size;
	size_t		  page_size;
	long		  page_allocs;
	long		  page_frees;
	int		  	  flags;
}zt_mem_pool_stats;

/**
 * Initialize a new heap with name and size.
 * @param name name of the heap
 * @param size size of the heap
 * @return a pointer to a new zt_mem_heap structure
 */
zt_mem_heap *zt_mem_heap_init(char *name, size_t size);

/**
 * Destroy a heap.
 * @param heap ** to a heap
 * @return void
 */
void zt_mem_heap_destroy(zt_mem_heap **heap);

/**
 * Get access to the current heap pointer.
 * @param heap pointer to heap
 * @return void*
 */
void *zt_mem_heap_get_data(zt_mem_heap *heap);

/**
 * Return the name of the heap.
 * @param heap pointer to a heap
 * @return char *
 */
char *zt_mem_heap_get_name(zt_mem_heap *heap);

/**
 * Pools.
 */

/**
 * initialize a new mem pool.
 * @param name name of the pool
 * @param elts estimated number of elts desired per page
 * @param size size of an elt
 * @param release_test callback that is called when a page is about to be released
 * @param cb_data user data to be passed to release_test when called
 * @param flags flags set for the pool
 */
zt_mem_pool *
zt_mem_pool_init(char					* name,
				 long					  elts,
				 size_t					  size,
				 zt_page_release_test	  release_test,
				 void					* cb_data,
				 int					  flags);

/**
 * allocate memory from a pool.
 * @param pool pool to allocate from
 * @return new element from the pool
 */
void *
zt_mem_pool_alloc(zt_mem_pool *pool);

/**
 * release memory back to a pool.
 * @param data address of the data returned from zt_mem_pool_alloc
 * @return void
 */
void
zt_mem_pool_release(void **data);

/**
 * release free pages.
 * @param pool pool from which to release free pages
 * @return 0 on success -1 on error
 */
int
zt_mem_pool_release_free_pages(zt_mem_pool *pool);

/**
 * destroy a pool.
 * @param pool address of the pool returned from zt_mem_pool_init
 * @return 0 on success -1 on error
 */
int
zt_mem_pool_destroy(zt_mem_pool **pool);

/**
 * initialize a new mem pool group.
 * @param group array of zt_mem_pool_desc (NULL ended)
 * @return NULL on failure zt_mem_pool_group * on success
 */
zt_mem_pool_group * zt_mem_pool_group_init(zt_mem_pool_desc * group, int len);

/**
 * allocate memory from a mem pool group.
 * @param group pointer to a mem_pool_group (NULL ended)
 * @param size size of the pointer to allocate
 * @return pointer to a datum at least of size size or NULL
 */
void *zt_mem_pool_group_alloc(zt_mem_pool_group * group, size_t size);

/**
 * release memory allocated with zt_mem_pool_group_alloc.
 * @param data address of the datum allocated with zt_mem_pool_group_alloc
 */
/****d* Memory/zt_mem_pool_group_release
 *  NAME
 *    zt_mem_pool_group_release
 *****/
#define zt_mem_pool_group_release zt_mem_pool_release
/* void zt_mem_pool_group_release(void **data); */

/**
 * destroy a zt_mem_pool_group created by zt_mem_pool_group_init
 * @param group pointer to a zt_mem_pool_group (NULL ended)
 * @return -1 on error 0 on success
 */
int zt_mem_pool_group_destroy(zt_mem_pool_group *group);

/****d* Memory/DISPLAY_POOL_HEADER_ONLY
 *  NAME
 *    DISPLAY_POOL_HEADER_ONLY
 *****/
#define DISPLAY_POOL_HEADER_ONLY 0

/****d* Memory/DISPLAY_POOL_FREE_LIST
 *  NAME
 *    DISPLAY_POOL_FREE_LIST
 *****/
#define DISPLAY_POOL_FREE_LIST 	 1

/****d* Memory/DISPLAY_POOL_PAGES
 *  NAME
 *    DISPLAY_POOL_PAGES
 *****/
#define DISPLAY_POOL_PAGES	 2

/****d* Memory/DISPLAY_POOL_ALL
 *  NAME
 *    DISPLAY_POOL_ALL
 *****/
#define DISPLAY_POOL_ALL (DISPLAY_POOL_FREE_LIST | DISPLAY_POOL_PAGES)

/**
 * display one pool.
 * flags are:
 * DISPLAY_POOL_HEADER_ONLY display only the header
 * DISPLAY_POOL_FREE_LIST display the free elt list
 * DISPLAY_POOL_PAGES display the page information
 * DISPLAY_POOL_ALL both FREE_LIST and PAGES and HEADER.
 *
 * @param offset to begin printing the pool
 * @param pool the pool to display
 * @param flags see above
 * @return void
 */
void
zt_mem_pool_display(int offset, zt_mem_pool *pool, int flags);

/**
 * display one pool group.
 * flags are:
 * DISPLAY_POOL_HEADER_ONLY display only the header
 * DISPLAY_POOL_FREE_LIST display the free elt list
 * DISPLAY_POOL_PAGES display the page information
 * DISPLAY_POOL_ALL both FREE_LIST and PAGES and HEADER.
 *
 * @param offset to begin printing the pool
 * @param group the pool group to display
 * @param flags see above
 * @return void
 */
void
zt_mem_pool_group_display(int offset, zt_mem_pool_group *group, int flags);

/**
 * display all of the pools.
 * 
 * @param offset offset to begin printing the pool
 * @param flags see flags in zt_mem_pool_display()
 * @return void
 * @see zt_mem_pool_display()
 */
void
zt_mem_pools_display(int offset, int flags);


/** 
 * return the mem_pool specified by name.
 * 
 * @param name the name of the mem_pool to return
 * @return the mem_pool named name or 0 if it does not exist
 */
zt_mem_pool *
zt_mem_pool_get(char *name);

/*
 * Sets
 */

/**
 * initialize a mem_set.
 *
 * @param name the name of the set
 * @return a new set
 */
zt_mem_set *
zt_mem_set_init(char *name);

/**
 * add a new pointer to set.
 *
 * @param set the set to add <d> to
 * @param d the data to add to set
 * @return 0 on success -1 on failure 
 */
int
zt_mem_set_add(zt_mem_set *set, void *d);

/**
 * release all pointers in set.
 *
 * @param set the set to release
 * @return 0 on success -1 on failure
 */
int
zt_mem_set_release(zt_mem_set *set);

#endif	/* _ZT_MEM_H_ */
