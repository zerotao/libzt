#include <string.h>

#include "zt_cothread.h"

static int _schedule(zt_cothread_sched *);
static void _add_req(zt_cothread_sched *, struct event_req *, int, ...);
static void _vadd_req(zt_cothread_sched *, struct event_req *, int, va_list);
static int _check(struct event_req *, struct timeval *);
static void _enqueue(zt_cothread_sched *, struct event_queue *, struct event_req *);
static void _enqueue_timer(zt_event_sys, struct timeval *, void *);
static void _enqueue_io(zt_event_sys, int, zt_event_enum, void *);
static struct timeval *_timeout2timeval(struct timeval *, int);


zt_cothread_sched *
zt_cothread_sched_new(void)
{
	zt_cothread_sched	* cts;

	cts = XCALLOC(zt_cothread_sched, 1);
	
	cts->active = cts->queues;
	cts->wait = cts->queues + 1;
	memset(cts->queues, 0, sizeof(cts->queues));
	
	cts->sys = zt_event_select();
	cts->hevents = 0;
	cts->revents = 0;
	cts->event_flags = (ZT_ALL_EVENTS|ZT_RUN_ONCE);
	
	memcpy(&cts->ctime, zt_event_tod(cts->sys), sizeof(struct timeval));	
	return cts;
}

void
zt_cothread_sched_delete(zt_cothread_sched *cts)
{
	memset(cts, 0, sizeof(zt_cothread_sched));
	XFREE(cts);
}

zt_cothread *
zt_cothread_new(zt_cothread_sched *cts, void *(*func)(), int stacksize, ...) 
{
	zt_cothread			* co;
	va_list			  	  args;
	struct event_req	  req[1];
	
	_add_req(cts, req, ZT_TIMER_EVENT, 0);
	va_start(args, stacksize);

	if((co = zt_coro_create(func, 0, stacksize))) {
		zt_coro_call(co, args);
	}

	va_end(args);
	return co;
}

int
zt_cothread_wait(zt_cothread_sched *cts, int mode, ...) 
{
	va_list			  args;
	struct event_req	  req[1];

	va_start(args, mode);

	_vadd_req(cts, req, mode, args);
	va_end(args);
	
	return _schedule(cts);
}

void
zt_cothread_join(zt_cothread_sched *cts)
{
	while(! zt_cothread_sched_empty(cts)) {
		zt_cothread_wait(cts, ZT_TIMER_EVENT, 10);
	}
}

/* local functions */

static int _schedule(zt_cothread_sched *cts)
{
	struct event_queue	* queue;
	struct event_req	* req;
	int					  result;

	for(;;){
		
		queue = cts->active;
		while((req = queue->req)) { /* for each request in the queue */
			queue->req = req->next;
			if(req->mode & ZT_ANY_IO_EVENT) {
				zt_event_remove_io(cts->sys, req->fd, req->mode);
			}
			
			zt_coro_call(req->coro, NULL); /* call it */
			return -1;					   /* returning to my parent */
		}
		
		while(zt_event_run(cts->sys, cts->event_flags) == -1);
		memcpy(&cts->ctime, zt_event_tod(cts->sys), sizeof(struct timeval));
	}

	return 0;
}

static void _add_req(zt_cothread_sched *cts, struct event_req *req, int mode, ...)
{
	va_list	  args;

	va_start(args, mode);
	_vadd_req(cts, req, mode, args);
	va_end(args);
}

static void _vadd_req(zt_cothread_sched *cts, struct event_req *req, int mode, va_list args)
{
	req->coro = zt_coro_get_current();
	req->mode = mode;
	if(mode & ZT_ANY_IO_EVENT) {
		req->fd = va_arg(args, int);
	}
	if(mode & ZT_TIMER_EVENT) {
		zt_add_time(&req->timeout,
			    _timeout2timeval(&req->timeout, va_arg(args, int)),
			    &cts->ctime);

	}
	_enqueue(cts, cts->wait, req);
}

static int _check(struct event_req *req, struct timeval *ctime)
{
#define tst_fde(r, e) (((r)->mode & (e)) && ((r)->rmode & (e))) ? e : 0
	int	  res = 0;

	if(req->mode & ZT_ANY_IO_EVENT) {
		res |= tst_fde(req, ZT_READ_EVENT);
		res |= tst_fde(req, ZT_WRITE_EVENT);
		res |= tst_fde(req, ZT_EXCEPT_EVENT);
		req->rmode = 0;
	}

	if (res == 0) {
		if (req->mode & ZT_TIMER_EVENT) {			
			if (zt_cmp_time(&req->timeout, ctime) <= 0) {
				res |= ZT_TIMER_EVENT;
			}
		}
	}
	return res;
#undef tst_fde
}

static void _enqueue(zt_cothread_sched *cts, struct event_queue *queue, struct event_req *req)
{

	req->cts = cts;
	if (req->mode & ZT_ANY_IO_EVENT) {
		zt_event_register_io(cts->sys,
				     req->fd,
				     req->mode,
				     _enqueue_io, req);
		cts->revents++;
		req->rmode = 0;
	}

	if (req->mode & ZT_TIMER_EVENT) {
		zt_event_register_timer(cts->sys,
					&req->timeout,
					_enqueue_timer,
					req, (ZT_EVENT_TIMER_ONCE |
					      ZT_EVENT_TIMER_ABSOLUTE));
		cts->revents++;
		/* 
                 * if(!queue->mintime || zt_cmp_time(queue->mintime, &req->timeout) > 0) {
		 * 	queue->mintime = &req->timeout;
		 * }
                 */
	}
}

static void _enqueue_timer(zt_event_sys sys, struct timeval *tv, void *data)
{
	struct event_req		* req = (struct event_req *)data;
	struct zt_cothread_sched	* cts;

	cts = req->cts;
	
	req->next = cts->active->req;
	cts->active->req = req;
	cts->hevents++;
}

static void _enqueue_io(zt_event_sys sys, int fd, zt_event_enum type, void *data)
{
	struct event_req			* req = (struct event_req *)data;
	struct zt_cothread_sched	* cts;

	cts = req->cts;
	
	req->next = cts->active->req;
	req->rmode |= type;
	cts->active->req = req;
	cts->hevents++;
	
}

static struct timeval *_timeout2timeval(struct timeval *tv, int to)
{
	if (to <= 0){
		tv->tv_sec = 0;
		tv->tv_usec = 0;
	} else {
		tv->tv_sec = to/1000;
		tv->tv_usec = to%1000 * 1000;
	}
	
	return tv;
}

