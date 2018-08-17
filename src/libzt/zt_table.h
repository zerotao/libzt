/*!
 * Filename: zt_table.h
 * Description: table (dictionary, etc) implementation using pools
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2002-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_TABLE_H_
#define _ZT_TABLE_H_

#include <zt.h>

BEGIN_C_DECLS
typedef int (*zt_table_compare_cb)(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata);
typedef unsigned long (*zt_table_hash_func_cb)(const void *key, size_t key_len, void *cdata);
typedef int (*zt_table_iterator_cb)(void * key, size_t key_len, void * datum, size_t datum_len, void * param);

typedef struct zt_table zt_table;
typedef struct zt_table_allocator zt_table_allocator;

#define ZT_TABLE_SIZE_USE_HINT     1
#define ZT_TABLE_ALLOW_DUP_KEYS    2

zt_mem_pool * zt_table_pool_init(long elts);

zt_table* zt_table_init(char *name, zt_table_hash_func_cb func,
                        zt_table_compare_cb cmp, size_t hint,
                        int flags, void *cdata);

void zt_table_destroy(zt_table *h);

int zt_table_set(zt_table *h, const void *key, size_t key_len, const void *datum, size_t datum_len);

void* zt_table_get(zt_table *h, const void *key, size_t key_len);

void* zt_table_del(zt_table *h, const void *key, size_t key_len);

int zt_table_copy(zt_table *h, zt_table *t2);

int zt_table_for_each(zt_table *h, zt_table_iterator_cb iterator, void *param);

/************ common types ************/
unsigned long zt_table_hash_int(const void *key, size_t key_len, void *cdata);
int zt_table_compare_int(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata);
unsigned long zt_table_hash_string(const void *key, size_t key_len, void *cdata);
int zt_table_compare_string(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata);
int zt_table_compare_string_case(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata);
unsigned long zt_table_hash_buff(const void *key, size_t key_len, void *cdata);
int zt_table_compare_buff(const void *lhs, size_t lhs_len, const void *rhs, size_t rhs_len, void *cdata);
END_C_DECLS
#endif /* _ZT_TABLE_H_ */
