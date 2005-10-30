#include <libzt/zt_assert.h>
#include <libzt/zt_time.h>
#include "gtimer.h"


struct zt_timer_node *
gtimer_new(struct timeval *when, struct timeval *otime, zt_event_timer_cb *cb, void *data, zt_event_timer_flags flags) 
{
	struct zt_timer_node	* new;
	assert(when);
	
	new = XCALLOC(struct zt_timer_node, 1);
	new->flags = flags;

	zt_copy_timeval(&new->when, when);
	zt_copy_timeval(&new->otime, otime);
	
	new->cb = cb;
	new->data = data;

	return new;
}

void
gtimer_free(struct zt_timer_node *node) 
{
	assert(node);
	
	XFREE(node);
}
