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

#define DISPLAY_POOL_HEADER_ONLY 0
#define DISPLAY_POOL_FREE_LIST 	 1
#define DISPLAY_POOL_PAGES	 2

#define DISPLAY_POOL_ALL (DISPLAY_POOL_FREE_LIST | DISPLAY_POOL_PAGES)

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

/*
 * Heaps
 */
zt_mem_heap *
zt_mem_heap_init(char *, size_t);

void
zt_mem_heap_destroy(zt_mem_heap **);

void *
zt_mem_heap_get_data(zt_mem_heap *);

char *
zt_mem_heap_get_name(zt_mem_heap *);

/*
 * Pools
 */


typedef int(*zt_page_release_test)(int,int,int,int,void*);
/* f
 * initialize a new pool
 */
zt_mem_pool
* zt_mem_pool_init(char *,
		   long,
		   size_t,
		   zt_page_release_test,
		   void*,
		   int);

/* f
 * allocate memory from a pool
 */
void *
zt_mem_pool_alloc(zt_mem_pool *);

/* f
 * release memory back to a pool
 */
void
zt_mem_pool_release(void **);

/*
 * release free pages
 */
int
zt_mem_pool_release_free_pages(zt_mem_pool *);

/*
 * destroy a pool
 */
int
zt_mem_pool_destroy(zt_mem_pool **);

/* f
 * display one pool
 */
void
zt_mem_pool_display(int, zt_mem_pool *, int);

/* f
 * display all pools
 */
void
zt_mem_pools_display(int, int);

/*
 * Sets
 */
zt_mem_set *
zt_mem_set_init(char *name);

int
zt_mem_set_add(zt_mem_set *, void *);

int
zt_mem_set_release(zt_mem_set *);

#endif	/* _ZT_MEM_H_ */
