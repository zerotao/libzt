#ifndef _EVENT_PRIVATE_H_
#define _EVENT_PRIVATE_H_

#include <libzt/zt.h>
#include <libzt/zt_event.h>
#include <libzt/adt/zt_table.h>
#include <libzt/adt/zt_queue.h>
#include <libzt/zt_time.h>

BEGIN_C_DECLS

struct zt_event_sys {
	struct zt_event_vtbl	* vtbl;

	/* other options */
	long			  num_events;
	struct timeval		  tod;
	struct timeval		  min_timeout;
};

typedef struct zt_event_vtbl {
	size_t	  size;
	/* virtual functions */

	int (* register_io)(zt_event_sys sys, int fd, zt_event_enum type, zt_event_io_cb cb, void *data);
	int (* remove_io)(zt_event_sys sys, int fd, zt_event_enum type);
	
	int (* register_signal)(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data);
	int (* remove_signal)(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data);
	
	int (* register_timer)(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data, zt_event_timer_flags flags);
	int (* remove_timer)(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data);
	
	int  (* handle_events)(zt_event_sys sys, zt_event_flags flags);
	void (* destroy)(zt_event_sys sys);

} zt_event_vtbl;

zt_event_sys zt_event_new(zt_event_vtbl *vtbl);
long zt_event_num_events(zt_event_sys sys);
struct timeval *zt_event_tod(zt_event_sys sys);

END_C_DECLS
#endif /* _EVENT_PRIVATE_H_ */
