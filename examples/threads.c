#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <zt.h>

zt_threads_mutex *mutex;

void *
example_thread_cb(void *_data) {
    char *data = (char *)_data;

    while (1) {
        /* lock the mutex */
        zt_threads_lock(0, mutex);
        printf("id=%lu data=%s\n", zt_threads_id(), data);
        zt_threads_unlock(0, mutex);
        usleep(80000);
    }

    zt_threads_end(NULL);
    return NULL;
}


void *
example_init_cb(void *args UNUSED) {
    return (void *)"example init data";
}

void *
example_iput_work_cb(void *init_data, void *data) {
    printf("iput_worker: id = %lu init_data = %s, data = %s\n",
           zt_threads_id(), (char *)init_data, (char *)data);
    return "sending to oput";
}

void *
example_oput_work_cb(void *init_data UNUSED, void *data) {
    printf("oput_worker: id = %lu data = %s\n",
           zt_threads_id(), (char *)data);
    return "sending to finalizer";
}

void example_finalizer(void *init_data UNUSED, void *data) {
    printf("finalized: %s\n", (char *)data);
}

int main(int argc UNUSED, char **argv UNUSED) {
    /* tell libzt to use pthreads */
    zt_threads_use_pthreads();

    int   i;

    zt_threads_thread *thread1;
    zt_threads_thread *thread2;

    /* allocate a mutex */
    mutex   = zt_threads_alloc_lock(0);

    /* allocate some threads */
    thread1 = zt_threads_alloc_thread();
    thread2 = zt_threads_alloc_thread();

    /* start up the threads */
    zt_threads_start(thread1, NULL, example_thread_cb, (void *)"one");
    zt_threads_start(thread2, NULL, example_thread_cb, (void *)"two");

    /* loop for a few */
    for (i = 0; i <= 2; i++) {
        printf("parent...\n");
        sleep(1);
    }

    zt_threads_kill(thread1);
    zt_threads_kill(thread2);

    /* lets do some threadpools */
    {
        zt_threadpool                 *tpool;

        struct zt_threadpool_callbacks tpcbs = {
            NULL, /* input queue looper */
            NULL, /* output queue looper */
            example_init_cb, /* thread data initializer */
            example_iput_work_cb, /* processes data from an input queue */
            example_oput_work_cb, /* processes data from an output queue */
            example_finalizer /* called after oput worker to finalize data */
        };

        /* set our callbacks, leave the threadpool looper function to default (NULL) */
        zt_threadpool_set_callbacks(&tpcbs);

        /* create a threadpool with five listeners, also don't create
         * pipe based signaling */
        tpool = zt_threadpool_init(5, 5, 0, 0);

        /* start up the threadpool */
        printf("-------- starting test threadpool -------------\n");
        zt_threadpool_start(tpool);

        for (i = 0; i <= 20; i++) {
            zt_threadpool_insert_iput(tpool, (void *)"blah");
            usleep(90000);
        }
    }

    return 0;
} /* main */

