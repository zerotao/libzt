/*
 * zt_table.h        ZeroTao Tablees
 *
 * Copyright (C) 2002-2004, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 */
#ifndef _ZT_TABLE_H_
#define _ZT_TABLE_H_

#include <sys/types.h>

#include <libzt/zt.h>

typedef int (*table_compare)(void *x, void *y);
typedef unsigned long (*table_func)(unsigned char *,
				    unsigned long prev,
				    unsigned long maxbits);
typedef int (*table_iterator)(void *, void *, void *);

typedef struct zt_table zt_table;

#define TABLE_SIZE_USE_HINT 1

zt_table*	table_init(char *name,
			   table_func,
			   table_compare,
			   size_t hint,
			   int flags);
void  		table_destroy(zt_table *h);
int   		table_set(zt_table *h,
			  void *key,
			  void *datum);
void*		table_get(zt_table *h,
			  void *key);
int		table_for_each(zt_table *h,
			       table_iterator,
			       void *param);

/* common types */
unsigned long table_hash_int(unsigned char *key,
			     unsigned long prev,
			     unsigned long maxbits);
int table_compare_int(void *x, void *y);

unsigned long table_hash_string(unsigned char *key,
				unsigned long prev,
				unsigned long maxbits);
int table_compare_string(void *x, void *y);
int table_compare_string_case(void *x, void *y);

#endif /* _ZT_TABLE_H_ */
