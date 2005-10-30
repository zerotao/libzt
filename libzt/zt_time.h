/*
 * Copyright (C) 2004-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 * All Rights Reserved.
 * See file COPYING for details.
 */
#ifndef _ZT_TIME_H_
#define _ZT_TIME_H_

#include <sys/time.h>

#include <libzt/zt.h>

/****s* Time/time_result
 * NAME
 *   time_result - result structure used by zt_time functions
 * SOURCE
 */
struct time_result {
	struct timeval	  sys_time;
	struct timeval	  usr_time;
};
/*** time_result ***/

struct timeval *
zt_add_time(struct timeval *at,
	    struct timeval *t1,
	    struct timeval *t2);

struct timeval *
zt_sub_time(struct timeval *st,
	    struct timeval *t1,
	    struct timeval *t2);

struct timeval *
zt_diff_time(struct timeval *dt,
	     struct timeval *t1,
	     struct timeval *t2);

long
zt_cmp_time(struct timeval *t1,
	    struct timeval *t2);

void
zt_time_result_to_elapsed(struct time_result *result,
			  float *usr,
			  float *sys,
			  float *total);
void *
zt_time(int n,
	struct time_result *result,
	void *(*test)(void *),
	void *data);

void
zt_time_print_result(struct time_result *tr, char *name, int n);

#define zt_copy_timeval(x, y) ((x)->tv_sec = (y)->tv_sec, (x)->tv_usec = (y)->tv_usec)
#endif /* _ZT_TIME_H_ */
