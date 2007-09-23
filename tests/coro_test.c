#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <libzt/zt_coro.h>
#include <libzt/zt_assert.h>

#include "test.h"

struct io_request {
    int   fd;
    int   bsize;
    int   rlen;
    int   result;
    char    * buffer;
};

void *_call_read(zt_coro_ctx *ctx, void *data) 
{
    struct io_request   * req = (struct io_request *)data;
    int           res = 0;
    
    while((res = read(req->fd, req->buffer, req->bsize)) != 0) {
        if(res == -1) {
            switch(errno){
                case EAGAIN:
                case EINTR:
                    req->rlen = 0;
                    req->result = 0;
                    zt_coro_yield(ctx, data);
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
    return data;
}

void *_call2(zt_coro_ctx *ctx, void *data) 
{
    int                   i = (int) data;
    struct except_Frame     * stack = _except_Stack;
	
	/* 
     * unsigned char		    dummy[ZT_CORO_MIN_STACK_SIZE + 10];
	 * memset(dummy, 0, sizeof(dummy));
     */
	
    TEST("zt_coro_call[2]", i == 2);
    i = (int)zt_coro_yield(ctx, (void *)3);
    
    TEST("zt_coro_call[6]", i == 6);

    i = 7;
    TEST("_call2 local except stack[1]", stack == _except_Stack);
        
    THROW(i);
    zt_coro_exit(ctx, (void *)7);
}


void *_call1(zt_coro_ctx *ctx, void *data) 
{
	zt_coro_stack_left(ctx);
    int                   i = (int) data;
    zt_coro                 * co = zt_coro_create(ctx, _call2, 0, ZT_CORO_MIN_STACK_SIZE);
    struct except_Frame     * stack = _except_Stack;
	// uncomment to force a stack overflow
	//int ** 				  p = (int **)alloca(1024 * sizeof(int));
	
	zt_coro_check_stack(ctx);
	zt_coro_stack_left(ctx);
	
	printf("frame: %d\n", sizeof(struct except_Frame));
	
    TEST("zt_coro_call[1]", i == 1);
    
    i = (int)zt_coro_call(ctx, co, (void *)2);
    TEST("zt_coro_call[3]", i == 3);
	
	
    i = (int)zt_coro_yield(ctx, (void *)4);
    TEST("zt_coro_call[5]", i == 5);

    DO_TRY
    {
        i = (int)zt_coro_call(ctx, co, (void *)6);
    }
    ELSE_TRY
    {
        CATCH(except_CatchAll, i = 7;);
        /* RETHROW(); */
    }
    END_TRY;
        
             
    TEST("zt_coro_call[7]", i == 7);
    TEST("_call1 local except stack[1]", stack == _except_Stack);
    zt_coro_exit(ctx, (void *)8);
}

int
main(int argc, char *argv[])
{
    zt_coro         	* co;
    zt_coro         	* read_coro;
	zt_coro_ctx			  ctx;
	
    int           	  i;
    struct io_request     r1;
    struct io_request     r2;
    char              	  b1[1024];
    char              	  b2[1024];
    struct except_Frame * stack = _except_Stack;
        
    /* Stacks using the "Default" minimum stack size should not
     * make any use of zt_except (it expects alot more stack to be available
     */
	zt_coro_init_ctx(&ctx);
    co = zt_coro_create(&ctx, _call1, 0, ZT_CORO_MIN_STACK_SIZE + 2048);
		
    if(co == NULL) {
        exit(1);
    }
        
    i = (int)zt_coro_call(&ctx, co, (void *)1);
    TEST("zt_coro_call[4]", i == 4);
        
    DO_TRY
    {
        i = (int)zt_coro_call(&ctx, co, (void *)5);
        TEST("zt_coro_call[8]", i == 8);
    }
    ELSE_TRY
    {
        CATCH(except_CatchAll,{});
    }
    END_TRY;
	
    
    /* the coroutine exited it's self
       no need to delete it */
    /* zt_coro_delete(co); */
    TEST("main local except stack[1]", stack == _except_Stack);
    
    read_coro = zt_coro_create(&ctx, _call_read, 0, ZT_CORO_MIN_STACK_SIZE);

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
    
    /*  while(1) { */
    /*      struct io_request   * res; */
    /*      res = zt_coro_call(read_coro, &r1); */
    /*      if(res->result == 0 && res->rlen > 0){ */
    /*          printf("result: %s", res->buffer); */
    /*      } */
    /*  } */
    return 0;
}


