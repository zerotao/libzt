#include <libzt/zt_event.h>

extern zt_event_sys zt_event_select(void);
extern zt_event_sys zt_event_kqueue(void);
extern zt_event_sys zt_event_poll(void);
extern zt_event_sys zt_event_epoll(void);
extern zt_event_sys zt_event_devpoll(void);
extern zt_event_sys zt_event_best(void);
