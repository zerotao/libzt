#include "zt_event_private.h"

/* local declarations */
static int init(zt_event_ty * evnt);
static int dispatch(zt_event_ty * evnt);
static int destructor(zt_event_ty * evnt);

/* component data */
static zt_event_vtbl_ty vtbl = {
	sizeof(zt_event_ty),
	init,
	dispatch,
	destructor,
};

zt_event_ty *
zt_event_unix(void)
{
	return zt_event_new(&vtbl);
}

/* local definitions */
static int
init(zt_event_ty * evnt)
{
	return 1;
}

static int
dispatch(zt_event_ty * evnt)
{
	return 1;
}

static int
destructor(zt_event_ty * zt_event)
{
	return 1;
}
