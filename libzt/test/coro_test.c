#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <libzt/zt_coro.h>
#include <libzt/zt_assert.h>

#include "test.h"

struct io_request {
	int	  fd;
	int	  bsize;
	int	  rlen;
	int	  result;
	char	* buffer;
};

void *_call_read(void *data) 
{
	struct io_request	* req = (struct io_request *)data;
	int			  res = 0;
	
	while((res = read(req->fd, req->buffer, req->bsize)) != 0) {
		if(res == -1) {
			switch(errno){
				case EAGAIN:
				case EINTR:
					req->rlen = 0;
					req->result = 0;
					zt_coro_yield(data);
					break;
				default:
					req->rlen = 0;
					req->result = errno;
					zt_coro_exit(data);
					break;
			}
		} else {
			req->rlen = res;
			req->result = 0;
			zt_coro_yield(data);
		}
	}
	req->rlen = res;
	req->result = -1;
	return data;
}

void *_call2(void *data) 
{
	int	  i = (int) data;

	TEST("zt_coro_call[2]", i == 2);
	i = (int)zt_coro_yield((void *)3);
	
	TEST("zt_coro_call[6]", i == 6);

	zt_coro_exit((void *)7);
}

void *_call1(void *data) 
{
	int	  i = (int) data;
	zt_coro	* co = zt_coro_create(_call2, 0, ZT_CORO_MIN_STACK_SIZE);
	
	TEST("zt_coro_call[1]", i == 1);
	
	i = (int)zt_coro_call(co, (void *)2);
	TEST("zt_coro_call[3]", i == 3);
	
	i = (int)zt_coro_yield((void *)4);
	TEST("zt_coro_call[5]", i == 5);

	i = (int)zt_coro_call(co, (void *)6);
	TEST("zt_coro_call[7]", i == 7);

	zt_coro_exit((void *)8);
}

int
main(int argc, char *argv[])
{
	zt_coro			* co;
	zt_coro			* read_coro;
	int	  		  i;
	struct io_request	  r1;
	struct io_request	  r2;
	char			  b1[1024];
	char			  b2[1024];
		
	
	co = zt_coro_create(_call1, 0, ZT_CORO_MIN_STACK_SIZE);

	if(co == NULL) {
	  exit(1);
	}
	i = (int)zt_coro_call(co, (void *)1);
	TEST("zt_coro_call[4]", i == 4);
	
	i = (int)zt_coro_call(co, (void *)5);
	TEST("zt_coro_call[8]", i == 8);
	
	/* the coroutine exited it's self
	   no need to delete it */
	/* zt_coro_delete(co); */


	read_coro = zt_coro_create(_call_read, 0, ZT_CORO_MIN_STACK_SIZE);

	r1.fd = 0;
	r1.buffer = b1;
	r1.bsize = sizeof(b1);
	memset(b1, 0, sizeof(b1));
	fcntl(0, F_SETFL, O_NONBLOCK);
	
	/* 
         * r2.fd = XX;
	 * r1.buffer = b2;
	 * r2.bsize = sizeof(b2);
         */
	
/* 	while(1) { */
/* 		struct io_request	* res; */
/* 		res = zt_coro_call(read_coro, &r1); */
/* 		if(res->result == 0 && res->rlen > 0){ */
/* 			printf("result: %s", res->buffer); */
/* 		} */
/* 	} */
	return 0;
}


