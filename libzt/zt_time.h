#ifndef _ZT_TIME_H_
#define _ZT_TIME_H_

#include <sys/time.h>

#include <libzt/zt.h>

struct time_result {
	struct timeval	  sys_time;
	struct timeval	  usr_time;
};

/** f
 * calculate the difference between 2 time vals and return the result in dt
 *
 * @param dt pointer in which the results of the difference are returned
 * @param t1 pointer containing the first time
 * @param r2 pointer containing the second time
 */
struct timeval *
zt_diff_time _((struct timeval *dt, struct timeval *t1, struct timeval *t2));

/* f
 *
 * calculates the time in float of result and returns the results in usr, sys, total
 * the calculation used is specific to how zt_time calculates it's information.
 *
 * see also: zt_time
 */
void
zt_time_result_to_elapsed(struct time_result *result,
			  float *usr,
			  float *sys,
			  float *total);


/* f
 *
 * prints a message about how long the function test took when called
 * with data.
 *
 * see also: zt_time_result_to_elapsed
 */
void *
zt_time _((int n, struct time_result *result, void *(*test)(void *), void *data));


#endif /* _ZT_TIME_H_ */
