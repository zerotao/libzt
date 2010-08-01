/*
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 *
 */

/*
 * Description:
 */
#include <stdio.h>
#include <string.h>
#include <libzt/zt.h>
#include <libzt/zt_mem.h>

struct pool_test {
    int one;
    int two;
    int three;
};

#define NPTEST 100

#define NSIZE(x) (sizeof(char) * x)
zt_mem_pool_desc pool_group[] = {
    { "64",   0, NSIZE(64),   0, 0, 0 },
    { "128",  0, NSIZE(128),  0, 0, 0 },
    { "256",  0, NSIZE(256),  0, 0, 0 },
    { "512",  0, NSIZE(512),  0, 0, 0 },
    { "1024", 0, NSIZE(1024), 0, 0, 0 },
    { "2048", 0, NSIZE(2048), 0, 0, 0 },
    { "4096", 0, NSIZE(4096), 0, 0, 0 },
    { 0,      0, 0,           0, 0, 0 }
};

int
main(int argc, char *argv[])
{
    struct zt_mem_heap       * heap;
    struct zt_mem_pool       * pool;
    struct zt_mem_pool       * tpool;
    struct zt_mem_pool_group * group;
    struct pool_test         * data[NPTEST];
    int                        i;

    heap = zt_mem_heap_init("testheap", 1024 * sizeof(char));

    if (heap) {
        printf("%s\n", zt_mem_heap_get_name(heap));
    } else {
        printf("failed to alloc a heap\n");
        return -1;
    }

    group = zt_mem_pool_group_init(pool_group, sizeof_array(pool_group) - 1);
    if (!group) {
        printf("group alloc failed");
    } else {
        void * elt;
        void * elt2;
        void * elt3;

        elt = zt_mem_pool_group_alloc(group, sizeof(char) * 257);
        zt_mem_pool_group_display(0, group, 0);

        elt2 = zt_mem_pool_group_alloc(group, sizeof(char));

        DO_TRY {
            elt3 = zt_mem_pool_group_alloc(group, sizeof(char) * 4097);
        } ELSE_TRY {
            CATCH(zt_exception.memory.pool.group, {});
        } END_TRY;

        zt_mem_pool_group_display(0, group, 0);

        zt_mem_pool_group_release(&elt);
        zt_mem_pool_group_release(&elt2);
        zt_mem_pool_group_destroy(group);
    }

    tpool = zt_mem_pool_init("testpool", 4092, sizeof(struct pool_test), NULL, NULL, 0);

    /* element size is > normal system page size*/
    pool = zt_mem_pool_init("pool2", 3, sizeof(struct pool_test) + 4092, NULL, NULL, 0);
    printf("*INIT*\n");

    printf("pool lookup: ");
    if (tpool == zt_mem_pool_get("testpool")) {
        printf("success\n");
    } else {
        printf("failed\n");
    }

    zt_mem_pools_display(0, DISPLAY_POOL_HEADER_ONLY);

    for (i = 0; i < NPTEST; i++) {
        data[i] = zt_mem_pool_alloc(pool);
        memset(data[i], 0, sizeof(struct pool_test)+4092);
        data[i]->one = i;
        data[i]->two = i * 100;
        data[i]->three = i * 1000;
    }

    printf("*ALLOC*\n");
    zt_mem_pools_display(0, DISPLAY_POOL_HEADER_ONLY);

    for (i = 0; i < NPTEST; i++) {
        void *dp = data[i];
        zt_mem_pool_release(&(dp));
    }

    printf("*RELEASE*\n");
    zt_mem_pool_destroy(&tpool);
    zt_mem_pools_display(0, DISPLAY_POOL_HEADER_ONLY);

    zt_mem_pool_destroy(&pool);
    zt_mem_pools_display(0, DISPLAY_POOL_HEADER_ONLY);

    zt_mem_heap_destroy(&heap);
    return 0;
} /* main */

