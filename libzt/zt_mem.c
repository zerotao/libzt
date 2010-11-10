#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "zt_internal.h"
#include "adt/zt_list.h"
#include "zt_mem.h"
#include "zt_atexit.h"

/*
 * FIXME: threads will need these wrapped
 */
static struct {
    void *(* alloc)(size_t);
    void  (* dealloc)(void *);
    void *(* realloc)(void *, size_t);
} GLOBAL_zmt = { malloc, free, realloc };

static zt_elist(pools);
static zt_elist(sets);
static int mem_atexit = 0;

/* static long x_sys_page_size = 0; */
typedef struct zt_mem_elt zt_mem_elt;
struct zt_mem_elt {
    zt_elist_t           free_elt_list;
    struct zt_mem_page * parent_page;
    unsigned long        data[];
};

struct zt_mem_heap {
    char        * name;
    size_t        size;
    unsigned long heap[];
};

typedef struct zt_mem_page zt_mem_page;
struct zt_mem_page {
    zt_elist_t           page_list;
    struct zt_mem_pool * parent_pool;
    unsigned long        num_free_elts;
    unsigned long        data[];
};

struct zt_mem_pool {
    zt_elist_t           pool_list;
    char               * name;
    size_t                 rcache; /* requested elements in cache */
    size_t                 ncache; /* actual elements in cache */
    size_t                 pcache; /* pages in cache */
    size_t                 nfree_pages; /* number of free pages */
    size_t                 npages; /* total pages */
    size_t                 elts_per_page; /* elements per page */
    size_t               elt_size; /* element size */
    size_t               page_size; /* page size */
    size_t                 page_allocs; /* number of page allocs */
    size_t                 page_frees; /* number of page frees */
    zt_page_release_test release_test_cb;
    void               * release_test_cb_data;
    int                  flags;
    zt_elist_t           page_list;
    zt_elist_t           free_elt_list;
};

struct zt_mem_pool_group {
    zt_elist_t    group_list;
    size_t        npools;
    zt_mem_pool **pools;
};

struct zt_mem_set_elt {
    zt_elist_t    elt_list;
    void        * elt;
};

struct zt_mem_set {
    zt_elist_t    set_list;
    char        * name;
    zt_elist_t    elt_list;
};


/* forward declarations */
static char *zt_mem_strdup(char *str);
static void zt_mem_page_display(int, zt_mem_page *);
static int zt_mem_page_destroy(zt_mem_page *page);
static zt_mem_page *zt_mem_page_alloc(zt_mem_pool *);
static void zt_mem_elt_list_display(int offset, zt_elist_t *head);
static int zt_default_release_test(size_t total_pages, size_t free_pages, size_t cache_size, int flags, void *cb_data);
static void zt_mem_release(void * data);

/* exported functions */
zt_mem_heap*
zt_mem_heap_init(char *name, size_t size)
{
    zt_mem_heap * heap;

    if ((heap = GLOBAL_zmt.alloc(sizeof(zt_mem_heap) + size)) == NULL) {
        return NULL;
    }

    heap->name = zt_mem_strdup(name ? name : "*unknown*");
    heap->size = size;
    return heap;
}

void
zt_mem_heap_destroy(zt_mem_heap **heap)
{
    if (!heap) {
        return;
    }

    GLOBAL_zmt.dealloc((*heap)->name);
    (*heap)->name = 0;
    (*heap)->size = 0;

    GLOBAL_zmt.dealloc(*heap);
    *heap = 0;
}

void *
zt_mem_heap_get_data(zt_mem_heap *heap)
{
    return (void *)heap->heap;
}

char *
zt_mem_heap_get_name(zt_mem_heap *heap)
{
    if (heap) {
        return heap->name;
    }
    return 0;
}

static int
x_calculate_page_data(ssize_t elts, size_t size, size_t *page_size, size_t *epp, size_t *npages)
{
    static size_t sys_page_size = 0;
    size_t        usable_page_size = 0;

    if (sys_page_size == 0) {
        sys_page_size = sysconf(_SC_PAGESIZE);
    }

    usable_page_size = sys_page_size - sizeof(zt_mem_page);

    /* This algorithm is pretty stupid it mearly tries to
     * calculate if I can fit a reasonable number of elements into
     * a system memory page.
     * if (the element size is < 1/2 (system page size - overhead))
     *   use a system page for page size
     * else
     *   page size is elts * size + overhead
     */
    if (size < (usable_page_size / 2)) {
        *epp = usable_page_size / (sizeof(zt_mem_elt) + size);
        *page_size = sys_page_size;
        if (elts > (ssize_t)*epp) {
            *npages = elts / *epp;
        } else {
            *npages = 1;
        }
    } else {
        *epp = elts;
        *page_size = (elts * (sizeof(zt_mem_elt) + size)) + sizeof(zt_mem_page);
        *npages = 1;
    }

    return 0;
}

zt_mem_pool*
zt_mem_pool_init(char *name, ssize_t elts,
                 size_t size, zt_page_release_test release_test,
                 void *cb_data, int flags)
{
    zt_mem_pool * pool;
    zt_mem_page * page;
    size_t        epp;
    size_t        npage_size;
    size_t        npages;
    size_t        pcache;

    x_calculate_page_data(elts ? elts : 1, size, &npage_size, &epp, &npages);

    if (!mem_atexit) {
        zt_atexit(zt_mem_release, NULL);
        mem_atexit = 1;
    }

    if (elts > 0) {
        if (elts < (ssize_t)epp) {
            pcache = 1;
        } else {
            ssize_t tmp = elts % epp;
            pcache = elts / epp;
            if (tmp > 0) {
                pcache++;
            }
        }
    } else {
        /* will be 1 with the current algorithm */
        pcache = npages;
    }

    if ((pool = GLOBAL_zmt.alloc(sizeof(zt_mem_pool))) == 0) {
        return NULL;
    }

    zt_elist_reset(&pool->pool_list);

    pool->name = zt_mem_strdup(name ? name : "*unknown*");
    pool->rcache = elts;
    pool->ncache = pcache * epp;
    pool->pcache = pcache;
    /* > 0 ? cache : 0; */ /* cache should never be negative*/
    pool->nfree_pages = 0;
    pool->npages = 0;
    /* pool->elts_per_page = elts; */
    pool->elts_per_page = epp;
    pool->elt_size = size;
    pool->page_size = npage_size;
    pool->page_allocs = 0;
    pool->page_frees = 0;
    pool->release_test_cb = release_test ? release_test : zt_default_release_test;
    pool->release_test_cb_data = cb_data ? cb_data : NULL;
    pool->flags = flags;

    /*     printf("page size calculated: %ld\npage size actual: %ld\n", */
    /*            pool->page_size, */
    /*            sizeof(zt_mem_page) + (pool->elts_per_page * (sizeof(zt_mem_elt) + pool->elt_size))); */

    zt_elist_reset(&pool->page_list);
    zt_elist_reset(&pool->free_elt_list);

    if (npages > 0) {
        /* fill the cache */
        while (npages--) {
            page = zt_mem_page_alloc(pool);
            zt_elist_add_tail(&pool->page_list, &page->page_list);
            pool->page_allocs++;
        }
    }

    zt_elist_add_tail(&pools, &pool->pool_list);
    return pool;
} /* zt_mem_pool_init */

void *
zt_mem_pool_alloc(zt_mem_pool *pool)
{
    zt_elist_t * tmp;
    zt_mem_elt * elt;

    if (pool == 0) {
        return 0;
    }

    /* if the free elt list is empty get a new page */
    if (zt_elist_empty(&pool->free_elt_list) == 1) {
        /* alloc a new page */
        zt_mem_page *page;
        page = zt_mem_page_alloc(pool);
        /*  */
        zt_elist_add_tail(&pool->page_list, &page->page_list);
        pool->page_allocs++;
    }

    tmp = zt_elist_get_next(&pool->free_elt_list);
    zt_elist_remove(tmp);
    elt = zt_elist_data(tmp, zt_mem_elt, free_elt_list);

    --elt->parent_page->num_free_elts;

    /* if we have used the first elt then decrement the free pages */
    if (elt->parent_page->num_free_elts == pool->elts_per_page - 1) {
        --pool->nfree_pages;
    }

    return (void *)&elt->data;
}

void
zt_mem_pool_release(void **data)
{
    zt_mem_elt  * elt;
    zt_mem_pool * pool;
    zt_mem_page * page;

    elt = zt_elist_data(*data, zt_mem_elt, data);
    page = elt->parent_page;

    if (page == NULL) {
        /* this element was allocated by XALLOC and should just be freed */
        XFREE(*data);
        return;
    }

    pool = page->parent_pool;

    /* add the element to the pools free elt list */
    page->num_free_elts++;
    zt_elist_add(&pool->free_elt_list, &elt->free_elt_list);

    /* if all of the pages elements are free */
    if (page->num_free_elts == pool->elts_per_page) {
        pool->nfree_pages++;
    }

    if (page->num_free_elts == pool->elts_per_page) {
        if (pool->release_test_cb(pool->rcache,
                                  pool->ncache * pool->nfree_pages,
                                  pool->ncache * (pool->npages - pool->nfree_pages),
                                  pool->flags,
                                  pool->release_test_cb_data)) {
            /* release the page */
            zt_mem_page_destroy(page);
            pool->page_frees++;
        }
    }
}

int
zt_mem_pool_release_free_pages(zt_mem_pool *pool)
{
    zt_elist_t  * pages;
    zt_elist_t  * tpage;
    zt_mem_page * page;

    zt_elist_for_each_safe(&(pool)->page_list, pages, tpage) {
        page = zt_elist_data(pages, zt_mem_page, page_list);

        if (page->num_free_elts == pool->elts_per_page) {
            zt_mem_page_destroy(page);
        }
    }

    return 0;
}


int
zt_mem_pool_destroy(zt_mem_pool **pool)
{
    zt_elist_t  * tmp;
    zt_elist_t  * ptmp;
    zt_mem_page * page;

    zt_elist_for_each_safe(&(*pool)->page_list, tmp, ptmp) {
        page = zt_elist_data(tmp, zt_mem_page, page_list);

        /* sanity checking */
        if (page->num_free_elts < (*pool)->elts_per_page) {
            return -1;
        } else {
            zt_mem_page_destroy(page);
        }
    }

    zt_elist_remove(&(*pool)->pool_list);

    GLOBAL_zmt.dealloc((*pool)->name);
    (*pool)->name = NULL;

    GLOBAL_zmt.dealloc((*pool));
    *pool = NULL;

    return 0;
}

int
zt_mem_pool_get_stats(zt_mem_pool *pool, zt_mem_pool_stats *stats)
{
    if (!pool || !stats) {
        return ZT_FAIL;
    }

    stats->requested_elts = pool->rcache;
    stats->actual_elts = pool->ncache;
    stats->cached_pages = pool->pcache;
    stats->free_pages = pool->nfree_pages;
    stats->pages = pool->npages;
    stats->elts_per_page = pool->elts_per_page;
    stats->elt_size = pool->elt_size;
    stats->page_size = pool->page_size;
    stats->page_allocs = pool->page_allocs;
    stats->page_frees = pool->page_frees;
    stats->flags = pool->flags;

    return ZT_PASS;
}

void
zt_mem_pool_display(int offset, zt_mem_pool *pool, int flags)
{
    zt_elist_t  * tmp;
    zt_mem_page * page;
    long          felts = 0;

    zt_elist_for_each(&pool->page_list, tmp) {
        page = zt_elist_data(tmp, zt_mem_page, page_list);
        felts += page->num_free_elts;
    }

    printf(BLANK "pool: \"%s\" [%p] {\n"
           BLANK "elements: {\n"
           BLANK "elt cache requested: %ld elements\n"
           BLANK "elt cache actual: %ld elements\n"
           BLANK "total elts: %ld\n"
           BLANK "free elts: %ld\n"
           BLANK "}\n"
           BLANK "pages: {\n"
           BLANK "page cache: %ld page(s)\n"
           BLANK "free pages: %ld\n"
           BLANK "page size: %ld\n"
           BLANK "pages in use: %ld\n"
           BLANK "total pages: %ld\n"
           BLANK "elements per page: %ld\n"
           BLANK "page allocs: %ld\n"
           BLANK "page frees: %ld\n"
           BLANK "}\n"
           BLANK "overall: {\n"
           BLANK "element size: %ld bytes + overhead = %ld bytes \n"
           BLANK "page usage (num elts * elt size): %ld bytes\n"
           BLANK "page memory (page size * total pages): %ld bytes\n"
           BLANK "}\n",
           INDENT(offset), pool->name, (void *)pool,
           INDENT(offset + 1),
           INDENT(offset + 2), pool->rcache,
           INDENT(offset + 2), pool->ncache,
           INDENT(offset + 2), (pool->elts_per_page * pool->npages),
           INDENT(offset + 2), felts,
           INDENT(offset + 1), INDENT(offset + 1),
           INDENT(offset + 2), pool->pcache,
           INDENT(offset + 2), pool->nfree_pages,
           INDENT(offset + 2), pool->page_size,
           INDENT(offset + 2), (pool->npages - pool->nfree_pages),
           INDENT(offset + 2), pool->npages,
           INDENT(offset + 2), pool->elts_per_page,
           INDENT(offset + 2), pool->page_allocs,
           INDENT(offset + 2), pool->page_frees,
           INDENT(offset + 1), INDENT(offset + 1),
           INDENT(offset + 2), (long)pool->elt_size, (unsigned long)(sizeof(zt_mem_elt) + pool->elt_size),
           INDENT(offset + 2), (sizeof(zt_mem_elt) + pool->elt_size) * pool->elts_per_page,
           INDENT(offset + 2), pool->page_size * pool->npages,
           INDENT(offset + 1));

    if (flags & DISPLAY_POOL_FREE_LIST) {
        printf(BLANK "free_list {\n", INDENT(offset + 1));
        zt_mem_elt_list_display(offset + 2, &pool->free_elt_list);
        printf("\n" BLANK "}\n", INDENT(offset + 1));
    }

    if (flags & DISPLAY_POOL_PAGES) {
        printf(BLANK "pages {\n", INDENT(offset + 1));
        zt_elist_for_each(&pool->page_list, tmp) {
            page = zt_elist_data(tmp, zt_mem_page, page_list);
            zt_mem_page_display(offset + 2, page);
        }
        printf(BLANK "}\n", INDENT(offset + 1));
    }

    printf(BLANK "}\n", INDENT(offset));
} /* zt_mem_pool_display */

void
zt_mem_pools_display(int offset, int flags)
{
    zt_elist_t  * tmp;
    zt_mem_pool * pool;

    printf(BLANK "Pools { \n", INDENT(offset));
    zt_elist_for_each(&pools, tmp) {
        pool = zt_elist_data(tmp, zt_mem_pool, pool_list);
        zt_mem_pool_display(offset + 1, pool, flags);
    }
    printf(BLANK "}\n", INDENT(offset));
}

static void
zt_mem_release(void * data UNUSED)
{
    zt_elist_t  * tmp;
    zt_elist_t  * tmp2;
    zt_mem_pool * pool;

    zt_elist_for_each_safe(&pools, tmp, tmp2) {
        pool = zt_elist_data(tmp, zt_mem_pool, pool_list);
        zt_mem_pool_destroy(&pool);
    }
}

void
zt_mem_pool_group_display(int offset, zt_mem_pool_group *group, int flags)
{
    size_t len;
    size_t i;

    len = group->npools;

    printf(BLANK "Group {\n", INDENT(offset));
    for (i = 0; i < len; i++) {
        zt_mem_pool_display(offset + 1, group->pools[i], flags);
    }
    printf(BLANK "}\n", INDENT(offset));
}


zt_mem_pool *
zt_mem_pool_get(char *name)
{
    size_t       nlen;
    zt_elist_t * tmp;

    if (!name) {
        return 0;
    }

    nlen = strlen(name);

    zt_elist_for_each(&pools, tmp) {
        zt_mem_pool * pool;
        size_t        olen;

        pool = zt_elist_data(tmp, zt_mem_pool, pool_list);
        olen = strlen(pool->name);

        if (olen == nlen && (strncmp(name, pool->name, nlen) == 0)) {
            return pool;
        }
    }

    return 0;
}

zt_mem_pool_group *
zt_mem_pool_group_init(zt_mem_pool_desc *group, size_t len)
{
    zt_mem_pool_group * ngroup;
    size_t              i;

    if ((ngroup = XCALLOC(zt_mem_pool_group, 1)) == NULL) {
        return 0;
    }

    if ((ngroup->pools = XCALLOC(zt_mem_pool *, len)) == NULL) {
        return 0;
    }
    zt_elist_reset(&ngroup->group_list);
    ngroup->npools = len;

    for (i = 0; i < len && group[i].name; i++) {
        ngroup->pools[i] = zt_mem_pool_init(group[i].name,
                                            group[i].elts,
                                            group[i].size,
                                            group[i].release_test,
                                            group[i].cb_data,
                                            group[i].flags);
        if (!ngroup->pools[i]) {
            while (--i) {
                zt_mem_pool_destroy(&ngroup->pools[i]);
            }
            return 0;
        }
    }

    return ngroup;
}

void *
zt_mem_pool_group_alloc(zt_mem_pool_group *group, size_t size)
{
    size_t    len;
    size_t    i;

    len = group->npools;

    for (i = 0; i < len; i++) {
        if (size <= group->pools[i]->elt_size) {
            return zt_mem_pool_alloc(group->pools[i]);
        }
    }

    TRY_THROW(zt_exception.memory.pool.group.does_not_exist);
    return NULL;                                           /* never reached if exceptions are enabled */
}

int
zt_mem_pool_group_destroy(zt_mem_pool_group * group)
{
    size_t len;
    size_t i;
    int ret = 0;

    len = group->npools;

    for (i = 0; i < len; i++) {
        if (zt_mem_pool_destroy(&group->pools[i]) != 0) {
            ret = -1;
        }
    }

    XFREE(group->pools);
    XFREE(group);
    return ret;
}

zt_mem_set *
zt_mem_set_init(char *name)
{
    zt_mem_set * set;

    if ((set = GLOBAL_zmt.alloc(sizeof(zt_mem_set))) == 0) {
        return NULL;
    }

    zt_elist_reset(&set->set_list);
    zt_elist_reset(&set->elt_list);

    set->name = zt_mem_strdup(name ? name : "*unknown*");

    return set;
}

int
zt_mem_set_add(zt_mem_set *set UNUSED, void *d UNUSED)
{
    /* struct zt_mem_set_elt    * elt; */

    /* alloc a new elt wrapper
     * assign d to the data element
     * return true
     */
    return -1;
}

int
zt_mem_set_release(zt_mem_set *set UNUSED)
{
    /*
     * release the elt and release the wrapper
     */

    return 0;
}


/* static functions  */
static char *
zt_mem_strdup(char *str)
{
    char * tmp = "*unknown*";

    if (str) {
        size_t len = strlen(str);
        tmp = GLOBAL_zmt.alloc(len + 1 * sizeof(char));
        memcpy(tmp, str, len);
        tmp[len] = '\0';
    }
    return tmp;
}

static int
zt_mem_page_destroy(zt_mem_page *page)
{
    zt_mem_elt * elt;
    unsigned long i;
    size_t       size;

    if (page->num_free_elts < page->parent_pool->elts_per_page) {
        printf("error: %s called when elements are still in use!\n", __FUNCTION__);
        return 1;
    }

    page->parent_pool->npages--;
    page->parent_pool->nfree_pages--;

    size = sizeof(zt_mem_elt) + page->parent_pool->elt_size;
    elt = (zt_mem_elt *)page->data;
    /* remove the elements from the free element list */
    for (i = 0; i < page->num_free_elts; i++) {
        if (zt_elist_empty(&elt->free_elt_list) == 0) {
            zt_elist_remove(&elt->free_elt_list);
        }
        elt = (zt_mem_elt *)((unsigned long)elt + size);
    }
    /* remove the page from the page list */
    if (zt_elist_empty(&page->page_list) == 0) {
        zt_elist_remove(&page->page_list);
    }
    GLOBAL_zmt.dealloc(page);
    return 0;
}

static zt_mem_page *
zt_mem_page_alloc(zt_mem_pool *pool)
{
    zt_mem_page * page;
    zt_mem_elt  * head;
    zt_mem_elt  * elt;
    size_t        size;
    size_t        i;
    size_t        epp;

    size = sizeof(zt_mem_elt) + pool->elt_size;
    /* sizeof(zt_mem_page) + (pool->elts_per_page * size)) */

    if ((page = GLOBAL_zmt.alloc(pool->page_size)) == 0) {
        return 0;
    }

    page->num_free_elts = 0;
    page->parent_pool = pool;
    zt_elist_reset(&page->page_list);

    /* pointer to the first element */
    head = (zt_mem_elt *)page->data;

    /* add the first element to the free list*/
    zt_elist_add_tail(&pool->free_elt_list, &head->free_elt_list);
    head->parent_page = page;
    page->num_free_elts++;

    epp = pool->elts_per_page;
    elt = head;
    for (i = 1; i < epp; i++) {
        elt = (zt_mem_elt *)((unsigned long)elt + size);
        zt_elist_reset(&elt->free_elt_list);
        elt->parent_page = page;
        zt_elist_add_tail(&pool->free_elt_list, &elt->free_elt_list);
        page->num_free_elts++;
    }
    pool->npages++;
    pool->nfree_pages++;
    return page;
}

static void
zt_mem_elt_list_display(int offset, zt_elist_t *head)
{
    zt_elist_t * tmp;
    zt_mem_elt * elt;
    int          i;

    i = 0;
    zt_elist_for_each(head, tmp) {
        elt = zt_elist_data(tmp, zt_mem_elt, free_elt_list);
        if (i) {
            printf("\n");
        }
        printf(BLANK "elt: %p  parent_page: %p  data: %p", INDENT(offset), (void *)elt, (void *)elt->parent_page, (void *)elt->data);
        i = 1;
    }
}

/* static void */
static void
zt_mem_page_display(int offset, zt_mem_page *page)
{
    printf(BLANK "page: %p {\n", INDENT(offset), (void *)page);
    printf(BLANK "num_free_elts: %ld\n", INDENT(offset + 1), page->num_free_elts);
    printf(BLANK "parent pool: %p\n", INDENT(offset + 1), (void *)page->parent_pool);
    printf(BLANK "}\n", INDENT(offset));
}


static int
zt_default_release_test(size_t req_elts UNUSED, size_t free_elts UNUSED, size_t used_elts UNUSED, int flags, void *cb_data UNUSED)
{
    if (flags & POOL_NEVER_FREE) {
        return 0;
    }
    return 1;
}
