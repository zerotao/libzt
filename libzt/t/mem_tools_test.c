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

#include <libzt/zt_mem.h>

struct pool_test {
	int	one;
	int	two;
	int	three;
};

#define NPTEST 100

int
main(int argc, char *argv[])
{
	struct zt_mem_heap	*heap;
	struct zt_mem_pool	*pool;
	struct zt_mem_pool	*tpool;
	struct pool_test	*data[NPTEST];
	int			 i;
	
	heap = zt_mem_heap_init("testheap", 1024 * sizeof(char));
	
	if(heap)
	{
		printf("%s\n", zt_mem_heap_get_name(heap));
	} else {
		printf("failed to alloc a heap\n");
		return -1;
	}
	
	tpool = zt_mem_pool_init("testpool", 4092, sizeof(struct pool_test), NULL, NULL, 0);
	
	/* element size is > normal system page size*/
	pool = zt_mem_pool_init("pool2", 3, sizeof(struct pool_test)+4092, NULL, NULL, 0);
	printf("*INIT*\n");
	
	zt_mem_pools_display(0, DISPLAY_POOL_HEADER_ONLY);
	
	for(i=0; i < NPTEST; i++){
		data[i] = zt_mem_pool_alloc(pool);
		data[i]->one = i;
		data[i]->two = i*100;
		data[i]->three = i*1000;
	}

	printf("*ALLOC*\n");
	zt_mem_pools_display(0, DISPLAY_POOL_HEADER_ONLY);
	
	for(i=0; i < NPTEST; i++){
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
}

