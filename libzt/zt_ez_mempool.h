#ifndef _ZT_EZ_MEMPOOL_H_
#define _ZT_EZ_MEMPOOL_H_

typedef struct zt_ez_mempool zt_ez_mempool;
typedef struct zt_ez_mempool_chunk zt_ez_mempool_chunk;

struct zt_ez_mempool_chunk {
    void                *data;
    size_t               sz;
    zt_ez_mempool_chunk *next;
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
 * @return a chunk of allocated memory
 */
void *zt_ez_mempool_alloc(zt_ez_mempool *pool, size_t size);

/**
 * Destroy a memory pool (and all of its children)
 * @param pool the memory pool
 * @returns nothing
 */
void zt_ez_mempool_destroy(zt_ez_mempool *pool);
#endif
