#include <string.h>

#include <libzt/zt.h>
#include <libzt/zt_event.h>
#include <libzt/zt_event/private.h>
#include <libzt/zt_log.h>
#include <libzt/zt_unit.h>

static int counter = 0;

#define NUM_TIMERS 10000

void timeout_cb(zt_event_sys sys, struct timeval *tv, void *data) 
{
	/* static struct timeval	ntv = {0, 0}; */
	counter++;
}

static void
basic_tests(struct zt_unit_test *test, void *data)
{
	zt_event_sys	  	  sys;
	struct timeval	  	  ntv = {0, 0};
	int					  i;
	long		  	  	  last = 0;
	
	sys = zt_event_select();

	for(i=0; i < NUM_TIMERS; i++) {
		/* each entry must be made unique in one of:
		 * time, callback or callback data, otherwise
		 * the element is considered a duplicate and ignored
		 */
		zt_event_register_timer(sys, &ntv, timeout_cb, (void *)i, ZT_EVENT_TIMER_ONCE);
		if(i%2 == 0) {
			ntv.tv_usec = (i * 10);
		}
	}

	{
		while(zt_event_run(sys, ZT_ALL_EVENTS|ZT_RUN_ONCE|ZT_NON_BLOCK) != -1) {
			if(counter == NUM_TIMERS) {
				ZT_UNIT_ASSERT(test, counter == NUM_TIMERS);
				break;
			}
			last = counter;
		}
	}	
}


int
register_event_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "event tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
