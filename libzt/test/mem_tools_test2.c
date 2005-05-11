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
#include <stdlib.h>
#include <sys/time.h>		/* gettimeofday */
#include <sys/resource.h>
#include <libzt/zt_mem.h>
#include <libzt/zt_time.h>

struct test_data_t {
	long			  size;
	int			  n;
	struct zt_mem_pool	* pool;
	void			**parr;
};


void
timetest(char *name, int n, void *(*test)(void *), void *data)
{
	struct time_result	  result;
	float			  usr, sys, total;
	
	zt_time(n, &result, test, data);
	zt_time_result_to_elapsed(&result, &usr, &sys, &total);
	
	if(n == 1) {
		printf("%s took: %4.2fs user %4.2fs system %4.2fs total \n",name, usr, sys, total);
	} else {
		printf("%d calls of %s took: %4.2fs user %4.2fs system %4.2fs total \n",n, name, usr, sys, total);
	}	
}


void *test_malloc(void *data)
{
	struct test_data_t	* td = data;
	int			  size = td->size;
	int			  i;

	for(i = 0; i < td->n; i++) {
		td->parr[i] = malloc(size);
	}

	for(i = 0; i < td->n; i++) {
		free(td->parr[i]);
	}
	
	return 0;
}

void *test_pool(void *data)
{
	struct test_data_t	* td = data;	
	struct zt_mem_pool	* pool = td->pool;
	int			  i = 0;

	for(i = 0; i < td->n; i++) {
		td->parr[i] = zt_mem_pool_alloc(pool);
	}

	for(i = 0; i < td->n; i++) {	
		zt_mem_pool_release(&(td->parr[i]));
	}
	
	return 0;
}

#define NPTEST 10000
#define NPLOOP 500
int
main(int argc, char *argv[])
{
	struct test_data_t	  td_small;
	struct test_data_t	  td_large;
	void			**parr;

	parr = (void **)calloc(NPTEST+1, sizeof(void *));
	
	td_small.parr = parr;
	td_small.size = sizeof(int);
	td_small.n = NPTEST;

	td_large.parr = parr;
	td_large.size = 150*sizeof(int);
	td_large.n = NPTEST;

	
	timetest("malloc_small", NPLOOP, test_malloc, (void *)&td_small);
	timetest("malloc_large", NPLOOP, test_malloc, (void *)&td_large);

	/* I do not do these until here so that malloc has free reign on memory
	 * on at least one machine this made the difference between malloc taking
	 * 27 seconds versus 16 after the move.
	 */
	td_small.pool = zt_mem_pool_init("small_pool", 412, td_small.size, NULL, NULL, POOL_NEVER_FREE);
	td_large.pool = zt_mem_pool_init("large_pool", 100, td_large.size, NULL, NULL, POOL_NEVER_FREE);
	
	timetest("pool_alloc_small", NPLOOP, test_pool, (void *)&td_small);
	timetest("pool_alloc_large", NPLOOP, test_pool, (void *)&td_large);

	//	zt_mem_pools_display(0, DISPLAY_POOL_HEADER_ONLY);

	zt_mem_pool_destroy(&td_small.pool);
	zt_mem_pool_destroy(&td_large.pool);
	free(parr);
	return 0;
}
