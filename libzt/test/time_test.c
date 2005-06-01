
#include <libzt/zt.h>
#include <libzt/zt_time.h>
#include "test.h"


int
main(int argc, char *argv[]) 
{
	struct timeval	  tv1;
	struct timeval    tv2;
	struct timeval    tv_zero = {0,0}; /* 0 second diff time */
	struct timeval	  tv4 = {2,0}; /* 2 second diff time */
	struct timeval	  tgt;
	

	gettimeofday(&tv1, NULL);
	gettimeofday(&tv2, NULL);

	TEST("zt_cmp_time", zt_cmp_time(&tv1, &tv2) < 0);

	zt_diff_time(&tgt, &tv1, &tv2);
	TEST("zt_diff_time", zt_cmp_time(&tv4, &tgt) > 0);

	zt_sub_time(&tgt, &tv1, &tv1);
	TEST("zt_sub_time", zt_cmp_time(&tgt, &tv_zero) == 0);

	zt_add_time(&tgt, &tv1, &tv1);
	TEST("zt_add_time", zt_cmp_time(&tgt, &tv1) > 0);
	
	
	
	return 0;
}
