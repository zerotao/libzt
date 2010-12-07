/*!
 * Filename: zt_time.h
 * Description: Time routines
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *      Copyright (C) 2004-2010, Jason L. Shiffer.
 *      See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_TIME_H_
#define _ZT_TIME_H_

#ifdef WIN32
#include <WinSock2.h>
#else
#include <sys/time.h>
#endif


#include <zt_internal.h>

struct time_result {
    struct timeval sys_time;
    struct timeval usr_time;
};

struct timeval * zt_add_time(struct timeval *at, struct timeval *t1, struct timeval *t2);

struct timeval * zt_sub_time(struct timeval *st, struct timeval *t1, struct timeval *t2);

struct timeval * zt_diff_time(struct timeval *dt, struct timeval *t1, struct timeval *t2);

long zt_cmp_time(struct timeval *t1, struct timeval *t2);

void zt_time_result_to_elapsed(struct time_result *result, float *usr, float *sys, float *total);

void * zt_time(int n, struct time_result *result, void *(*test)(void *), void *data);

void zt_time_print_result(struct time_result *tr, char *name, int n);

void zt_time_calibrate(void);

#define zt_copy_timeval(x, y) ((x)->tv_sec = (y)->tv_sec, (x)->tv_usec = (y)->tv_usec)

#endif /* _ZT_TIME_H_ */
