
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../zt.h"
#include "../zt_event.h"
#include "private.h"


zt_event_sys
zt_event_new(zt_event_vtbl *vptr) 
{
	zt_event_sys	result;

	result = XCALLOCS(vptr->size, 1);

	result->vtbl = vptr;
	return(result);
}
