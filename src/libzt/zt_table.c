/*
 * zt_table.c        ZeroTao Tablees
 *
 * Copyright (C) 2002-2005, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 */
#include <errno.h>
#include "zt.h"
#include "zt_internal.h"


#define DEFAULT_BUCKETS 512
static uint8_t
nbits(size_t val)
{
    uint8_t nbits;

    for (nbits = 1; (val = val >> 1); nbits++) {
        ;
    }
    return nbits;
}

unsigned long powers_of_2[] = { 2,            4,         8,
                                16,           32,        64,
                                128,          256,       512,
                                1024,         2048,      4096,
                                8192,         16384,     32768,
                                65535,        131072,    262144,
                                524288,       1048576,   2097152,
                                4194304,      8388608,   16777216,
                                33554432,     67108864,  134217728,
                                268435456,    536870912, 1073741824,
                                2147483648UL, UINT_MAX };

struct table_node {
    void              * datum;
    size_t              datum_len;
    void              * key;
    size_t              key_len;
    unsigned long       hash_key;
    struct table_node * next;
};

struct zt_table {
    char                * name;
    zt_table_compare_cb   cmp;
    zt_table_hash_func_cb func;
    void                * cdata;
    uint8_t               nbits;
    int                   flags;
    size_t                num_buckets;
    unsigned int          filled;
    zt_mem_pool         * node_pool;
    zt_mem_heap         * table_heap;
    struct table_node   **buckets;
};

#define ZT_DEFAULT_TABLE_POOL "zt.table.pool"

zt_mem_pool *
zt_table_pool_init(long elts)
{
    zt_mem_pool * table_mem_pool;

    table_mem_pool = zt_mem_pool_init(ZT_DEFAULT_TABLE_POOL, elts, sizeof(zt_table), NULL, NULL, 0);

    return table_mem_pool;
}

zt_table *
zt_table_init(char *name, zt_table_hash_func_cb func,
              zt_table_compare_cb cmp, size_t hint, int flags,
              void *cdata)
{
    zt_table    * table;
    size_t        nbuckets = 0;
    char          tname[1024];
    zt_mem_pool * table_mem_pool;


    if ( (func == NULL) ||
         (cmp == NULL)) {
        return NULL;
    }

    table_mem_pool = zt_mem_pool_get(ZT_DEFAULT_TABLE_POOL);

    if (table_mem_pool == NULL) {
        table_mem_pool = zt_table_pool_init(2);
    }

    if ( (table = (zt_table *)zt_mem_pool_alloc(table_mem_pool)) == NULL) {
        return NULL;
    }

    table->flags = flags;
    table->cdata = cdata;

    if (flags & ZT_TABLE_SIZE_USE_HINT) {
        nbuckets = hint;
    } else {
        int i = 0;
        int len = sizeof_array(powers_of_2);

        for (i = 0; i < len; i++) {
            if (powers_of_2[i] > hint) {
                nbuckets = powers_of_2[i];
                break;
            }
        }
    }

    table->nbits = nbits(nbuckets - 1);

    table->table_heap = zt_mem_heap_init(0, nbuckets * sizeof(struct table_node *));

    table->buckets = zt_mem_heap_get_data(table->table_heap);
    memset(table->buckets, 0, nbuckets * sizeof(struct table_node *));

    if (name) {
        table->name = zt_strdup(name);
    } else {
        table->name = zt_strdup("anonymous");
    }


    snprintf(tname, sizeof_array(tname), "%s (table node pool)", name);
    table->node_pool = zt_mem_pool_init(tname, nbuckets,
                                        sizeof(struct table_node), NULL, NULL, 0);

    table->num_buckets = nbuckets;
    table->cmp = cmp;
    table->func = func;

    return table;
} /* zt_table_init */

void
zt_table_destroy(zt_table *h)
{
    size_t    i;

    for (i = 0; i < h->num_buckets; i++) {
        struct table_node *node = h->buckets[i];
        while (node) {
            struct table_node *tmp = node->next;
            zt_mem_pool_release((void **)&node);
            node = tmp;
        }
    }

    if (h->name) {
        zt_free(h->name);
    }

    zt_mem_pool_destroy(&h->node_pool);
    zt_mem_heap_destroy(&h->table_heap);
    zt_mem_pool_release((void **)&h);
}

int
zt_table_set(zt_table *h, const void *key, size_t key_len, const void *datum, size_t datum_len)
{
    unsigned long       nkey = h->func(key, key_len, h->cdata);
    unsigned long       skey = nkey;
    struct table_node * node;
    struct table_node * nn;

    ZT_HASH_SUB32_MASK(nkey, h->nbits);
    /* ZT_LOG_XDEBUG("for key %d hash key is: %d", (int)key, nkey); */

    nn = h->buckets[nkey];

    if (!(h->flags & ZT_TABLE_ALLOW_DUP_KEYS)) {
        while (nn) {
            if (nn->hash_key == skey) {
                if (h->cmp((void *)nn->key, nn->key_len, key, key_len, h->cdata)) {
                    return 1;
                }
            }
            nn = nn->next;
        }
    }

    node = (struct table_node *)zt_mem_pool_alloc(h->node_pool);

    node->next = h->buckets[nkey];
    node->hash_key = skey;
    h->buckets[nkey] = node;
    node->key = (void *)key;
    node->key_len = key_len;
    node->datum = (void *)datum;
    node->datum_len = datum_len;

    return 0;
}

void *
zt_table_get(zt_table *h, const void *key, size_t key_len)
{
    struct table_node *node;
    unsigned long      nkey;
    unsigned long      skey;

    nkey = h->func(key, key_len, h->cdata);
    skey = nkey;

    ZT_HASH_SUB32_MASK(nkey, h->nbits);
    node = h->buckets[nkey];

    while (node) {
        if (node->hash_key == skey) {
            if (h->cmp((void *)node->key, node->key_len, key, key_len, h->cdata)) {
                return node->datum;
            }
        }
        node = node->next;
    }

    errno = ENOENT;
    return NULL;
}

void *
zt_table_del(zt_table *h, const void *key, size_t key_len)
{
    struct table_node * node;
    struct table_node * prev;
    unsigned long       nkey = h->func(key, key_len, h->cdata);
    unsigned long       skey = nkey;

    ZT_HASH_SUB32_MASK(nkey, h->nbits);

    prev = node = h->buckets[nkey];

    while (node) {
        if (node->hash_key == skey) {
            if (h->cmp((void *)node->key, node->key_len, key, key_len, h->cdata)) {
                void * datum = node->datum;

                if (prev == node) {
                    h->buckets[nkey] = node->next;
                } else {
                    prev->next = node->next;
                }

                zt_mem_pool_release((void **)&node);
                return datum;
            }
        }

        node = node->next;
    }
    errno = ENOENT;
    return NULL;
}

int
zt_table_copy(zt_table *t1, zt_table *t2)
{
    unsigned long i;
    int           any = 0;

    for (i = 0; i < t1->num_buckets; i++) {
        struct table_node *node = t1->buckets[i];
        while (node) {
            any = 1;
            /* if(!table_get(t2, node->key)) { */
            zt_table_set(t2, node->key, node->key_len, node->datum, node->datum_len);
            node = node->next;
        }
    }

    return any;
}

int
zt_table_for_each(zt_table *h, zt_table_iterator_cb iterator, void *param)
{
    unsigned long i;
    int           res;

    for (i = 0; i < h->num_buckets; i++) {
        struct table_node *tn;
        tn = h->buckets[i];
        while (tn) {
            struct table_node *next = tn->next;
            if ((res = iterator(tn->key, tn->key_len, tn->datum, tn->datum_len, param)) != 0) {
                return res;
            }

            tn = next;
        }
    }
    return 0;
}

/* common hash functions */
unsigned long
zt_table_hash_int(const void *key, size_t key_len, void *cdata UNUSED)
{
    unsigned char * skey = (unsigned char *)key;
    unsigned long   nkey = zt_hash32_buff(&skey, key_len, ZT_HASH32_INIT);

    return nkey;
}

int
zt_table_compare_int(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata UNUSED)
{
    if (lhs_len == rhs_len) {
        if (memcmp(&lhs, &rhs, lhs_len) == 0) {
            return 1;
        }
    }
    return 0;
}

unsigned long
zt_table_hash_string(const void *key, size_t key_len UNUSED, void *cdata UNUSED)
{
    unsigned char * skey = (unsigned char *)key;
    unsigned long   nkey = zt_hash32_cstr(skey, ZT_HASH32_INIT);

    return nkey;
}

int
zt_table_compare_string(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata UNUSED)
{
    if(lhs_len == rhs_len) {
        if (strncmp((char *)lhs, (char *)rhs, lhs_len) == 0) {
            return 1;
        }
    }
    return 0;
}

int
zt_table_compare_string_case(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata UNUSED)
{
    if (strcasecmp((char *)lhs, (char *)rhs) == 0) {
        return 1;
    }
    return 0;
}

unsigned long
zt_table_hash_buff(const void *key, size_t key_len, void *cdata UNUSED)
{
    unsigned char * skey = (unsigned char *)key;
    unsigned long   nkey = zt_hash32_buff(skey, key_len, ZT_HASH32_INIT);

    return nkey;
}

int
zt_table_compare_buff(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata UNUSED)
{
    if(lhs_len == rhs_len) {
        if (strncmp((char *)lhs, (char *)rhs, lhs_len) == 0) {
            return 1;
        }
    }
    return 0;
}
