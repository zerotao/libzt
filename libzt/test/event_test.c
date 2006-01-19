#include <libzt/zt.h>
#include <libzt/zt_event.h>
#include <libzt/zt_event/private.h>
#include <libzt/zt_log.h>
#include "test.h"

typedef struct stream *stream;
struct stream {
	char		  buff[255];
	int		  fd;
};

char f[] = { '\\', '|', '/', '-' };
int i = 0;

void io_cb(zt_event_sys sys, int fd, zt_event_enum type, void *data)
{
	stream	  st = (stream)data;
	int	  len = 0;

	memset(st->buff, '\0', 255);
	if((len = read(st->fd, st->buff, sizeof(st->buff) - 1)) == -1) {
		zt_event_remove_io(sys, fd, type);
		fprintf(stderr, "connection closed\n");
		return;
	}

	if(len) {
		printf("stream: %s\n", st->buff);
		memset(st->buff, '\0', 255);
		printf("\n%c", f[0]), fflush(stdout);
		i = 1;
	}
}


static int counter = 0;

void timeout_cb(zt_event_sys sys, struct timeval *tv, void *data) 
{
	static struct timeval	ntv = {0, 0};
	
	counter++;
	
	/* 
         * printf("Timeout: %d:%d  %d:%d\n", sys->tod.tv_sec, sys->tod.tv_usec,
	 *        tv->tv_sec, tv->tv_usec);
         */
	/* zt_event_register_timer(sys, &ntv, timeout_cb, NULL); */
}

int main(int argc, char *argv[]) 
{
	zt_event_sys	  sys;
	struct stream	  st;
	struct timeval	  ntv = {0, 0};
	int		  i;
	int		  n;
	long		  last = 0;

	st.fd = STDIN_FILENO;
	
	sys = zt_event_select();

	//zt_event_register_io(sys, 0, ZT_READ_EVENT, io_cb, NULL);

	for(i=0; i < 10000; i++) {
		/* each entry must be made unique in one of:
		 * time, callback or callback data, otherwise
		 * the element is considered a duplicate and ignored
		 */
		zt_event_register_timer(sys, &ntv, timeout_cb, (void *)i, ZT_EVENT_TIMER_ONCE);
		if(i%2 == 0) {
			ntv.tv_usec = (i * 10);
		}
	}
	
	i = 0;
	while(zt_event_run(sys, ZT_ALL_EVENTS|ZT_RUN_ONCE|ZT_NON_BLOCK) != -1) {
		if(counter == 10000) {
			TEST("10000 timers", counter == 10000);
			break;
		}

/* 		if(counter != last) { */
/* 			TEST_N("TIMERS firing", i, last < counter); */
/* 		} */
		last = counter;
	}

	zt_event_register_io(sys, st.fd, ZT_READ_EVENT, io_cb, &st);

/* 	printf("\n%c", f[0]), fflush(stdout); */
/* 	i = 1; */
/* 	while(zt_event_run(sys, ZT_ALL_EVENTS | ZT_RUN_ONCE | ZT_NON_BLOCK) != -1) { */
/* 		if(++n % 100000 == 0) { */
/* 			printf("%c", f[i++]), fflush(stdout); */
/* 			if(i >= sizeof(f)) { */
/* 				i = 0; */
/* 			} */
/* 		} */
/* 	} */

	return 0;
}