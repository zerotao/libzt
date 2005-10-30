#include <libzt/zt.h>
#include <libzt/zt_assert.h>

#include "zt_event.h"
#include "zt_event/private.h"

int zt_event_register_io(zt_event_sys sys, int fd, zt_event_enum type, zt_event_io_cb cb, void *data)
{
	int	  ret;
	assert(sys);
	if(sys->vtbl->register_io){
		if((ret = sys->vtbl->register_io(sys, fd, type, cb, data)) == 0) {
			sys->num_events++;
		}
		return ret;
	}
	
	return -1;
}

int zt_event_remove_io(zt_event_sys sys, int fd, zt_event_enum type)
{
	int	  ret = 0;
	
	assert(sys);
	if(sys->vtbl->remove_io){
		if((ret = sys->vtbl->remove_io(sys, fd, type)) == 0){
			sys->num_events--;
		}
		return ret;
	}
	
	return -1;
}


int zt_event_register_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data)
{
	int	  ret = 0;
	
	assert(sys);
	if(sys->vtbl->register_signal){
		if((ret = sys->vtbl->register_signal(sys, signal, cb, data)) == 0){
			sys->num_events++;
		}
		return ret;
	}
	
	return -1;
}

int zt_event_remove_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data)
{
	int	  ret = 0;
	
	assert(sys);
	if(sys->vtbl->remove_signal){
		if((ret = sys->vtbl->remove_signal(sys, signal, cb, data)) == 0){
			sys->num_events--;
		}
		return ret;
	}
	
	return -1;
}


int zt_event_register_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data, zt_event_timer_flags flags)
{
	int	  ret = 0;
	
	assert(sys);
	if(sys->vtbl->register_timer){
		if((ret = sys->vtbl->register_timer(sys, time, cb, data, flags)) == 0){
			sys->num_events++;
		}
		
		return ret;
	}
	
	return -1;
}

int zt_event_remove_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data)
{
	int	  ret = 0;
	
	assert(sys);
	if(sys->vtbl->remove_timer){
		if((ret = sys->vtbl->remove_timer(sys, time, cb, data)) == 0){
			sys->num_events--;
		}
		return ret;
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

long zt_event_num_events(zt_event_sys sys)
{
	assert(sys);
	return sys->num_events;
}

struct timeval* zt_event_tod(zt_event_sys sys)
{
	assert(sys);
	return &sys->tod;
}

