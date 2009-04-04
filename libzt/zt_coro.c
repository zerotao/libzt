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
			printf("Stack Overflow (by %d bytes of %ld) for coroutine @ %p\n", left, co->size, (void *)co); \
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
	register intptr_t start = (int)&co->ctx + (int)sizeof(co->ctx); /* ((intptr_t)co->ctx.uc_stack.ss_sp);	 */
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

#else  /* SETJUMP */

static void _switch_context(zt_coro *old, zt_coro *new) {
	if (!setjmp(old->ctx)) {
		printf("%p %p\n", (void *)old->ctx, (void *)new->ctx);
		longjmp(new->ctx, 1);
	}
}

# if defined(USE_SIGCONTEXT)

/* 
 * static void _bootstrap_sigcontext(void) 
 * {
 * 	zt_coro		* volatile co_starting;
 * 	void		(* volatile co_starting_fn)(void);
 * 
 * }
 * 
 * static void _sigcontext_trampoline(void)
 * {
 * }
 * 
 * static int _set_context(zt_coro_ctx *ctl, zt_coro *co, void *fn, unsigned char *stack, long size)
 * {
 * }
 */

# else	/* USE_SIGCONTEXT */

static int _set_context(zt_coro_ctx *ctl, zt_coro *co, void *fn, unsigned char *stack, long size)
{
  /* Solaris 9 Sparc with GCC */
  #if defined(__SVR4) && defined (__sun)
    #if defined(_JBLEN) && (_JBLEN == 12) && defined(__sparc)
      #if defined(_LP64) || defined(_I32LPx)
        #define JBTYPE long
        JBTYPE x;
      #else
        #define JBTYPE int
        JBTYPE x;
        asm("ta 3"); /* flush register window */
      #endif
      #define SUN_PROGRAM_COUNTER 2
      #define SUN_STACK_START_INDEX 3 
      #define SUN_STACK_END_INDEX 1
    /* Solaris 9 i386 with GCC */
    #elif defined(_JBLEN) && (_JBLEN == 10) && defined(__i386)
      #if defined(_LP64) || defined(_I32LPx)
        #define JBTYPE long
        JBTYPE x;
      #else
        #define JBTYPE int
        JBTYPE x;
      #endif
      #define SUN_PROGRAM_COUNTER 5
      #define SUN_STACK_START_INDEX 3 
      #define SUN_STACK_END_INDEX 4
    #endif
  #endif
	
  setjmp(co->ctx);
  
  /* #if defined(__MACOSX__) && defined(_BSD_PPC_SETJMP_H_) */
  #if defined(_BSD_PPC_SETJMP_H_) 
    /* OSX/PPC */
    #define setjmp _setjmp /* no need for signal masks */
    #define longjmp _longjmp
    memset(((char *)stack) + size - 64, 0, 64);	/* very crude... */
    co->ctx[0] = ((intptr_t)stack + size - 64 + 15) & ~15;
    co->ctx[21] = (intptr_t)fn;

  #elif defined(sgi) && defined(_IRIX4_SIGJBLEN)
    /* Irix/SGI */
    co->ctx[JB_SP] = (__uint64_t)((char *)stack + size - 8);
    co->ctx[JB_PC] = (__uint64_t)fn;

  #elif defined(linux) && defined(JB_GPR1)
      /* Linux/PPC */
	co->ctx->__jmpbuf[JB_GPR1] = ((int) stack + size - 64 + 15) & ~15;
	co->ctx->__jmpbuf[JB_LR] = (int) fn;
    
  #elif defined(linux) && defined(JB_RBX)
      /* Linux/Opteron */
	co->ctx->__jmpbuf[JB_RSP] = (long int )stack + size;
	co->ctx->__jmpbuf[JB_PC] = fn;

  #elif defined(linux) && defined(JB_SP)
      /* Linux/x86 with glibc2 */
	co->ctx->__jmpbuf[JB_SP] = (int)stack + size;
	co->ctx->__jmpbuf[JB_PC] = (int)fn;
    
  #elif defined(linux) && defined(_I386_JMP_BUF_H)
	/* x86-linux with libc5 */
	co->ctx->__sp = stackend;
	co->ctx->__pc = fn;
    
  #elif defined(linux) && defined(__JMP_BUF_SP)
      /* arm-linux on the sharp zauras */
	co->ctx->__jmpbuf[__JMP_BUF_SP] = (int)stack + size;
	co->ctx->__jmpbuf[__JMP_BUF_SP+1] = (int)fn;

  #elif defined(__CYGWIN__) 
    /* Cygwin */
    co->ctx[7] = (int)((int)stack + size);
    co->ctx[8] = (int)fn;
    
  #elif defined(__MINGW32__) 
    co->ctx[4] = (int)((unsigned char *)stack + size - 100);   /* esp */
    co->ctx[5] = (int)fn; /* eip */
      
  #elif defined(_MSC_VER)
    /* win32 visual c */
    /* should this be the same as __MINGW32__? */
    co->ctx[4] = (int)((unsigned char *)stack + size);  /* esp */
    co->ctx[5] = (int)fn; /* eip */
    
  #elif defined(__SYMBIAN32__)
    /* Symbinan. */
    co->ctx[0] = 0;
    co->ctx[1] = 0;
    co->ctx[2] = 0;
    co->ctx[3] = (unsigned int)stack + CSTACK_SIZE - 64;
    co->ctx[9] = (int)fn;
    co->ctx[8] =  buf[3] + 32;

  #elif defined(__FreeBSD__) && defined(_JBLEN) && (_JBLEN == 81)
    /* FreeBSD/Alpha */
    co->ctx->_jb[2] = (long)fn;     /* sc_pc */
    co->ctx->_jb[26+4] = (long)fn;  /* sc_regs[R_RA] */
    co->ctx->_jb[27+4] = (long)fn;  /* sc_regs[R_T12] */
    co->ctx->_jb[30+4] = (long)(stack + size); /* sc_regs[R_SP] */

  #elif  defined(__FreeBSD__) && defined(_JBLEN)
    /* FreeBSD on IA32 */
    co->ctx->_jb[2] = (long)(stack + size);
    co->ctx->_jb[0] = (long)fn;

  #elif defined(__NetBSD__) && defined(_JB_ATTRIBUTES)
    /* NetBSD i386. */
    co->ctx[2] = (long)(stack + size);
    co->ctx[0] = (long)fn;

  #elif defined(__SVR4) && defined(__sun) && defined(SUN_PROGRAM_COUNTER)
    /* SunOS 9 */
    co->ctx[SUN_PROGRAM_COUNTER] = (JBTYPE)fn;
    
    x = (JBTYPE)stack;
    while ((x % 8) != 0) x--; /* align on an even boundary */
    co->ctx[SUN_STACK_START_INDEX] = (JBTYPE)x;
    x = (JBTYPE)((JBTYPE)stack-size/2+15);
    while ((x % 8) != 0) x++; /* align on an even boundary */
    co->ctx[SUN_STACK_END_INDEX] = (JBTYPE)x;
	
  #elif defined(__APPLE__) && defined(i386)
	co->ctx[7] = (intptr_t)stack + size; /* esp */
	co->ctx[10] = (intptr_t)fn;			  /* eip */
  #else
    #error "no setup_jumpbuf entry for this platform"
  #endif

  return 0;	
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
