#ifndef _EVENT_GTIMER_H_
#define _EVENT_GTIMER_H_

#include <zt.h>
#include <libzt/zt_event.h>
#include <libzt/zt_time.h>
#include <libzt/adt/zt_tree.h>

BEGIN_C_DECLS

struct zt_timer_node {
	zt_rbt_node		  node;

	struct timeval		  when;
	zt_event_timer_cb	* cb;
	void			* data;
};

extern struct zt_timer_node *
gtimer_new(struct timeval *when, zt_event_timer_cb *cb, void *data);

void
gtimer_free(struct zt_timer_node *node);


END_C_DECLS
#endif /* _EVENT_GTIMER_H_ */
