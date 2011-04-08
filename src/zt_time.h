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


#include <zt.h>

struct time_result {
    struct timeval sys_time;
    struct timeval usr_time;
};

struct timeval * zt_add_time(struct timeval * at, struct timeval * t1, struct timeval * t2);
struct timeval * zt_sub_time(struct timeval * st, struct timeval * t1, struct timeval * t2);
struct timeval * zt_diff_time(struct timeval * dt, struct timeval * t1, struct timeval * t2);
long             zt_cmp_time(struct timeval * t1, struct timeval * t2);
void             zt_time_result_to_elapsed(struct time_result * result, float * usr, float * sys, float * total);
void           * zt_time(int n, struct time_result * result, void *(*test)(void *), void * data);
void             zt_time_print_result(struct time_result * tr, char * name, int n);
void             zt_time_calibrate(void);

#define zt_copy_timeval(x, y) ((x)->tv_sec = (y)->tv_sec, (x)->tv_usec = (y)->tv_usec)

#if (!defined(_BSD_SOURCE) || !defined(_XOPEN_SOURCE) >= 500)
#if defined _POSIX_C_SOURCE >= 199309L
#define usleep(msec)          do {                 \
    unsigned long   millisec = msec;               \
    time_t          sec      = (int)(msec / 1000); \
    struct timespec req;                           \
                                                   \
    memset(&req, 0, sizeof(struct timespec));      \
    millisec    = msec - (sec * 1000);             \
    req.tv_sec  = sec;                             \
    req.tv_nsec = msec * 1000000L;                 \
                                                   \
    while (nanosleep(&req, &req) == -1) {          \
        continue;                                  \
    }                                              \
} while (0)
#else
#define usleep(msec)          do {       \
    long           usec = msec;          \
    struct timeval delay;                \
                                         \
    delay.tv_sec  = msec / 1000000L;     \
    delay.tv_usec = usec % 1000000L;     \
                                         \
    select(0, NULL, NULL, NULL, &delay); \
} while (0)
#endif
#endif

#endif /* _ZT_TIME_H_ */
