#ifndef _zt_cothread_h_
#define _zt_cothread_h_

#include <stdarg.h>

#include <libzt/zt.h>
#include <libzt/zt_event.h>
#include <libzt/zt_coro.h>

BEGIN_C_DECLS

typedef zt_coro zt_cothread;
typedef struct zt_cothread_sched zt_cothread_sched;

struct event_req {
	struct event_req	* next;		/* next iorequest */
	zt_coro				* coro;		/* coroutine waiting for event */
	int			  		  mode;		/* events we are waiting for */
	int			  		  rmode;	/* events we have received */
	int			  		  fd;       /* (optional) file descriptor we are waiting on */
	struct timeval		  timeout;  /* (optional) timeout */

	/*
	 * pointer back to my scheduler.  This is here to avoid the
	 * overhead of allocating a new wrapper data structure for
	 * _enqueue_io and _enqueue_timer, it is also the location of the zt_coro_ctl pointer
	 */
	struct zt_cothread_sched	* cts;

};
	
struct event_queue {
	struct event_req	* req;        /* first request in this queue */
	struct timeval		* mintime;    /* time of earilest (next) request */
};

struct zt_cothread_sched {
	zt_event_sys	  	  sys;	/* event system queue */
	long				  revents;
	long				  hevents;
	int			  		  event_flags;
	zt_coro_ctx			  coro_ctx;		/* pointer to the coroutine contol structure */
	
	struct timeval		  ctime;
	struct event_queue	* active;
	struct event_queue	* wait;
	struct event_queue	  queues[2];
};

zt_cothread_sched *zt_cothread_sched_new(void);
void zt_cothread_sched_delete(zt_cothread_sched *);

zt_cothread *zt_cothread_new(zt_cothread_sched *, void *(*)(), int, ...);
int zt_cothread_wait(zt_cothread_sched *, int, ...);

void zt_cothread_join(zt_cothread_sched *cts);

#define zt_cothread_sched_empty(cts) (((cts->active->req == 0) && (cts->wait->req == 0)) && (cts->revents == cts->hevents))

END_C_DECLS

#endif /* _zt_cothread_h_ */
