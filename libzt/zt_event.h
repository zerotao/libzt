/*
 * public interface to the zt_event event processing system
 *
 */
#ifndef _ZT_EVENT_H_
#define _ZT_EVENT_H_

#include <libzt/zt.h>
#include <libzt/zt_time.h>

BEGIN_C_DECLS

typedef enum zt_event_flags {
	ZT_RUN_ONCE       = 1,
	ZT_NON_BLOCK      = 2,
	ZT_READ_EVENTS	  = 4,
	ZT_WRITE_EVENTS   = 8,
	ZT_EXCEPT_EVENTS  = 16,
	ZT_TIMER_EVENTS   = 32,
	ZT_SIGNAL_EVENTS  = 64,
}zt_event_flags;
#define ZT_ALL_EVENTS (ZT_READ_EVENTS | ZT_WRITE_EVENTS | ZT_EXCEPT_EVENTS | ZT_TIMER_EVENTS | ZT_SIGNAL_EVENTS)

typedef enum zt_event_enum {
	ZT_READ_EVENT     = 1,
	ZT_WRITE_EVENT    = 2,
	ZT_EXCEPT_EVENT   = 4
}zt_event_enum;

typedef struct zt_event_sys *zt_event_sys;

typedef void zt_event_io_cb(zt_event_sys sys, int fd, zt_event_enum type, void *data);
typedef void zt_event_signal_cb(zt_event_sys sys, int signal, void *data);
typedef void zt_event_timer_cb(zt_event_sys sys, struct timeval *time, void *data);

int zt_event_register_io(zt_event_sys sys, int fd, zt_event_enum type, zt_event_io_cb cb, void *data);
int zt_event_remove_io(zt_event_sys sys, int fd, zt_event_enum type);

int zt_event_register_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data);
int zt_event_remove_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data);

int zt_event_register_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data);
int zt_event_remove_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data);

int zt_event_destroy(zt_event_sys sys);
int zt_event_run(zt_event_sys sys, zt_event_flags flags);

#include <libzt/zt_event/interface.h>
END_C_DECLS
#endif /* _ZT_EVENT_H_ */
