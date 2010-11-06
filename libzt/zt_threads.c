#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>
#include <unistd.h>

#include <zt.h>
#include <zt_threads.h>

static void * _zt_threadpool_iput_loop(void *args);
static void * _zt_threadpool_oput_loop(void *args);

struct zt_threads_lock_callbacks  _zt_threads_lock_cbs = {
    NULL, NULL, NULL, NULL
};

struct zt_threads_cond_callbacks  _zt_threads_cond_cbs = {
    NULL, NULL, NULL, NULL
};

struct zt_threads_cntrl_callbacks _zt_threads_cntrl_cbs = {
    NULL, NULL, NULL, NULL, NULL
};

struct zt_threadpool_callbacks    _zt_threadpool_cbs = {
    _zt_threadpool_iput_loop, _zt_threadpool_oput_loop, NULL, NULL, NULL
};

zt_threads_thread               * (*zt_threads_alloc_thread)(void);

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

zt_threads_mutex *
zt_threads_alloc_lock(int type) {
    if (_zt_threads_lock_cbs.alloc) {
        return _zt_threads_lock_cbs.alloc(type);
    }

    return NULL;
}

void
zt_threads_free_lock(zt_threads_mutex * lock, int locktype) {
    if (_zt_threads_lock_cbs.free) {
        _zt_threads_lock_cbs.free(lock, locktype);
    } else {
        return;
    }
}

int
zt_threads_lock(int mode, zt_threads_mutex *lock) {
    if (_zt_threads_lock_cbs.lock) {
        return _zt_threads_lock_cbs.lock(mode, lock);
    }

    return 0;
}

int
zt_threads_unlock(int mode, zt_threads_mutex *lock) {
    if (_zt_threads_lock_cbs.unlock) {
        return _zt_threads_lock_cbs.unlock(mode, lock);
    }

    return 0;
}

zt_threads_cond *
zt_threads_cond_alloc(int type) {
    if (_zt_threads_cond_cbs.alloc) {
        return _zt_threads_cond_cbs.alloc(type);
    }

    return NULL;
}

void
zt_threads_cond_free(zt_threads_cond *cond) {
    if (_zt_threads_cond_cbs.free) {
        return _zt_threads_cond_cbs.free(cond);
    }
}

int
zt_threads_cond_signal(zt_threads_cond *cond, int broadcast) {
    if (_zt_threads_cond_cbs.signal) {
        return _zt_threads_cond_cbs.signal(cond, broadcast);
    }

    return 0;
}

int
zt_threads_cond_wait(zt_threads_cond *cond, zt_threads_mutex *lock, struct timeval *timeout) {
    if (_zt_threads_cond_cbs.wait) {
        return _zt_threads_cond_cbs.wait(cond, lock, timeout);
    }

    return 0;
}

int
zt_threads_start(zt_threads_thread *thread, zt_threads_cond *attr, void * (*cb)(void*), void *args) {
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
zt_threads_kill(zt_threads_thread *thread) {
    if (_zt_threads_cntrl_cbs.kill) {
        return _zt_threads_cntrl_cbs.kill(thread);
    }
    return 0;
}

int
zt_threads_join(zt_threads_thread *thread, void **data) {
    if (_zt_threads_cntrl_cbs.join) {
        return _zt_threads_cntrl_cbs.join(thread, data);
    }

    return 0;
}

unsigned long int
zt_threads_id(void) {
    if (_zt_threads_cntrl_cbs.id) {
        return _zt_threads_cntrl_cbs.id();
    }

    return 0;
}

/*****************************************************************
* threadpool defs
*****************************************************************/

void
zt_threadpool_disable_iput_loop(void) {
    _zt_threadpool_cbs.iput_loop = NULL;
}

void
zt_threadpool_disable_oput_loop(void) {
    _zt_threadpool_cbs.oput_loop = NULL;
}

int
zt_threadpool_set_callbacks(struct zt_threadpool_callbacks *cbs) {
    if (cbs == NULL) {
        /* set our defaults */
        _zt_threadpool_cbs.iput_loop   = _zt_threadpool_iput_loop;
        _zt_threadpool_cbs.oput_loop   = _zt_threadpool_oput_loop;
        _zt_threadpool_cbs.init        = NULL;
        _zt_threadpool_cbs.iput_worker = NULL;
        _zt_threadpool_cbs.oput_worker = NULL;

        return 0;
    }


    if (cbs->iput_loop) {
        _zt_threadpool_cbs.iput_loop = cbs->iput_loop;
    }

    if (cbs->oput_loop) {
        _zt_threadpool_cbs.oput_loop = cbs->oput_loop;
    }

    if (cbs->init) {
        _zt_threadpool_cbs.init = cbs->init;
    }

    if (cbs->iput_worker) {
        _zt_threadpool_cbs.iput_worker = cbs->iput_worker;
    }

    if (cbs->oput_worker) {
        _zt_threadpool_cbs.oput_worker = cbs->oput_worker;
    }

    if (cbs->finalize) {
        _zt_threadpool_cbs.finalize = cbs->finalize;
    }

    return 0;
} /* zt_threadpool_set_callbacks */

static void *
_zt_threadpool_oput_loop(void *args) {
    zt_threadpool *tpool;
    void          *init_data = NULL;

    tpool = (zt_threadpool*)args;

    if (_zt_threadpool_cbs.init) {
        init_data = _zt_threadpool_cbs.init(args);
    }

    while (1) {
        zt_threadpool_entry *entry = NULL;

        if (zt_threads_lock(0, tpool->oput_mutex)) {
            zt_threads_end(NULL);
        }

        if (TAILQ_EMPTY(&tpool->oput_queue)) {
            if (tpool->kill) {
                zt_threads_unlock(0, tpool->oput_mutex);
                zt_threads_end(NULL);
                return NULL;
            }
            zt_threads_cond_wait(tpool->oput_cond, tpool->oput_mutex, NULL);
            zt_threads_unlock(0, tpool->oput_mutex);
            continue;
        } else {
            entry = TAILQ_FIRST(&tpool->oput_queue);
            TAILQ_REMOVE(&tpool->oput_queue, entry, next);
        }

        if (entry == NULL) {
            if (tpool->kill == 1) {
                zt_threads_unlock(0, tpool->oput_mutex);
                zt_threads_end(NULL);
                return NULL;
            }
            zt_threads_unlock(0, tpool->oput_mutex);
            continue;
        }

        zt_threads_unlock(0, tpool->oput_mutex);

        if (_zt_threadpool_cbs.oput_worker) {
            void *input;

            input = _zt_threadpool_cbs.oput_worker(init_data, entry->data);

            if (input != NULL) {
                if (_zt_threadpool_cbs.finalize) {
                    _zt_threadpool_cbs.finalize(init_data, input);
                }
            }
        }

        if (entry != NULL) {
            free(entry);
        }
    }

    return NULL;
} /* _zt_threadpool_loop */


static void *
_zt_threadpool_iput_loop(void *args) {
    zt_threadpool *tpool;
    void          *init_data = NULL;

    tpool = (zt_threadpool*)args;

    if (_zt_threadpool_cbs.init) {
        init_data = _zt_threadpool_cbs.init(args);
    }

    while (1) {
        zt_threadpool_entry *entry = NULL;

        if (zt_threads_lock(0, tpool->iput_mutex)) {
            zt_threads_end(NULL);
        }

        if (TAILQ_EMPTY(&tpool->iput_queue)) {
            if (tpool->kill) {
                zt_threads_unlock(0, tpool->iput_mutex);
                zt_threads_end(NULL);
                return NULL;
            }
            zt_threads_cond_wait(tpool->iput_cond, tpool->iput_mutex, NULL);
            zt_threads_unlock(0, tpool->iput_mutex);
            continue;
        } else {
            entry = TAILQ_FIRST(&tpool->iput_queue);
            TAILQ_REMOVE(&tpool->iput_queue, entry, next);
        }

        if (entry == NULL) {
            if (tpool->kill == 1) {
                zt_threads_unlock(0, tpool->iput_mutex);
                zt_threads_end(NULL);
                return NULL;
            }

            zt_threads_unlock(0, tpool->iput_mutex);

            continue;
        }

        zt_threads_unlock(0, tpool->iput_mutex);

        if (_zt_threadpool_cbs.iput_worker) {
            void *input;

            input = _zt_threadpool_cbs.iput_worker(init_data, entry->data);

            if (input != NULL) {
                zt_threadpool_insert_oput(tpool, input);
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
    int                  i = 0;

    entry       = calloc(sizeof(zt_threadpool_entry), 1);
    entry->data = data;

    zt_threads_lock(0, tpool->iput_mutex);
    {
        TAILQ_INSERT_TAIL(&tpool->iput_queue, entry, next);
        zt_threads_cond_signal(tpool->iput_cond, 0);

        if (tpool->iput_fd_sigs[1] >= 0) {
            write(tpool->iput_fd_sigs[1], &i, 1);
        }
    }
    zt_threads_unlock(0, tpool->iput_mutex);

    return 0;
}

int
zt_threadpool_insert_oput(zt_threadpool *tpool, void *data) {
    zt_threadpool_entry *entry;
    int                  i = 0;

    entry       = calloc(sizeof(zt_threadpool_entry), 1);
    entry->data = data;

    zt_threads_lock(0, tpool->oput_mutex);
    {
        TAILQ_INSERT_TAIL(&tpool->oput_queue, entry, next);
        zt_threads_cond_signal(tpool->oput_cond, 0);

        if (tpool->oput_fd_sigs[1] >= 0) {
            write(tpool->oput_fd_sigs[1], &i, 1);
        }
    }
    zt_threads_unlock(0, tpool->oput_mutex);

    return 0;
}

void *
zt_threadpool_get_oput(zt_threadpool *tpool) {
    zt_threadpool_entry *entry;
    void                *data;

    zt_threads_lock(0, tpool->oput_mutex);

    do {
        if (TAILQ_EMPTY(&tpool->oput_queue)) {
            break;
        }

        entry = TAILQ_FIRST(&tpool->oput_queue);
        TAILQ_REMOVE(&tpool->oput_queue, entry, next);
    } while (0);

    zt_threads_unlock(0, tpool->oput_mutex);

    if (entry == NULL) {
        return NULL;
    }

    data = entry->data;
    free(entry);

    return data;
}

int
zt_threadpool_start(zt_threadpool *tpool) {
    int i;

    if (_zt_threadpool_cbs.iput_loop) {
        for (i = 0; i < tpool->min_threads; i++) {
            tpool->iput_threads[i] = zt_threads_alloc_thread();
            zt_threads_start(tpool->iput_threads[i], NULL,
                             _zt_threadpool_cbs.iput_loop, (void*)tpool);
        }
    }

    if (_zt_threadpool_cbs.oput_loop) {
        for (i = 0; i < tpool->min_threads; i++) {
            tpool->oput_threads[i] = zt_threads_alloc_thread();
            zt_threads_start(tpool->oput_threads[i], NULL,
                             _zt_threadpool_cbs.oput_loop, (void*)tpool);
        }
    }

    return 0;
}

int
zt_threadpool_iput_fd_reader(zt_threadpool *tpool) {
    return tpool->iput_fd_sigs[0];
}

int
zt_threadpool_iput_fd_writer(zt_threadpool *tpool) {
    return tpool->iput_fd_sigs[1];
}

int
zt_threadpool_oput_fd_reader(zt_threadpool *tpool) {
    return tpool->oput_fd_sigs[0];
}

int
zt_threadpool_oput_fd_writer(zt_threadpool *tpool) {
    return tpool->oput_fd_sigs[1];
}

int
zt_threadpool_kill(zt_threadpool *tpool) {
    int i;
    int k = 1;

    tpool->kill = 1;

    /* signal all threads, and let them die */
    zt_threads_cond_signal(tpool->iput_cond, 1);
    zt_threads_cond_signal(tpool->oput_cond, 1);

    for (i = 0; i < tpool->min_threads; i++) {
        if (_zt_threadpool_cbs.iput_loop) {
            zt_threads_join(tpool->iput_threads[i], NULL);
        }

        if (_zt_threadpool_cbs.oput_loop) {
            zt_threads_join(tpool->oput_threads[i], NULL);
        }
    }

    if (tpool->oput_fd_sigs[1] >= 0) {
        write(tpool->oput_fd_sigs[1], &k, 1);
    }

    if (tpool->iput_fd_sigs[1] >= 0) {
        write(tpool->iput_fd_sigs[1], &i, 1);
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

    tpool->iput_threads = calloc(sizeof(zt_threads_thread *), min_threads);
    tpool->iput_mutex   = zt_threads_alloc_lock(0);
    tpool->oput_mutex   = zt_threads_alloc_lock(0);
    tpool->iput_cond    = zt_threads_cond_alloc(0);
    tpool->oput_cond    = zt_threads_cond_alloc(0);

    if (_zt_threadpool_cbs.oput_loop) {
        tpool->oput_threads = calloc(sizeof(zt_threads_thread *), min_threads);
    }

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

