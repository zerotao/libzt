#ifndef _ZT_MEM_H_
#define _ZT_MEM_H_

/*
 * mem_tools defines a group of functions for managing memory as
 * heaps, pools(or slabs), and sets.
 *
 * a heap is mearly awrapper around malloc and are the basis for pools
 * 
 * pools are page oriented allocators they are intended to be used
 * when there are many allocations of the same size.  They provide some
 * referential locality and vast speedups, up to an order of magnitude, when
 * they have been tuned properly.  They also provide a framework for gathering
 * statistics of system use.
 *
 * mem stets provide a group of pools that can be treated as a single entity
 * thus you can have a set of various sizes and a request from that set will
 * provide an allocation from the pool whose elements are the closest size
 * to the request.
 * 
 * ranges provide a mechanism for releasing a number of associated memory allocations
 * all at once.  They are used when you allocate a number of items from pools
 * (either the same pool or various pools) and want them to be released at the same
 * time.
 *
 */


#define POOL_NEVER_FREE		1

/* forward declarations */
typedef struct zt_mem_heap zt_mem_heap;
typedef struct zt_mem_pool zt_mem_pool;
typedef struct zt_mem_set zt_mem_set;

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
	int		  flags;
}zt_mem_pool_stats;

/**
 * Heaps
 */

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
 * Can be called whenever a page is about to be released.
 * @param total_pages total number of pages currently allocated
 * @param free_pages # of currently free pages
 * @param cache_size current size of the page_cache
 * @param flags the mem_pool's flags
 * @param cb_data callback data registered with the pool
 */
typedef int(*zt_page_release_test)(int	  total_pages,
				   int 	  free_pages,
				   int 	  cache_size,
				   int 	  flags,
				   void	* cb_data);

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
zt_mem_pool_init(char			* name,
		 long			  elts,
		 size_t			  size,
		 zt_page_release_test	  release_test,
		 void			* cb_data,
		 int			  flags);

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


#define DISPLAY_POOL_HEADER_ONLY 0
#define DISPLAY_POOL_FREE_LIST 	 1
#define DISPLAY_POOL_PAGES	 2
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
