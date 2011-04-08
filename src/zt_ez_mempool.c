#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "zt.h"
#include "zt_internal.h"

static void
chunk_append(zt_ez_mempool_chunk **head,
             zt_ez_mempool_chunk  *chunk) {
    if (*head == NULL) {
        *head = chunk;
        return;
    }

    chunk->next = *head;
    *head       = chunk;
}

static zt_ez_mempool_chunk *
zt_ez_mempool_chunk_init(const size_t size, zt_ez_mempool_free_cb free_cb) {
    zt_ez_mempool_chunk *chunk;

    chunk          = zt_malloc(zt_ez_mempool_chunk, 1);

    chunk->sz      = size;
    chunk->data    = zt_callocs(size, 1);
    chunk->free_cb = free_cb;
    chunk->next    = NULL;

    return chunk;
}

static void
pool_append(zt_ez_mempool **head,
            zt_ez_mempool  *pool) {
    if (*head == NULL) {
        *head = pool;
        return;
    }

    pool->next = *head;
    *head      = pool;
}

zt_ez_mempool *
zt_ez_mempool_init(zt_ez_mempool *parent) {
    zt_ez_mempool *pool;

    pool = zt_calloc(zt_ez_mempool, 1);

    if (pool == NULL) {
        return NULL;
    }

    if (parent != NULL) {
        pool_append(&parent->subpools, pool);
        pool->parent = parent;
    }

    return pool;
}

static void
free_chunks(zt_ez_mempool_chunk *chunks) {
    zt_ez_mempool_chunk *chunk_ptr;

    chunk_ptr = chunks;

    while (chunk_ptr != NULL) {
        zt_ez_mempool_chunk *tofree;

        tofree = chunk_ptr;

        if (chunk_ptr->free_cb) {
            chunk_ptr->free_cb(chunk_ptr->data);
        } else {
            zt_free(chunk_ptr->data);
        }

        chunk_ptr = chunk_ptr->next;
        zt_free(tofree);
    }
}

void
zt_ez_mempool_destroy(zt_ez_mempool *pool) {
    zt_ez_mempool *pool_ptr;

    pool_ptr = pool->subpools;

    if (pool->parent != NULL) {
        pool->parent->subpools = NULL;
    }

    free_chunks(pool->chunks);

    while (pool_ptr != NULL) {
        zt_ez_mempool *next = pool_ptr->next;
        zt_ez_mempool_destroy(pool_ptr);
        pool_ptr = next;
    }

    zt_free(pool);
}

void *
zt_ez_mempool_alloc(zt_ez_mempool *pool, size_t size, zt_ez_mempool_free_cb free_cb) {
    zt_ez_mempool_chunk *chunk;

    chunk = zt_ez_mempool_chunk_init(size, free_cb);

    if (chunk == NULL) {
        return NULL;
    }

    chunk_append(&pool->chunks, chunk);

    return chunk->data;
}

char *
zt_ez_mempool_strdup(zt_ez_mempool *pool, const char *s) {
    char *ret = zt_strdup(s);

    zt_ez_mempool_add_buffer(pool, (void *)ret, strlen(ret), free);

    return ret;
}

int
zt_ez_mempool_add_buffer(zt_ez_mempool *pool, void *data, size_t size,
                         zt_ez_mempool_free_cb free_cb) {
    zt_ez_mempool_chunk *chunk;

    if (data == NULL || size == 0) {
        return -1;
    }

    if (!(chunk = zt_malloc(zt_ez_mempool_chunk, 1))) {
        perror("malloc");
        return -1;
    }

    chunk->sz      = size;
    chunk->data    = data;
    chunk->next    = NULL;
    chunk->free_cb = free_cb;

    chunk_append(&pool->chunks, chunk);

    return 0;
}

