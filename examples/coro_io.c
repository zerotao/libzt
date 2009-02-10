#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <libzt/zt_coro.h>
#include <libzt/zt_assert.h>
#include <libzt/zt_unit.h>

struct io_request {
    int   	  fd;
    int 	  bsize;
    int 	  rlen;
    int 	  result;
    char    * buffer;
};

void *_call_write(zt_coro_ctx *ctx, void *data)
{
	struct io_request 	* req = (struct io_request *)data;
	int					  res = 0;
	char				* bp;
	int					  remain;
	
	bp = req->buffer;
	remain = req->bsize;

	errno = 0;
	while((res = write(req->fd, bp, remain)) != remain)
	{
		if(res == -1) {
			switch(errno) {
				case EAGAIN:
				case EINTR:
					break;
			}
		} else {
			req->rlen = res;
			req->result = 0;
			zt_coro_yield(ctx, data);
		}
	}
	
	req->rlen = req->bsize;
	req->result = -1;
	zt_coro_exit(ctx, data);
	return NULL;				/* never reached */
}


void *_call_read(zt_coro_ctx *ctx, void *data) 
{
    struct io_request   * req = (struct io_request *)data;
    int         		  res = 0;
	
	errno = 0;
    while((res = read(req->fd, req->buffer, req->bsize)) != 0) {
        if(res == -1) {
            switch(errno){
                case EAGAIN:
                case EINTR:
                    req->rlen = 0;
                    req->result = 0;
                    zt_coro_yield(ctx, data);
					errno = 0;
                    break;
                default:
                    req->rlen = 0;
                    req->result = errno;
                    zt_coro_exit(ctx, data);
                    break;
            }
        } else {
            req->rlen = res;
            req->result = 0;
            zt_coro_yield(ctx, data);
        }
    }
    req->rlen = res;
    req->result = -1;
    zt_coro_exit(ctx, (void *)data);
	return NULL;				/* never reached */
}



int main(int argc, char * argv[]) 
{
	struct io_request     r1;
	struct io_request     r2;
	char              	  b1[1024];
	char              	  b2[1024];
	
    zt_coro         	* read_coro;
	zt_coro         	* write_coro;
	zt_coro_ctx			  ctx;
	
	zt_coro_init_ctx(&ctx);
	
	read_coro = zt_coro_create(&ctx, _call_read, NULL, ZT_CORO_MIN_STACK_SIZE);
	
	write_coro = zt_coro_create(&ctx, _call_write, NULL, ZT_CORO_MIN_STACK_SIZE);

	
	r1.fd = 0;
	r1.buffer = b1;
	r1.bsize = sizeof(b1);
	memset(b1, 0, sizeof(b1));
	fcntl(0, F_SETFL, O_NONBLOCK);
    
	r2.fd = 0;
	r1.buffer = b2;
	r2.bsize = sizeof(b2);
    
	while(1) {
		struct io_request   * res;
		res = zt_coro_call(&ctx, read_coro, &r1);
		
		if(res->result == 0 && res->rlen > 0) {
			printf("result: %s", res->buffer);
		} else if (res->result == -1) {
			break;
		}
	}
}
