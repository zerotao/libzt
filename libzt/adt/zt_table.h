/****h* libZT/HashTable
 * DESCRIPTION
 *
 * COPYRIGHT
 *   Copyright (C) 2002-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/
#ifndef _ZT_TABLE_H_
#define _ZT_TABLE_H_

#include <sys/types.h>

#include <libzt/zt.h>


BEGIN_C_DECLS
typedef int (*table_compare)(const void *x, const void *y);
typedef unsigned long (*table_hash_func)(unsigned char *key);
typedef int (*table_iterator)(void *, void *, void *);

typedef struct zt_table zt_table;

#define TABLE_SIZE_USE_HINT 	1
#define TABLE_ALLOW_DUP_KEYS    2

zt_table*
table_init(char *name, table_hash_func func, table_compare cmp, size_t hint, int flags);

void
table_destroy(zt_table *h);

int
table_set(zt_table *h, const void *key, const void *datum);

void*
table_get(zt_table *h, const void *key);

void*
table_del(zt_table *h, const void *key);

int table_cpy(zt_table *h, zt_table *t2);

int
table_for_each(zt_table *h, table_iterator iterator, void *param);

/* common types */
unsigned long
table_hash_int(unsigned char *key);

int
table_compare_int(const void *x, const void *y);

unsigned long
table_hash_string(unsigned char *key);

int
table_compare_string(const void *x, const void *y);

int
table_compare_string_case(const void *x, const void *y);

END_C_DECLS
#endif /* _ZT_TABLE_H_ */
