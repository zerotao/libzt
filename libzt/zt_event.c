#include <libzt/zt.h>
#include <libzt/zt_assert.h>

#include "zt_event.h"
#include "zt_event/private.h"

int zt_event_register_io(zt_event_sys sys, int fd, zt_event_enum type, zt_event_io_cb cb, void *data)
{
	assert(sys);
	if(sys->vtbl->register_io){
		return sys->vtbl->register_io(sys, fd, type, cb, data);
	}
	
	return -1;
}

int zt_event_remove_io(zt_event_sys sys, int fd, zt_event_enum type)
{
	assert(sys);
	if(sys->vtbl->remove_io){
		return sys->vtbl->remove_io(sys, fd, type);
	}
	
	return -1;
}


int zt_event_register_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data)
{
	assert(sys);
	if(sys->vtbl->register_signal){
		return sys->vtbl->register_signal(sys, signal, cb, data);
	}
	
	return -1;
}

int zt_event_remove_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data)
{
	assert(sys);
	if(sys->vtbl->remove_signal){
		return sys->vtbl->remove_signal(sys, signal, cb, data);
	}
	
	return -1;
}


int zt_event_register_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data)
{
	assert(sys);
	if(sys->vtbl->register_timer){
		return sys->vtbl->register_timer(sys, time, cb, data);
	}
	
	return -1;
}

int zt_event_remove_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data)
{
	assert(sys);
	if(sys->vtbl->remove_timer){
		return sys->vtbl->remove_timer(sys, time, cb, data);
	}
	
	return -1;
}


int zt_event_destroy(zt_event_sys sys)
{
	assert(sys);
	if(sys->vtbl->destroy){
		sys->vtbl->destroy(sys);
		return 0;
	}
	
	return -1;
}

int zt_event_run(zt_event_sys sys, zt_event_flags flags)
{
	assert(sys);
	if(sys->vtbl->handle_events){
		return sys->vtbl->handle_events(sys, flags);
	}
	
	return -1;
}

