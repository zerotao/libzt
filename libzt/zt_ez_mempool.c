#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "zt_ez_mempool.h"

static void
chunk_append(zt_ez_mempool_chunk **head,
             zt_ez_mempool_chunk  *chunk) {
    if (*head == NULL) {
        *head = chunk;
        return;
    }

    chunk->next = *head;
    *head = chunk;
}

static zt_ez_mempool_chunk *
zt_ez_mempool_chunk_init(const size_t size, zt_ez_mempool_free_cb free_cb) {
    zt_ez_mempool_chunk *chunk;

    chunk       = malloc(sizeof(zt_ez_mempool_chunk));

    chunk->sz      = size;
    chunk->data    = calloc(size, 1);
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

    pool = calloc(sizeof(zt_ez_mempool), 1);

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

        tofree    = chunk_ptr;

	if (chunk_ptr->free_cb) {
	    chunk_ptr->free_cb(chunk_ptr->data);
	} else {
	    free(chunk_ptr->data);
	}

        chunk_ptr = chunk_ptr->next;
        free(tofree);
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

    free(pool);
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

int
zt_ez_mempool_add_buffer(zt_ez_mempool *pool, void *data, size_t size,
	zt_ez_mempool_free_cb free_cb) {
    zt_ez_mempool_chunk *chunk;

    if (data == NULL || size == 0) {
        return -1;
    }

    if (!(chunk = malloc(sizeof(zt_ez_mempool_chunk)))) {
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


#ifdef TEST_EZ_MEMPOOL_MAIN 
struct test_data {
    char *data1;
    char *data2;
    char *data3;
};

void test_special_free(void *data) {
    struct test_data *tdata;

   tdata = (struct test_data *)data; 
   free(tdata->data1);
   free(tdata->data2);
   free(tdata->data3);
   free(tdata);
}

int main(int argc, char **argv) {
    struct test_data *tdata;
    zt_ez_mempool *main_pool;
    zt_ez_mempool *subpool;
    char          *data;

    main_pool = zt_ez_mempool_init(NULL);
    subpool   = zt_ez_mempool_init(main_pool);


    zt_ez_mempool_alloc(main_pool, 1024, NULL);
    zt_ez_mempool_alloc(subpool, 1024, NULL);
    zt_ez_mempool_alloc(subpool, 1024, NULL);
    zt_ez_mempool_alloc(subpool, 1024, NULL);
    zt_ez_mempool_alloc(subpool, 1024, NULL);
    zt_ez_mempool_alloc(subpool, 1024, NULL);

    subpool = zt_ez_mempool_init(subpool);

    zt_ez_mempool_alloc(subpool, 1024, NULL);
    zt_ez_mempool_alloc(main_pool, 1024, NULL);

    tdata = malloc(sizeof(struct test_data));
    tdata->data1 = strdup("fjkdsl");
    tdata->data2 = strdup("fjkdsl");
    tdata->data3 = strdup("fjkdsl");
    zt_ez_mempool_add_buffer(subpool, tdata, sizeof(*tdata), test_special_free);

    zt_ez_mempool_destroy(main_pool);
    return 0;
}
#endif
