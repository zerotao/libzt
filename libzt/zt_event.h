#ifndef _ZT_EVENT_H_
#define _ZT_EVENT_H_

#ifdef __cplusplus
#define extern "C" {
#pragma }
#endif /* __cplusplus */

typedef struct zt_event_ty zt_event_ty;

/* interface to the event manager */
/*
 * zt_event_add(zt_event_mngr_ty *em, zt_event_ty *e);
 * zt_event_del(zt_event_mngr_ty *em, zt_event_ty *e);
 * zt_event_run_loop(zt_event_mngr_ty *em)
 */


/* interface to the underling events */
int zt_event_init(zt_event_ty *evnt);
int zt_event_dispatch(zt_event_ty *evnt);
int zt_event_destroy(zt_event_ty *evnt);

#ifdef __cplusplus
}
#endif

#include <libzt/zt_event/zt_event_interface.h>

#endif  /* _ZT_EVENT_H_ */
