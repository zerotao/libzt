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

struct test_data {
	struct zt_unit_test	* test;
	int				  	  i;
};


void *_call2(zt_coro_ctx *ctx, void *data) 
{
	struct test_data	   	* td = (struct test_data *) data;
    struct except_Frame     * stack = _except_Stack;
	
	/* 
     * unsigned char		    dummy[ZT_CORO_MIN_STACK_SIZE + 10];
	 * memset(dummy, 0, sizeof(dummy));
     */
	
    ZT_UNIT_ASSERT(td->test, td->i == 2);
	td->i = 3;
    td = (struct test_data *)zt_coro_yield(ctx, (void *)td);
    
    ZT_UNIT_ASSERT(td->test, td->i == 6);

    td->i = 7;
    ZT_UNIT_ASSERT(td->test, stack == _except_Stack);
        
    THROW(td);
    zt_coro_exit(ctx, (void *)td);
}


void *_call1(zt_coro_ctx *ctx, void *data) 
{
	zt_coro_stack_left(ctx);
	struct test_data	  	* td = (struct test_data *) data;
    zt_coro                 * co = zt_coro_create(ctx, _call2, 0, ZT_CORO_MIN_STACK_SIZE);
    struct except_Frame     * stack = _except_Stack;
	// uncomment to force a stack overflow
	//int ** 				  p = (int **)alloca(1024 * sizeof(int));
	
	zt_coro_check_stack(ctx);
	zt_coro_stack_left(ctx);
	
    ZT_UNIT_ASSERT(td->test, td->i == 1);
	
    td->i = 2;
    td = (struct test_data *)zt_coro_call(ctx, co, (void *)td);
    ZT_UNIT_ASSERT(td->test, td->i == 3);

	td->i = 4;
    td = (struct test_data *)zt_coro_yield(ctx, (void *)td);
    ZT_UNIT_ASSERT(td->test, td->i == 5);

    DO_TRY
    {
        td = (struct test_data *)zt_coro_call(ctx, co, (void *)td);
    }
    ELSE_TRY
    {
        CATCH(except_CatchAll, td->i = 7;);
        /* RETHROW(); */
    }
    END_TRY;
        
             
    ZT_UNIT_ASSERT(td->test, td->i == 7);
    ZT_UNIT_ASSERT(td->test, stack == _except_Stack);
    zt_coro_exit(ctx, (void *)td);
}

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    zt_coro         	* co;
	zt_coro_ctx			  ctx;
	
	struct except_Frame * stack = _except_Stack;
	struct test_data	  td;
	struct test_data	* tdp;
	
    /* Stacks using the "Default" minimum stack size should not
     * make any use of zt_except (it expects alot more stack to be available
     */
	zt_coro_init_ctx(&ctx);
	co = zt_coro_create(&ctx, _call1, 0, ZT_CORO_MIN_STACK_SIZE);
		
    if(co == NULL) {
        exit(1);
    }
    td.test = test;
	td.i = 1;
	
    tdp = (struct test_data *)zt_coro_call(&ctx, co, (void *)&td);
    ZT_UNIT_ASSERT(test, tdp->i == 4);
        
    DO_TRY
    {
		td.i = 5;
        tdp = (struct test_data *)zt_coro_call(&ctx, co, (void *)&td);
        ZT_UNIT_ASSERT(test, tdp->i == 8);
    }
    ELSE_TRY
    {
		CATCH(except_CatchAll,{});
	}
	END_TRY;
	
    
    /* the coroutine exited it's self
       no need to delete it */
    /* zt_coro_delete(co); */
    ZT_UNIT_ASSERT(test, stack == _except_Stack);
}

int
register_coro_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "coro tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}


