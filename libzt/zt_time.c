#include <sys/time.h>
#include <sys/resource.h>

#include "zt_time.h"
#include "zt_log.h"
#include "zt_assert.h"

struct timeval *
zt_diff_time(struct timeval *dt, struct timeval *t1, struct timeval *t2) 
{
	dt->tv_sec = t2->tv_sec - t1->tv_sec;
	dt->tv_usec = t2->tv_usec = t1->tv_usec;
	if(dt->tv_usec < 0){
		dt->tv_usec += 1000000.0;
		dt->tv_sec -= 1.0;
	}
	return dt;
}

void
zt_time_result_to_elapsed(struct time_result *result, float *usr, float *sys, float *total)
{
	assert(result);
	assert(usr);
	assert(sys);
	assert(total);

	*usr = result->usr_time.tv_sec + result->usr_time.tv_usec /   1000000.0;
	*sys = result->sys_time.tv_sec + result->sys_time.tv_usec /   1000000.0;
	*total = *usr + *sys;	
}

void *
zt_time(int n, struct time_result *tv, void *(*test)(void *), void *data)
{
	struct rusage	  rafter, rbefore;
	int		  i;
	void		* ret = NULL;

	assert(tv);
	
	getrusage(RUSAGE_SELF, &rbefore);
	
	for(i = 0; i < n; i++) {
		ret = test(data);
	}
	
	getrusage(RUSAGE_SELF, &rafter);

	zt_diff_time(&tv->usr_time, &rbefore.ru_utime, &rafter.ru_utime);
	zt_diff_time(&tv->sys_time, &rbefore.ru_stime, &rafter.ru_stime);
	
/* 	if(n == 1) { */
/* 		log_printf(log_info, "%s took: %4.2fs user %4.2fs system %4.2fs total \n",name, uel, sel, elapsed); */
/* 	} else { */
/* 		log_printf(log_info, "%d calls of %s took: %4.2fs user %4.2fs system %4.2fs total \n",n, name, uel, sel, elapsed); */
/* 	} */

	return ret;
}

