#include <stdint.h>     /* intptr_t */
#include <ctype.h>      /* size_t */
#include <string.h>     /* memset */
#include <stddef.h>
#include <stdio.h>      /* print */
#include <stdlib.h>     /* exit */

#include "zt.h"
#include "zt_assert.h"
#include "zt_coro.h"

#define ZT_CORO_CHECK_STACK(ctl, co)									\
	if (co != &ctl->main) {												\
		ptrdiff_t left = _stack_left(co);								\
		if((left <= 0) || (left > (co->size))) {						\
			printf("Stack Overflow (by %d bytes of %d) for coroutine @ %p\n", left, co->size, co); \
			exit(1);                            						\
		}																\
	}

char    * zt_coro_except_exit = "CoroExit";

int zt_coro_init_ctx(zt_coro_ctx *ctx)
{
	ctx->current = &ctx->main;
	return 0;
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
	intptr_t		  p1 = (intptr_t)(&dummy);
	intptr_t		  p2 = (intptr_t)_current_stack_pointer();
	register int	  stack_up = p2 > p1;
	register intptr_t start = ((intptr_t)co->ctx.uc_stack.ss_sp);
	register intptr_t end = start + co->size;
	
	if (stack_up) {
		return (ptrdiff_t)(end - p1);
	}
	/* else */
	return (ptrdiff_t)(p1 - start);
}

int zt_coro_stack_left(zt_coro_ctx *ctl) 
{
	ptrdiff_t left = _stack_left(ctl->current);
	return left;
}

void zt_coro_check_stack(zt_coro_ctx *ctl) 
{
	ZT_CORO_CHECK_STACK(ctl, ctl->current);
}

static void _coro_run(zt_coro_ctx *ctl)
{
    zt_coro     * co = ctl->current;
    
    co->target = co->caller;
    
    DO_TRY
    {
		co->except_stack = _except_Stack;
		co->func(ctl, co->data);
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
        
    zt_coro_exit(ctl, co->data);
}

typedef void (*mkcontext_fn)(void);

#define USE_UCONTEXT 1
#if defined(USE_UCONTEXT)
static int _set_context(zt_coro_ctx *ctl, zt_coro *co, void *fn, unsigned char *stk, long size) 
{
	if (getcontext(&(co->ctx)) < 0) {
		return -1;
	}

	co->ctx.uc_stack.ss_sp = stk;
	co->ctx.uc_stack.ss_size = size;
	co->ctx.uc_stack.ss_flags = 0;
	co->size = size;
	co->func = fn;
	
	makecontext(&(co->ctx), (mkcontext_fn)_coro_run, 1, ctl);

	return 0;
}

static void _switch_context(zt_coro *old, zt_coro *new)
{
    if (swapcontext(&old->ctx, &new->ctx) < 0) {
		printf("Error switch failed\n");
		exit(1);
	}
}

#else  /* USE_UCONTEXT */

static void _switch_context(zt_coro *old, zt_coro *new) {

	if (!setjmp(old->ctx))
		longjmp(new->ctx, 1);
}

# if defined(USE_SIGCONTEXT)
static void _bootstrap_sigcontext(void) 
{
	zt_coro		* volatile co_starting;
	void		(* volatile co_starting_fn)(void);

}

static void _sigcontext_trampoline(void)
{
}

static int _set_context(zt_coro_ctx *ctl, zt_coro *co, void *fn, unsigned char *stk, long size)
{
}

# else	/* USE_SIGCONTEXT */

static int _set_context(zt_coro_ctx *ctl, zt_coro *co, void *fn, unsigned char *stk, long size)
{
	
}

# endif	 /* USE_SIGCONTEXT */

#endif	 /* USE_UCONTEXT */



zt_coro *
zt_coro_create(zt_coro_ctx *ctl, void *(*func)(zt_coro_ctx *, void *), zt_coro *co, size_t stack_size) {
	unsigned char	* stack;
	
    assert((stack_size &= ~(sizeof(int) - 1)) >= ZT_CORO_MIN_STACK_SIZE);

    if (!co) {
        co = (zt_coro *)XCALLOC(unsigned char, sizeof(zt_coro) + stack_size);
    }
	
	stack = (unsigned char *)(co+1);

	if (_set_context(ctl, co, func, stack, stack_size) == -1) {
		printf("create context failed");
		abort();
	}
				 
/* 
 *     /\* Assign the coroutine to the base of the stack *\/  
 *     if(getcontext(&(co->ctx)) < 0) {
 *         printf("getcontext failed");
 *         abort();
 *     }
 *     
 *     co->ctx.uc_stack.ss_sp = stack;
 *     co->ctx.uc_stack.ss_size =  stack_size;
 * 	
 * 	co->size = stack_size;
 * 	
 * #if !defined(__APPLE__) || defined(__DARWIN_UNIX03)
 *     co->ctx.uc_stack.ss_flags = 0;
 *     co->ctx.uc_link = NULL;
 * #endif
 * 
 *     co->func = func;
 *     makecontext(&(co->ctx), (mkcontext_fn)_coro_run, 1, ctl);
 */
    return co;
}

void zt_coro_delete(zt_coro_ctx *ctl, zt_coro *co) 
{
    if(co == ctl->current) {
        printf("coroutine cannot delete itself\n");
        exit(1);
    }

    free(co);
}


void *_zt_coro_call(zt_coro_ctx *ctl, zt_coro *co, void *data, int yield)
{
    zt_coro *old = ctl->current;
        
	ZT_CORO_CHECK_STACK(ctl, ctl->current);
        
    co->caller = ctl->current;
    ctl->current = co;
    
    co->data = data;

	old->except_stack = _except_Stack;
	_except_Stack = co->except_stack;

	_switch_context(old, co);
	//swapcontext(&(old->ctx), &(co->ctx));
	
	_except_Stack = old->except_stack;
        
	if(_except_Stack &&
	   _except_Stack->caught == -1) {
		/* RETHROW the exception */
		_except_Stack->caught = 0;
		longjmp(_except_Stack->env, 1);
	}
        
    return old->data;
}

void *zt_coro_call(zt_coro_ctx *ctl, zt_coro *co, void *data)
{
    return _zt_coro_call(ctl, co, data, 0);
}

void *zt_coro_yield(zt_coro_ctx *ctl, void *data)
{
    data = _zt_coro_call(ctl, ctl->current->target, data, 1);
    ctl->current->target = ctl->current->caller;
    return data;    
}

static void *_del_helper(zt_coro_ctx *ctl, void *data) 
{
    zt_coro *co;

    for(;;){

        co = ctl->helper;
        ctl->helper = NULL;
        zt_coro_delete(ctl, ctl->current->caller);
        data = zt_coro_call(ctl, co, data);
        if(!ctl->helper) {
            printf("yielded to delete helper coroutine\n");
            exit(1);
        }
    }
    return data;
}

        

void zt_coro_exit_to(zt_coro_ctx *ctl, zt_coro *co, void *data)
{
    static zt_coro	* helper = NULL;
    static char 	  vco[sizeof(zt_coro) + ZT_CORO_MIN_STACK_SIZE];
	
    if(!helper &&
       !(helper = zt_coro_create(ctl, _del_helper, (zt_coro *)&vco, ZT_CORO_MIN_STACK_SIZE))) {
        printf("Unable to create delete helper coroutine\n");
        exit(1);
    }
        
    ctl->helper = co;
    zt_coro_call(ctl, helper, data);
    printf("stale corotine called\n");
    exit(1);
}
    

void zt_coro_exit(zt_coro_ctx *ctl, void *data) 
{
    zt_coro_exit_to(ctl, ctl->current->target, data);
}

zt_coro *zt_coro_get_current(zt_coro_ctx *ctl)
{
    return ctl->current;
}
