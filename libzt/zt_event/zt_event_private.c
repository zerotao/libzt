#include <stdlib.h>
#include "zt_event_private.h"

int
zt_event_class(zt_event_ty *evnt)
{
	if (evnt) {
		return evnt->class;
	}
	return -1;
}

zt_event_ty*
zt_event_new(zt_event_vtbl_ty *vptr)
{
	zt_event_ty *result;
	result = calloc(1, vptr->size);
	result->vtbl = vptr;
	return(result);
}
