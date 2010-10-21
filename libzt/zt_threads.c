#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>
#include <unistd.h>
#include <zt_threads.h>

#ifndef _ZT_DISABLE_THREAD_SUPPORT

static void *_zt_threadpool_loop(void *args);

struct zt_threads_lock_callbacks  _zt_threads_lock_cbs = {
    NULL, NULL, NULL, NULL
};

struct zt_threads_cond_callbacks  _zt_threads_cond_cbs = {
    NULL, NULL, NULL, NULL
};

struct zt_threads_cntrl_callbacks _zt_threads_cntrl_cbs = {
    NULL, NULL
};

struct zt_threadpool_callbacks    _zt_threadpool_cbs = {
    _zt_threadpool_loop, NULL, NULL
};

void                            * (*zt_threads_alloc_thread)(void);

int
zt_threads_set_lock_callbacks(struct zt_threads_lock_callbacks *cbs) {
    struct zt_threads_lock_callbacks *t = &_zt_threads_lock_cbs;

    if (cbs == NULL) {
        memset(t, 0, sizeof(_zt_threads_lock_cbs));
        return 0;
    }

    if (cbs->alloc && cbs->free && cbs->lock && cbs->unlock) {
        memcpy(t, cbs, sizeof(_zt_threads_lock_cbs));
        return 0;
    }

    return -1;
}

int
zt_threads_set_cond_callbacks(struct zt_threads_cond_callbacks *cbs) {
    struct zt_threads_cond_callbacks *t = &_zt_threads_cond_cbs;

    if (cbs == NULL) {
        memset(t, 0, sizeof(_zt_threads_cond_cbs));
        return 0;
    }

    if (cbs->alloc && cbs->free && cbs->signal && cbs->wait) {
        memcpy(t, cbs, sizeof(_zt_threads_cond_cbs));
        return 0;
    }

    return -1;
}

int
zt_threads_set_cntrl_callbacks(struct zt_threads_cntrl_callbacks *cbs) {
    struct zt_threads_cntrl_callbacks *t = &_zt_threads_cntrl_cbs;

    if (cbs == NULL) {
        memset(t, 0, sizeof(_zt_threads_cntrl_cbs));
        return 0;
    }

    if (cbs->start && cbs->end) {
        memcpy(t, cbs, sizeof(_zt_threads_cntrl_cbs));
        return 0;
    }

    return -1;
}

void *
zt_threads_alloc_lock(int type) {
    if (_zt_threads_lock_cbs.alloc) {
        return _zt_threads_lock_cbs.alloc(type);
    }

    return NULL;
}

void
zt_threads_free_lock(void * lock, int locktype) {
    if (_zt_threads_lock_cbs.free) {
        _zt_threads_lock_cbs.free(lock, locktype);
    } else {
        return;
    }
}

int
zt_threads_lock(int mode, void *lock) {
    if (_zt_threads_lock_cbs.lock) {
        return _zt_threads_lock_cbs.lock(mode, lock);
    }

    return 0;
}

int
zt_threads_unlock(int mode, void *lock) {
    if (_zt_threads_lock_cbs.unlock) {
        return _zt_threads_lock_cbs.unlock(mode, lock);
    }

    return 0;
}

void *
zt_threads_cond_alloc(int type) {
    if (_zt_threads_cond_cbs.alloc) {
        return _zt_threads_cond_cbs.alloc(type);
    }

    return NULL;
}

void
zt_threads_cond_free(void *cond) {
    if (_zt_threads_cond_cbs.free) {
        return _zt_threads_cond_cbs.free(cond);
    }
}

int
zt_threads_cond_signal(void *cond, int broadcast) {
    if (_zt_threads_cond_cbs.signal) {
        return _zt_threads_cond_cbs.signal(cond, broadcast);
    }

    return 0;
}

int
zt_threads_cond_wait(void *cond, void *lock, struct timeval *timeout) {
    if (_zt_threads_cond_cbs.wait) {
        return _zt_threads_cond_cbs.wait(cond, lock, timeout);
    }

    return 0;
}

int
zt_threads_start(void *thread, void *attr, void * (*cb)(void *), void *args) {
    if (_zt_threads_cntrl_cbs.start) {
        return _zt_threads_cntrl_cbs.start(thread, attr, cb, args);
    }

    return 0;
}

void
zt_threads_end(void *args) {
    if (_zt_threads_cntrl_cbs.end) {
        return _zt_threads_cntrl_cbs.end(args);
    }
}

int
zt_threads_kill(void *thread) {
    if (_zt_threads_cntrl_cbs.kill) {
        return _zt_threads_cntrl_cbs.kill(thread);
    }
    return 0;
}

unsigned int
zt_threads_id(void) {
    if (_zt_threads_cntrl_cbs.id) {
        return _zt_threads_cntrl_cbs.id();
    }

    return 0;
}

/*****************************************************************
* threadpool defs
*****************************************************************/

int
zt_threadpool_set_callbacks(struct zt_threadpool_callbacks *cbs) {
    if (cbs == NULL) {
        /* set our defaults */
        _zt_threadpool_cbs.loop   = _zt_threadpool_loop;
        _zt_threadpool_cbs.init   = NULL;
        _zt_threadpool_cbs.worker = NULL;

        return 0;
    }

    if (cbs->loop) {
        _zt_threadpool_cbs.loop = cbs->loop;
    }

    if (cbs->init) {
        _zt_threadpool_cbs.init = cbs->init;
    }

    if (cbs->worker) {
        _zt_threadpool_cbs.worker = cbs->worker;
    }

    return 0;
}

static void *
_zt_threadpool_loop(void *args) {
    zt_threadpool *tpool;
    void          *init_data = NULL;

    tpool = (zt_threadpool *)args;

    if (_zt_threadpool_cbs.init) {
        init_data = _zt_threadpool_cbs.init(args);
    }

    while (1) {
        zt_threadpool_entry *entry = NULL;

        if (zt_threads_lock(0, tpool->iput_mutex)) {
            fprintf(stderr, "lock error...\n");
            zt_threads_end(NULL);
        }

        if (TAILQ_EMPTY(&tpool->iput_queue)) {
            zt_threads_cond_wait(tpool->iput_cond, tpool->iput_mutex, NULL);
        } else {
            entry = TAILQ_FIRST(&tpool->iput_queue);
            TAILQ_REMOVE(&tpool->iput_queue, entry, next);
        }

        zt_threads_unlock(0, tpool->iput_mutex);

        if (entry == NULL) {
            continue;
        }

        if (_zt_threadpool_cbs.worker) {
            int status;
            status = _zt_threadpool_cbs.worker(init_data, entry->data);

            if (status == 1) {
                zt_threadpool_insert_oput(tpool, entry->data);
            }
        }

        if (entry != NULL) {
            free(entry);
        }
    }

    return NULL;
} /* _zt_threadpool_loop */

int
zt_threadpool_insert_iput(zt_threadpool *tpool, void *data) {
    zt_threadpool_entry *entry;

    entry       = calloc(sizeof(zt_threadpool_entry), 1);
    entry->data = data;

    zt_threads_lock(0, tpool->iput_mutex);
    {
        TAILQ_INSERT_TAIL(&tpool->iput_queue, entry, next);
        zt_threads_cond_signal(tpool->iput_cond, 0);

        if (tpool->iput_fd_sigs[1] >= 0) {
            write(tpool->iput_fd_sigs[1], "", 1);
        }
    }
    zt_threads_unlock(0, tpool->iput_mutex);

    return 0;
}

int
zt_threadpool_insert_oput(zt_threadpool *tpool, void *data) {
    zt_threadpool_entry *entry;

    entry       = calloc(sizeof(zt_threadpool_entry), 1);
    entry->data = data;

    zt_threads_lock(0, tpool->oput_mutex);
    {
        TAILQ_INSERT_TAIL(&tpool->oput_queue, entry, next);
        zt_threads_cond_signal(tpool->oput_cond, 0);

        if (tpool->oput_fd_sigs[1] >= 0) {
            write(tpool->oput_fd_sigs[1], "", 1);
        }
    }
    zt_threads_unlock(0, tpool->oput_mutex);

    return 0;
}

int
zt_threadpool_start(zt_threadpool *tpool) {
    int i;

    for (i = 0; i < tpool->min_threads; i++) {
        tpool->threads[i] = zt_threads_alloc_thread();
        zt_threads_start(tpool->threads[i], NULL, _zt_threadpool_cbs.loop, (void *)tpool);
    }

    return 0;
}

zt_threadpool *
zt_threadpool_init(int min_threads, int max_threads, int pipe_iput, int pipe_oput) {
    zt_threadpool *tpool;

    tpool               = calloc(sizeof(zt_threadpool), 1);
    tpool->min_threads  = min_threads;
    tpool->max_threads  = max_threads;
    tpool->thread_count = min_threads;

    tpool->threads      = calloc(sizeof(void *), min_threads);
    tpool->iput_mutex   = zt_threads_alloc_lock(0);
    tpool->oput_mutex   = zt_threads_alloc_lock(0);
    tpool->iput_cond    = zt_threads_cond_alloc(0);
    tpool->oput_cond    = zt_threads_cond_alloc(0);

    if (pipe_iput) {
        pipe(tpool->iput_fd_sigs);
    } else {
        tpool->iput_fd_sigs[0] = -1;
        tpool->iput_fd_sigs[1] = -1;
    }

    if (pipe_oput) {
        pipe(tpool->oput_fd_sigs);
    } else {
        tpool->oput_fd_sigs[0] = -1;
        tpool->oput_fd_sigs[1] = -1;
    }

    TAILQ_INIT(&tpool->iput_queue);
    TAILQ_INIT(&tpool->oput_queue);

    return tpool;
}

#endif /* ifndef _ZT_DISABLE_THREAD_SUPPORT */

