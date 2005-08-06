#include <libzt/zt_assert.h>
#include "gtimer.h"


struct zt_timer_node *
gtimer_new(struct timeval *when, zt_event_timer_cb *cb, void *data) 
{
	struct zt_timer_node	* new;
	assert(when);
	
	new = XCALLOC(struct zt_timer_node, 1);
	new->when.tv_sec = when->tv_sec;
	new->when.tv_usec = when->tv_usec;

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
