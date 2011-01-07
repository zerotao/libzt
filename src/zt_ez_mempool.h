#include <zt_internal.h>

#ifndef _ZT_EZ_MEMPOOL_H_
#define _ZT_EZ_MEMPOOL_H_


typedef struct zt_ez_mempool zt_ez_mempool;
typedef struct zt_ez_mempool_chunk zt_ez_mempool_chunk;
typedef void (*zt_ez_mempool_free_cb)(void *usrdata);

struct zt_ez_mempool_chunk {
    void                 *data;
    size_t                sz;
    zt_ez_mempool_chunk   *next;
    zt_ez_mempool_free_cb free_cb;
};

struct zt_ez_mempool {
    zt_ez_mempool_chunk *chunks;
    zt_ez_mempool       *subpools;
    zt_ez_mempool       *next;
    zt_ez_mempool       *parent;
};

/**
 * Initialize an ez memory pool
 * @param parent if parent is passed a subpool of parent is created
 * @return a new memory pool
 */
zt_ez_mempool *zt_ez_mempool_init(zt_ez_mempool *parent);

/**
 * Allocate memory from an ez memory pool
 * @param pool the memory pool to allocate from
 * @parem size the size to allocate
 * @param an optional callback to free this data
 * @return a chunk of allocated memory
 */
void *zt_ez_mempool_alloc(zt_ez_mempool *pool, size_t size, zt_ez_mempool_free_cb);

/**
 * Destroy a memory pool (and all of its children)
 * @param pool the memory pool
 * @returns nothing
 */
void zt_ez_mempool_destroy(zt_ez_mempool *pool);

/**
 * Add an already-allocated piece of memory to a pool
 * @param pool the memory pool
 * @param data the pointer to an allocated chunk of memory
 * @param size of the data
 * @param an optional callback to free this data
 * @return 0 on success, -1 on error
 */
int zt_ez_mempool_add_buffer(zt_ez_mempool *, void *, size_t, zt_ez_mempool_free_cb);

#endif
