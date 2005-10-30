#include <libzt/zt.h>
#include <libzt/zt_log.h>

#include "private.h"

static void destroy( zt_event_sys sys)
{
	XFREE(sys);
}


static int
register_io(zt_event_sys sys, int fd, zt_event_enum type, zt_event_io_cb cb, void *data)
{
	return 0;
}

static int
remove_io(zt_event_sys sys, int fd, zt_event_enum type)
{
	return 0;
}

	
static int
register_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data)
{
	return 0;
}

static int
remove_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data)
{
	return 0;
}

	
static int
register_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data, zt_event_timer_flags flags)
{
	return 0;
}

static int
remove_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data)
{
	return 0;
}

static int
handle_events(zt_event_sys sys, zt_event_flags flags)
{
	return 0;
}


static zt_event_vtbl vtbl = {
	sizeof(zt_event_sys),
	register_io,
	remove_io,
	register_signal,
	remove_signal,
	register_timer,
	remove_timer,
	handle_events,
	destroy,
};

zt_event_sys 
zt_event_kqueue(void) 
{
	
	log_printf(log_err, "Unsupported interface: kqueue");
	return NULL;
	return zt_event_new(&vtbl);
}


