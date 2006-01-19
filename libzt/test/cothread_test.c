#include <string.h>

#include <stdarg.h>
#include <libzt/zt_cothread.h>
#include "test.h"


struct glbl {
	zt_cothread_sched	* cts;
}glbl;

static void *test1(va_list args) {
	char *str = va_arg(args, char *);
	int limit = va_arg(args, int);
	int i = 0;

	//printf("%s started\n", str);
	while (i < limit) {
		zt_cothread_wait(glbl.cts, ZT_TIMER_EVENT, 100 * limit);
		i++;
		//printf("%s: %d\n", str, i++);
	}
	//printf("%s: dying\n", str);
	zt_cothread_wait(glbl.cts, 0);
        return NULL;
}


static void *test2(va_list args) {
	char *str = va_arg(args, char *);
	int in = va_arg(args, int);
	int out = va_arg(args, int);
	char buf[256];
	int n;

	printf("%s started\n", str);
	for (;;) {
		zt_cothread_wait(glbl.cts, ZT_READ_EVENT, in);
		if ((n = read(in, buf, sizeof(buf))) <= 0)
			break;
		printf("test2 writing\n");
		zt_cothread_wait(glbl.cts, ZT_WRITE_EVENT, out);
		write(out, buf, n);
	}
	printf("%s: dying\n", str);
	zt_cothread_wait(glbl.cts, 0);
        return NULL;
}

#define CORO_N 1000
int main(int argc, char **argv) {
	char			  name[CORO_N + 1][256];
	int			  i;
	
	glbl.cts = zt_cothread_sched_new();

	/*
	 * setting this here makes sure 
	 */
	glbl.cts->event_flags |= ZT_NON_BLOCK;

	printf("Testing %d co_threads\n", CORO_N);	
	for(i=0; i < CORO_N; i++) {
		sprintf(name[i], "test[%d]", i);
		zt_cothread_new(glbl.cts, test1, 32768, name[i], 15);
	}	
	/* 
         * cothread_new(test1, "test1a", 10);
	 * cothread_new(test1, "test1b", 12);
	 * cothread_new(test1, "test1c", 14);
	 * cothread_new(test2, "test2", 0, 2);
         */
	printf("Init Complete\n");
	
	/*
	 * this will reduce the CPU load and system call overhead as
	 * there is nothing to be doing while the threads are running
	 * other then loop back on it's self
	 */
	glbl.cts->event_flags &= ~(ZT_NON_BLOCK);

	while(zt_event_num_events(glbl.cts->sys) > 0) {
	  printf("main: waiting on %ld cothreads...\n", zt_event_num_events(glbl.cts->sys));
		zt_cothread_wait(glbl.cts, ZT_TIMER_EVENT, 3000);
	}
	printf("%ld events registered\n", glbl.cts->revents);
	printf("%ld events handled\n", glbl.cts->hevents);
}


