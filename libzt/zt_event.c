/*
 * zt_event.c	event support and main loop
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "zt_event.h"
#include "zt_event/zt_event_private.h"

/*
 * interface to the underling event
 */
int
zt_event_init( zt_event_ty *evnt )
{
	if (evnt && evnt->init) {
		return evnt->init(evnt);
	}
	
	return 1;
}

int
zt_event_destroy( zt_event_ty *evnt )
{
	if(evnt && evnt->destructor) {
		return evnt->destructor(evnt);
	}
	
	return 1;
}

int
zt_event_dispatch( zt_event_ty *evnt )
{
	if(evnt && evnt->dispatch) {
		return evnt->dispatch(evnt);
	}
	
	return 1;
}
