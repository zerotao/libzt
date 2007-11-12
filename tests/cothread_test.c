#include <string.h>

#include <stdarg.h>
#include <libzt/zt_cothread.h>
#include <stdlib.h>
#include <libzt/zt_unit.h>


struct glbl {
	zt_cothread_sched	* cts;
}glbl;

int loops = 0;
int inits = 0;

#define STACK_SIZE ZT_CORO_MIN_STACK_SIZE
#define CORO_N 100
#define DEBUG 0

static void *test1(va_list args) {
	char *str = va_arg(args, char *);
	int limit = va_arg(args, int);
	int i = 0;
	
	inits++;
	/* We have been called initially, so we want to wait for a little
	 * while, charging the schedular with a large number of threads.
	 * we do a little bit of magic with the wait time here to "try" to
	 * make the semantics work, basicly I want to wait until all of
	 * the threads are started before I start to process them as the
	 * schedular does not have the concept, yet, of enabled/disabled I
	 * try to give enough time, in usec, for all of the threads to get
	 * started (# of threads / 2) before we begin running.  This is
	 * obviosuly flawed, but works most of the time.
	 */
	zt_cothread_wait(glbl.cts, ZT_TIMER_EVENT, CORO_N/2);
	while (i < limit) {
		loops++;
		zt_cothread_wait(glbl.cts, ZT_TIMER_EVENT, 1); /* 0 = schedule for the next spin */
		i++;
	}
	
	zt_cothread_wait(glbl.cts, 0); /* request a delete of the cothread */
	return NULL;
}


static void *test2(va_list args) {
	char *str = va_arg(args, char *);
	int in = va_arg(args, int);
	int out = va_arg(args, int);
	char buf[256];
	int n;

	for (;;) {
		zt_cothread_wait(glbl.cts, ZT_READ_EVENT, in);
		if ((n = read(in, buf, sizeof(buf))) <= 0)
			break;
		
		zt_cothread_wait(glbl.cts, ZT_WRITE_EVENT, out);
		write(out, buf, n);
	}
	
	zt_cothread_wait(glbl.cts, 0);
	return NULL;
}

static void
basic_tests(struct zt_unit_test *test, void *data)
{
	int			  i;
	
	glbl.cts = zt_cothread_sched_new();

	/*
	 * Make sure that the event system does not block waiting for an event
	 * to fire.  This way we can load up the system with a large number
	 * of "threads" before we start "running".  This results in higher CPU
	 * load as we spin, in the for loop and request new threads, but keeps from
	 * blocking on each one.
	 */
	glbl.cts->event_flags |= ZT_NON_BLOCK;

#define YIELDS_PER_THREAD 5
	for(i=0; i < CORO_N; i++) {
		zt_cothread_new(glbl.cts, test1, STACK_SIZE, "test_thread", YIELDS_PER_THREAD);
	}

#if DEBUG
	printf("request: %d handled: %d\n", glbl.cts->revents, glbl.cts->hevents);
#endif
	
	ZT_UNIT_ASSERT(test, inits == CORO_N);
	ZT_UNIT_ASSERT(test, glbl.cts->hevents > 0);
	
	/*
	 * Now we block waiting for events to fire,
	 * this will reduce the CPU load and system call overhead as there
	 * is nothing to be doing while the threads are running other then
	 * loop back on it's self.  this is what should be done when using
	 * the parent workerthread model (one parent that wraps multiple
	 * child threads).
	 */
	glbl.cts->event_flags &= ~(ZT_NON_BLOCK);

	/* Wait for all events to complete */
	zt_cothread_join(glbl.cts);	
	
	ZT_UNIT_ASSERT(test, glbl.cts->revents == glbl.cts->hevents);
	
	ZT_UNIT_ASSERT(test, CORO_N * YIELDS_PER_THREAD == loops);
	ZT_UNIT_ASSERT(test, zt_cothread_sched_empty(glbl.cts) == 1);

#if DEBUG
	printf("coro * yields: %d\n", CORO_N * YIELDS_PER_THREAD);
	printf("loops: %d\n", loops);
		
	printf("%ld events registered\n", glbl.cts->revents);
	printf("%ld events handled\n", glbl.cts->hevents);
#endif
}

int
register_cothread_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "cothread tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}

