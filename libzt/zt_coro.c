#include <stdint.h>     /* intptr_t */
#include <ctype.h>      /* size_t */
#include <string.h>     /* memset */
#include <stddef.h>
#include <stdio.h>      /* print */
#include <stdlib.h>     /* exit */

#include "zt.h"
#include "zt_assert.h"
#include "zt_coro.h"

#define ZT_CORO_CHECK_STACK(co)                         				\
	if (co != &coro_main) {												\
		int left = _stack_left(co);										\
		if(left <= 0 || left > co->size) {								\
			printf("Stack Overflow for coroutine @ %p\n", co);          \
			exit(1);                            						\
		}																\
	}

char    * zt_coro_except_exit = "CoroExit";

/* FIXME: these need to be thread safe */
static zt_coro coro_main;
static zt_coro *coro_current = &coro_main;
static zt_coro *coro_helper;

static void _switch_context(zt_coro *old, zt_coro *new)
{
    swapcontext(&old->ctx, &new->ctx);
}

static uint8_t *_current_stack_pointer(void)
{
	uint8_t	  a;
	uint8_t	* b = &a;
	return b;
}

static ptrdiff_t _stack_left(zt_coro *co)
{
	unsigned char	  dummy;
	ptrdiff_t		  p1 = (ptrdiff_t)(&dummy);
	ptrdiff_t		  p2 = (ptrdiff_t)_current_stack_pointer();
	int				  stack_up = p2 > p1;
	ptrdiff_t		  start = ((ptrdiff_t)co->ctx.uc_stack.ss_sp);
	ptrdiff_t		  end = start + co->size;
	
	if (stack_up) {
		return end - p1;
	}
	/* else */
	return p1 - start;
}

static void _coro_run(void)
{
    zt_coro     * co = coro_current;
    
    co->target = co->caller;
    
    DO_TRY
    {
    co->except_stack = _except_Stack;
    co->func(co->data);
    }
    ELSE_TRY
    {
    CATCH(except_CatchAll,
          {
          if(_except_Stack->exception != &zt_coro_except_exit) {
              if(co->target->except_stack == NULL) {
              except_unhandled_exception(_except_Stack, 1);
              } else {
              /* copy the exception for propigation */
              _COPY_FRAME_DATA(_except_Stack, co->target->except_stack);
              co->target->except_stack->caught = -1;
              }
          }
          });
                
    }
    END_TRY;
        
    zt_coro_exit(co->data);
}

zt_coro *
zt_coro_create(void *(*func)(void *), zt_coro *co, size_t stack_size) {
	unsigned char	* stack;
	
    assert((stack_size &= ~(sizeof(int) - 1)) >= ZT_CORO_MIN_STACK_SIZE);

    if (!co) {
        co = (zt_coro *)XCALLOC(unsigned char, sizeof(*co) + stack_size);
    }
	
	stack = (unsigned char *)(co+1);

    if (coro_main.overflow == 0) {
        coro_main.overflow = 0xDEADBEEF;
    }
    
    /* Assign the coroutine to the base of the stack */
    co->overflow = 0xDEADBEEF;
    
    if(getcontext(&(co->ctx)) < 0) {
        printf("getcontext failed");
        abort();
    }
    
    co->ctx.uc_stack.ss_sp = stack;
    co->ctx.uc_stack.ss_size =  stack_size;
	
	co->size = stack_size;
	
#if !defined(__APPLE__)
    co->ctx.uc_stack.ss_flags = 0;
    co->ctx.uc_link = NULL;
#endif

    co->func = func;
    makecontext(&(co->ctx), _coro_run, 0);
    return co;
}

void zt_coro_delete(zt_coro *co) 
{
    if(co == coro_current) {
        printf("coroutine cannot delete itself\n");
        exit(1);
    }

    free(co);
}


void *_zt_coro_call(zt_coro *co, void *data, int yield)
{
    zt_coro *old = coro_current;
        
	ZT_CORO_CHECK_STACK(coro_current);
        
    co->caller = coro_current;
    coro_current = co;
    
    co->data = data;

	old->except_stack = _except_Stack;
	_except_Stack = co->except_stack;

	_switch_context(old, co);
        
	_except_Stack = old->except_stack;
        
	if(_except_Stack &&
	   _except_Stack->caught == -1) {
		/* RETHROW the exception */
		_except_Stack->caught = 0;
		longjmp(_except_Stack->env, 1);
	}
        
    return old->data;
}

void *zt_coro_call(zt_coro *co, void *data)
{
    return _zt_coro_call(co, data, 0);
}

void *zt_coro_yield(void *data)
{
    data = _zt_coro_call(coro_current->target, data, 1);
    coro_current->target = coro_current->caller;
    return data;    
}

static void *_del_helper(void *data) 
{
    zt_coro *co;

    for(;;){

        co = coro_helper;
        coro_helper = NULL;
        zt_coro_delete(coro_current->caller);
        data = zt_coro_call(co, data);
        if(!coro_helper) {
            printf("yielded to delete helper coroutine\n");
            exit(1);
        }
    }
    return data;
}

        

void zt_coro_exit_to(zt_coro *co, void *data)
{
    static zt_coro	* helper = NULL;
    static char 	  vco[sizeof(zt_coro) + ZT_CORO_MIN_STACK_SIZE];
	
    if(!helper &&
       !(helper = zt_coro_create(_del_helper, (zt_coro *)&vco, ZT_CORO_MIN_STACK_SIZE))) {
        printf("Unable to create delete helper coroutine\n");
        exit(1);
    }
        
    coro_helper = co;
    zt_coro_call(helper, data);
    printf("stale corotine called\n");
    exit(1);
}
    

void zt_coro_exit(void *data) 
{
    zt_coro_exit_to(coro_current->target, data);
}

zt_coro *zt_coro_get_current(void)
{
    return coro_current;
}
