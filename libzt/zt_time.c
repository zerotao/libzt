/****h* libZT/Time
 * DESCRIPTION
 *   Tools to manipulate time
 ***
 */
#include <sys/time.h>
#include <sys/resource.h>

#include "zt_time.h"
#include "zt_log.h"
#include "zt_assert.h"

struct timeval *
zt_add_time(struct timeval *at,
	    struct timeval *t1,
	    struct timeval *t2)
{
	at->tv_sec = t1->tv_sec + t2->tv_sec;
	at->tv_usec = t1->tv_usec + t2->tv_usec;
	
	while(at->tv_usec >= 1000000) {
		at->tv_sec++;
		at->tv_usec -= 1000000;
	}
	
	return at;
}


struct timeval *
zt_sub_time(struct timeval *st,
	    struct timeval *t1,
	    struct timeval *t2)
{
	st->tv_sec = t1->tv_sec - t2->tv_sec;
	st->tv_usec = t1->tv_usec - t2->tv_usec;
	
	while(st->tv_usec < 0) {
		st->tv_sec--;
		st->tv_usec += 1000000;
	}	
	
	return st;
}


/****f* Time/zt_diff_time
 * NAME
 *   zt_diff_time
 *
 * SYNOPSIS
 *   struct timeval *zt_diff_time(struct timeval *dt, struct timeval *t1, struct timeval *t2);
 *
 * DESCRIPTION
 *   calculate the difference between 2 time vals and return the result in dt
 *
 * EXAMPLES
 *
 ****/
struct timeval *
zt_diff_time(struct timeval *dt,
	     struct timeval *t1,
	     struct timeval *t2) 
{
	dt->tv_sec = t2->tv_sec - t1->tv_sec;
	dt->tv_usec = t2->tv_usec - t1->tv_usec;
	
	while(dt->tv_usec < 0){
		dt->tv_usec += 1000000.0;
		dt->tv_sec -= 1.0;
	}
	return dt;
}

/*    <0, 0, >0
 * t1  <, =, >  t2
 */
long
zt_cmp_time(struct timeval *t1,
	    struct timeval *t2)
{
	long	  t = t1->tv_sec - t2->tv_sec;
	
	if(t) {
		return t;
	}
	
	return t1->tv_usec - t2->tv_usec;
}



/****f* Time/zt_time_result_to_elapsed
 * NAME
 *   zt_time_result_to_elapsed
 *
 * DESCRIPTION
 *   calculates the time in float of result and returns the results in usr, sys, total
 *   the calculation used is specific to how zt_time calculates it's information.
 *
 * INPUTS
 *   o result
 *   o usr
 *   o sys
 *   o total
 *
 * SEE ALSO
 *   zt_time
 ****/
void
zt_time_result_to_elapsed(struct time_result *result,
			  float *usr,
			  float *sys,
			  float *total)
{
	assert(result);
	assert(usr);
	assert(sys);
	assert(total);

	*usr = result->usr_time.tv_sec + result->usr_time.tv_usec /   1000000.0;
	*sys = result->sys_time.tv_sec + result->sys_time.tv_usec /   1000000.0;
	*total = *usr + *sys;	
}


/****f* Time/zt_time
 * NAME
 *   zt_time
 *
 * DESCRIPTION
 *   returns a struct time_result specifing how long the function test took when called
 *   with data.
 *
 * INPUTS
 *   o n
 *   o result
 *   o test
 *   o data
 *
 * SEE ALSO
 *   zt_time_result_to_elapsed
 ****/
void *
zt_time(int n,
	struct time_result *tv,
	void *(*test)(void *),
	void *data)
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
	
	return ret;
}

void
zt_time_print_result(struct time_result *tr, char *name, int n)
{
	float usr, sys, total;
	
	assert(tr);
	assert(name);
	
	zt_time_result_to_elapsed(tr, &usr, &sys, &total);

	if(n <= 1) {
		log_printf(log_info, "%s took: %4.2fs user %4.2fs system %4.2fs total\n", name, usr, sys, total);
	} else {
		log_printf(log_info, "%d calls of %s took: %4.2fs user %4.2fs system %4.2fs total \n",n, name, usr, sys, total);
	}
	
}

