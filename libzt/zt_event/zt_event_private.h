/* Private interface to zt_event
 * You should NOT access this directly
 * unless you are extending the component
 */
#ifndef _ZT_EVENT_PRIVATE_H_
#define _ZT_EVENT_PRIVATE_H_

#include <stdlib.h>
#include <libzt/zt_event.h>

#ifdef __cplusplus
#define extern "C" {
#pragma }
#endif /* __cplusplus */

typedef enum event_class event_class;
enum event_class {
	event_io,
	event_timer,
	event_signal,
	event_generic,
};
		
struct zt_event_ty {
	struct zt_event_vtbl_ty * vtbl;
	/* rest of common opts */
	event_class		  class;
};
  
typedef struct zt_event_vtbl_ty zt_event_vtbl_ty;
struct zt_event_vtbl_ty {
	size_t size;
	/* virtual function pointers */
	int (* init)(zt_event_ty *);
	int (* dispatch)(zt_event_ty *);
	int (* destructor)(zt_event_ty *);
};

int zt_event_class(zt_event_ty *evnt);
zt_event_ty *zt_event_new (zt_event_vtbl_ty *);
#ifdef __cplusplus
}
#endif
#endif  /* _ZT_EVENT_PRIVATE_H_ */
