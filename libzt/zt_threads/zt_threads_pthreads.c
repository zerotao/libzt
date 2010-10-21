#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

#include <zt_threads.h>

void *
zt_threads_pthreads_lock_alloc(int locktype) {
    pthread_mutex_t *lock;

    if ((lock = calloc(sizeof(pthread_mutex_t), 1)) == NULL) {
        return NULL;
    }

    if (pthread_mutex_init(lock, NULL)) {
        free(lock);
        return NULL;
    }

    return lock;
}

void
zt_threads_pthreads_lock_free(void *_lock, int locktype) {
    pthread_mutex_t *lock;

    lock = (pthread_mutex_t *)_lock;

    pthread_mutex_destroy(lock);
    free(lock);
}

int
zt_threads_pthreads_lock(int mode, void *_lock) {
    pthread_mutex_t *lock;

    lock = (pthread_mutex_t *)_lock;

    if (mode & ZT_THREADS_LOCK_NONBLOCKING) {
        return pthread_mutex_trylock(lock);
    }

    return pthread_mutex_lock(lock);
}

int
zt_threads_pthreads_unlock(int mode, void *_lock) {
    pthread_mutex_t *lock;

    lock = (pthread_mutex_t *)_lock;

    return pthread_mutex_unlock(lock);
}

void *
zt_threads_pthreads_cond_alloc(int condtype) {
    pthread_cond_t *cond;

    if ((cond = calloc(sizeof(pthread_cond_t), 1)) == NULL) {
        return NULL;
    }

    if (pthread_cond_init(cond, NULL)) {
        free(cond);
        return NULL;
    }

    return cond;
}

void
zt_threads_pthreads_cond_free(void *_cond) {
    pthread_cond_t *cond;

    cond = (pthread_cond_t *)_cond;
    pthread_cond_destroy(cond);
    free(cond);
}

int
zt_threads_pthreads_cond_signal(void *_cond, int broadcast) {
    pthread_cond_t *cond;
    int             status;

    cond = (pthread_cond_t *)_cond;

    if (broadcast) {
        status = pthread_cond_broadcast(cond);
    } else {
        status = pthread_cond_signal(cond);
    }

    return status ? -1 : 0;
}

int
zt_threads_pthreads_cond_wait(void *_cond, void *_lock, struct timeval *tv) {
    int              status;
    pthread_cond_t  *cond;
    pthread_mutex_t *lock;

    cond = (pthread_cond_t *)_cond;
    lock = (pthread_mutex_t *)_lock;

    if (tv != NULL) {
        struct timeval  now;
        struct timeval  abstime;
        struct timespec ts;

        /* timeradd(tvp, uvp, vvp) timeradd((tvp), (uvp), (vvp)) */
        gettimeofday(&now, NULL);
        timeradd(&now, tv, &abstime);
        ts.tv_sec  = abstime.tv_sec;
        ts.tv_nsec = abstime.tv_usec * 1000;

        status     = pthread_cond_timedwait(cond, lock, &ts);

        if (status == ETIMEDOUT) {
            return 1;
        }

        if (status) {
            return -1;
        }

        return 0;
    }

    status = pthread_cond_wait(cond, lock);
    return status ? -1 : 0;
}

int
zt_threads_pthreads_start(void *_thread, void *_attr,
                          void * (*start_cb)(void *), void *args) {
    pthread_t      *thread;
    pthread_attr_t *attr;
    int             status;

    thread = (pthread_t *)_thread;
    attr   = (pthread_attr_t *)_attr;

    status = pthread_create(thread, attr, start_cb, args);

    return status ? -1 : 0;
}

void
zt_threads_pthreads_end(void *args) {
    return pthread_exit(args);
}

void *
zt_threads_pthreads_alloc_thread(void) {
    return calloc(sizeof(pthread_t), 1);
}

int
zt_threads_pthreads_kill(void *_thread) {
    pthread_t *thread;

    thread = (pthread_t *)_thread;

    return pthread_cancel(*thread);
}

unsigned int
zt_threads_pthread_id(void) {
    return (unsigned int)pthread_self();
}

int
zt_threads_use_pthreads(void) {
    struct zt_threads_lock_callbacks  cbs = {
        zt_threads_pthreads_lock_alloc,
        zt_threads_pthreads_lock_free,
        zt_threads_pthreads_lock,
        zt_threads_pthreads_unlock
    };

    struct zt_threads_cond_callbacks  cond_cbs = {
        zt_threads_pthreads_cond_alloc,
        zt_threads_pthreads_cond_free,
        zt_threads_pthreads_cond_signal,
        zt_threads_pthreads_cond_wait
    };

    struct zt_threads_cntrl_callbacks cntrl_cbs = {
        zt_threads_pthreads_start,
        zt_threads_pthreads_end,
        zt_threads_pthreads_kill,
        zt_threads_pthread_id
    };

    zt_threads_set_lock_callbacks(&cbs);
    zt_threads_set_cond_callbacks(&cond_cbs);
    zt_threads_set_cntrl_callbacks(&cntrl_cbs);
    zt_threads_alloc_thread = zt_threads_pthreads_alloc_thread;

    return 0;
}

