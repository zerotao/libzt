#ifndef __ZT_THREADS_H__
#define __ZT_THREADS_H__
/* includes */

#define ZT_THREADS_LOCK_BLOCKING    (0 << 0)
#define ZT_THREADS_LOCK_NONBLOCKING (1 << 0)
#define ZT_THREADS_LOCK_DUMMY       (1 << 1)

#ifndef _ZT_DISABLE_THREAD_SUPPORT
#include <sys/queue.h>

typedef struct zt_threadpool zt_threadpool;
typedef struct zt_threadpool_entry zt_threadpool_entry;

typedef void zt_threads_mutex;
typedef void zt_threads_cond;
typedef void zt_threads_thread;
typedef void zt_threads_attr;

struct zt_threads_lock_callbacks {
    void * (*alloc)(int locktype);
    void   (*free)(zt_threads_mutex * lock, int locktype);
    int    (*lock)(int mode, zt_threads_mutex *lock);
    int    (*unlock)(int mode, zt_threads_mutex *lock);
};

struct zt_threads_cond_callbacks {
    zt_threads_cond * (*alloc)(int condtype);
    void              (*free)(zt_threads_cond *cond);
    int               (*signal)(zt_threads_cond *cond, int broadcast);
    int               (*wait)(zt_threads_cond *cond, zt_threads_mutex *lock, struct timeval *timeout);
};

struct zt_threads_cntrl_callbacks {
    int               (*start)(zt_threads_thread *thread, zt_threads_attr *attr, void * (*start_cb)(void *), void *arg);
    void              (*end)(void *arg);
    int               (*kill)(zt_threads_thread *thread);
    unsigned long int (*id)(void);
    int               (*join)(zt_threads_thread *thread, void **data);
};

struct zt_threadpool_callbacks {
    /* the function that reads from the queue */
    void * (*iput_loop)(void *args);
    void * (*oput_loop)(void *args);

    /* the function to call before going into the loop, the output
     * of this data will be sent to the worker callback  */
    void * (*init)(void *args);

    /* the function to call when data is avail,
     * if this function returns 0 nothing is done,
     * if function returns 1, the output will be
     * inserted into the oput_queue
     */
    /* the input worker is fed data from the input queue,
     * if the return value is not NULL, the returned data
     * is placed in the output queue */
    void *(*iput_worker)(void *init_data, void *data);

    /* the output worker is fed data via the output queue
     * if the return value is not NULL, the finalize data
     * function will be called */
    void *(*oput_worker)(void *init_data, void *data);

    /* a function which is called after the oput worker is
     * done processing the data */
    void (*finalize)(void *init_data, void *data);
};

struct zt_threadpool_entry {
    void *data;

    TAILQ_ENTRY(zt_threadpool_entry) next;
};

struct zt_threadpool {
    zt_threads_thread **iput_threads;
    zt_threads_thread **oput_threads;
    zt_threads_mutex   *iput_mutex;
    zt_threads_mutex   *oput_mutex;
    zt_threads_cond    *iput_cond;
    zt_threads_cond    *oput_cond;

    /* used to signal things via a pipe */
    int iput_fd_sigs[2];
    int oput_fd_sigs[2];

    int min_threads;
    int max_threads;
    int thread_count;

    int kill;

    TAILQ_HEAD(, zt_threadpool_entry) iput_queue;
    TAILQ_HEAD(, zt_threadpool_entry) oput_queue;
};

extern struct zt_threads_lock_callbacks  _zt_threads_lock_cbs;
extern struct zt_threads_cond_callbacks  _zt_threads_cond_cbs;
extern struct zt_threads_cntrl_callbacks _zt_threads_cntrl_cbs;
extern zt_threads_thread               * (*zt_threads_alloc_thread)(void);

/* module declarations */
int zt_threads_set_lock_callbacks(struct zt_threads_lock_callbacks *);
int zt_threads_set_cond_callbacks(struct zt_threads_cond_callbacks *);
int zt_threads_set_cntrl_callbacks(struct zt_threads_cntrl_callbacks *);

zt_threads_mutex *zt_threads_alloc_lock(int type);
void  zt_threads_free_lock(void *lock, int type);
int   zt_threads_lock(int mode, void *lock);
int   zt_threads_unlock(int mode, void *lock);
zt_threads_cond *zt_threads_cond_alloc(int type);
void  zt_threads_cond_free(void *cond);
int   zt_threads_cond_signal(void *cond, int broadcast);
int   zt_threads_cond_wait(void *cond, void *lock, struct timeval *timeout);
int   zt_threads_start(void *thread, void *attr, void * (*start_cb)(void *), void *arg);
void  zt_threads_end(void *args);
int   zt_threads_kill(void *thread);
int   zt_threads_join(void *thread, void **data);
unsigned long int zt_threads_id(void);

#if defined(_ZT_THREADS_HAVE_PTHREADS)
#define ZT_THREADS_PTHREADS_IMPLEMENTED 1
int zt_threads_use_pthreads(void);
#endif

/* threadpool defs */
extern struct zt_threadpool_callbacks _zt_threadpool_callbacks;
zt_threadpool *zt_threadpool_init(int min_threads, int max_threads, int pipe_iput, int pipe_oput);
int zt_threadpool_start(zt_threadpool *tpool);
int zt_threadpool_set_callbacks(struct zt_threadpool_callbacks *cbs);
int zt_threadpool_insert_iput(zt_threadpool *, void *);
int zt_threadpool_insert_oput(zt_threadpool *, void *);
void zt_threadpool_disable_iput_loop(void);
void zt_threadpool_disable_oput_loop(void);

int zt_threadpool_iput_fd_reader(zt_threadpool *tpool);
int zt_threadpool_oput_fd_reader(zt_threadpool *tpool);
int zt_threadpool_iput_fd_writer(zt_threadpool *tpool);
int zt_threadpool_oput_fd_writer(zt_threadpool *tpool);

void * zt_threadpool_get_oput(zt_threadpool *tpool);
int zt_threadpool_kill(zt_threadpool *tpool);

#endif /* _ZT_DISABLE_THREAD_SUPPORT */

#endif /* __ZT_THREADS_H__ */

