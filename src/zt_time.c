/*!
 * Filename: zt_time.c
 * Description: time manipulation functions
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *      Copyright (C) 2000-2010, Jason L. Shiffer.
 *      See file COPYING for details
 *
 * Notes:
 *
 */

#ifndef WIN32
#include <sys/resource.h>
#endif

#include "zt_time.h"
#include "zt_log.h"
#include "zt_assert.h"

struct timeval *
zt_add_time(struct timeval *at, struct timeval *t1, struct timeval *t2)
{
    at->tv_sec = t1->tv_sec + t2->tv_sec;
    at->tv_usec = t1->tv_usec + t2->tv_usec;

    while (at->tv_usec >= 1000000) {
        at->tv_sec++;
        at->tv_usec -= 1000000;
    }

    return at;
}


struct timeval *
zt_sub_time(struct timeval *st, struct timeval *t1, struct timeval *t2)
{
    st->tv_sec = t1->tv_sec - t2->tv_sec;
    st->tv_usec = t1->tv_usec - t2->tv_usec;

    while (st->tv_usec < 0) {
        st->tv_sec--;
        st->tv_usec += 1000000;
    }

    return st;
}


struct timeval *
zt_diff_time(struct timeval *dt, struct timeval *t1, struct timeval *t2)
{
    dt->tv_sec = t2->tv_sec - t1->tv_sec;
    dt->tv_usec = t2->tv_usec - t1->tv_usec;

    while (dt->tv_usec < 0) {
        dt->tv_usec += 1000000;
        dt->tv_sec -= 1;
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
    long t = t1->tv_sec - t2->tv_sec;

    if (t) {
        return t;
    }
    return t1->tv_usec - t2->tv_usec;
}



/*!
 * Name: zt_time_result_to_elapsed
 * Description:
 *   calculates the time in float of result and returns the results in usr, sys, total
 *   the calculation used is specific to how zt_time calculates it's information.
 */
void
zt_time_result_to_elapsed(struct time_result *result, float *usr, float *sys, float *total)
{
    zt_assert(result);
    zt_assert(usr);
    zt_assert(sys);
    zt_assert(total);

    *usr = (float) (result->usr_time.tv_sec + result->usr_time.tv_usec / 1000000.0);
    *sys = (float) (result->sys_time.tv_sec + result->sys_time.tv_usec / 1000000.0);
    *total = *usr + *sys;
}

static struct time_result _calibration_time = { { 0, 0 }, { 0, 0 } };

void
zt_time_calibrate(void)
{
    zt_time(0, &_calibration_time, NULL, NULL);
}

/*!
 * Name: zt_time
 * Description:
 *   returns a struct time_result specifing how long the function test took when called
 *   with data. If n is 0 then test does not need to be set (ie test the zt_time overhead).
 *   returns the last result of calling test (or NULL if n = 0).
 */
void *
zt_time(int n, struct time_result *tv, void *(*test)(void *), void *data)
{
#ifndef WIN32
    struct rusage rafter;
    struct rusage rbefore;
    int           i;
    void        * ret = NULL;

    zt_assert(tv);

    getrusage(RUSAGE_SELF, &rbefore);

    if (test && n) {
        for (i = 0; i <= n; i++) {
            ret = test(data);
        }
    }

    getrusage(RUSAGE_SELF, &rafter);

    zt_diff_time(&tv->usr_time, &rbefore.ru_utime, &rafter.ru_utime);
    zt_diff_time(&tv->sys_time, &rbefore.ru_stime, &rafter.ru_stime);
    zt_sub_time(&tv->usr_time, &tv->usr_time, &_calibration_time.usr_time);
    zt_sub_time(&tv->sys_time, &tv->sys_time, &_calibration_time.sys_time);

    return ret;
#else
    return NULL;  // TODO
#endif
}

void
zt_time_print_result(struct time_result *tr, char *name, int n)
{
    float usr;
    float sys;
    float total;

    zt_assert(tr);
    zt_assert(name);

    zt_time_result_to_elapsed(tr, &usr, &sys, &total);

    if (n <= 1) {
        zt_log_printf(zt_log_info, "%s took: %4.2fs user %4.2fs system %4.2fs total", name, usr, sys, total);
    } else {
        zt_log_printf(zt_log_info, "%d calls of %s took: %4.2fs user %4.2fs system %4.2fs total", n, name, usr, sys, total);
    }
}
