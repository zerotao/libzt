/*
 * zt_table.c        ZeroTao Tablees
 *
 * Copyright (C) 2002-2004, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include "../zt.h"
#include "../zt_mem.h"
#include "../zt_hash.h"
#include "../zt_table.h"
#include "../zt_log.h"

#define DEFAULT_BUCKETS 512
static int
nbits(unsigned long val) 
{
	int nbits;
	for(nbits=1; (val = val >> 1); nbits++);
	return nbits;
}

unsigned long powers_of_2[] = {2,4,8,
			       16,32,64,
			       128,256,512,
			       1024,2048,4096,
			       8192,16384,32768,
			       65535,131072,262144,
			       524288,1048576,2097152,
			       4194304,8388608,16777216,
			       33554432,67108864,134217728,
			       268435456,536870912,1073741824,
			       2147483648UL, UINT_MAX};
		 
struct table_node {
	void		 *datum;
	void		 *key;
	struct table_node *next;
};

struct zt_table {
	char			* name;
	table_compare 		  cmp;
	table_func    		  func;
	int			  nbits;
	unsigned long		  num_buckets;
	unsigned int 		  filled;
	zt_mem_pool		* node_pool;
	zt_mem_heap		* table_heap;
	struct table_node	**buckets;
};

struct Global {
	zt_mem_pool	* table_mem_pool;
}Global;

zt_table *
table_init(char *name, table_func func, table_compare cmp, size_t hint, int flags)
{
	zt_table 	* table;
	int		  nbuckets = 0;
	char		  tname[1024];

	if( (func == NULL) ||
	    (cmp == NULL) ||
	    (name == NULL))
	{
		return NULL;
	}
	
	if(Global.table_mem_pool == NULL)
	{
		Global.table_mem_pool = zt_mem_pool_init("table pool", 2, sizeof(zt_table), NULL, NULL, 0);
	}
	
	if( (table = (zt_table *)zt_mem_pool_alloc(Global.table_mem_pool)) == NULL){
		return NULL;
	}

	if(flags & TABLE_SIZE_USE_HINT) {
		nbuckets = hint;
	} else {
		int i = 0;
		int len = sizeof_array(powers_of_2);
		
		for(i = 0; i < len; i++) {
			if(powers_of_2[i] > hint) {
				nbuckets = powers_of_2[i];
				break;
			}
		}
	}

	table->nbits = nbits(nbuckets-1);
	
	table->table_heap = zt_mem_heap_init(0, nbuckets * sizeof(struct table_node *));

	table->buckets = zt_mem_heap_get_data(table->table_heap);
	memset(table->buckets, 0, nbuckets * sizeof(struct table_node *));
	
	table->name = XSTRDUP(name);
	
	snprintf(tname, sizeof_array(tname), "%s (table node pool)", name);
	table->node_pool = zt_mem_pool_init(tname, nbuckets,
					   sizeof(struct table_node), NULL, NULL, 0);
	
	table->num_buckets = nbuckets;
	table->cmp = cmp;
	table->func = func;

	return table;
}

void
table_destroy(zt_table *h)
{
	int i;
  
	for(i=0; i < h->num_buckets; i++)
	{
		struct table_node *node = h->buckets[i];
		while(node)
		{
			struct table_node *tmp = node->next;
			zt_mem_pool_release((void **)&node);
			node = tmp;
		}
	}
	
	if(h->name) {
		XFREE(h->name);
	}
	
	zt_mem_heap_destroy(&h->table_heap);
}

int
table_set(zt_table *h, void *key, void *datum)
{
	unsigned long	  nkey = h->func(key, 1, h->nbits);
	
	LOG_NDEBUG("for key %d hash key is: %d", (int)key, nkey);
	
	struct table_node *node =(struct table_node *)zt_mem_pool_alloc(h->node_pool);

	node->next = h->buckets[nkey];
	h->buckets[nkey] = node;
	node->key = key;
	node->datum = datum;

	return 0;
}

void *
table_get(zt_table *h, void *key)
{
	int nkey = h->func (key, 1, h->nbits);
	
	struct table_node *node = h->buckets[nkey];
	while(node)
	{
		if(h->cmp ((void *)node->key, key))
			return node->datum;
		node = node->next;
	}
	return NULL;
}

int
table_for_each(zt_table *h, table_iterator iterator, void *param)
{
	int i;
  
	for(i=0; i < h->num_buckets; i++)
	{
		struct table_node *tn;
		tn = h->buckets[i];
		while(tn)
		{
			struct table_node *next = tn->next;
			iterator (tn->key, tn->datum, param);
			tn = next;
		}
	}
	return 0;
}

/* common hash functions */
unsigned long table_hash_int(unsigned char *key, unsigned long prev, unsigned long maxbits)
{
	unsigned long nkey = zt_hash((unsigned char *)&key, sizeof(int), prev);
	nkey = (nkey & zt_hashmask(maxbits));
	return nkey;
}

int table_compare_int(void *x, void *y)
{
	if((int)x == (int)y) {
		return 1;
	}
	return 0;
}

unsigned long table_hash_string(unsigned char *key, unsigned long prev, unsigned long maxbits)
{
	unsigned long nkey = zt_hash(key, strlen((char *)key), prev);
	nkey = (nkey & zt_hashmask(maxbits));
	return nkey;
}

int table_compare_string(void *x, void *y)
{
	if(strcmp((char *)x, (char *)y) == 0) {
		return 1;
	}
	return 0;
}

int table_compare_string_case(void *x, void *y)
{
	if(strcasecmp((char *)x, (char *)y) == 0) {
		return 1;
	}
	return 0;
}
