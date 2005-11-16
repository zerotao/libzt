#include <stdint.h>		/* intptr_t */
#include <ctype.h> 		/* size_t */
#include <string.h>		/* memset */

#include <stdio.h>		/* print */
#include <stdlib.h>		/* exit */

#include "zt.h"
#include "zt_assert.h"
#include "zt_coro.h"

#define CORO_STACK_ALIGN 256
#define CORO_STACK_SIZE ((sizeof(zt_coro) + CORO_STACK_ALIGN - 1) & ~(CORO_STACK_ALIGN - 1))

static zt_coro coro_main;
static zt_coro *coro_current = &coro_main;
static zt_coro *coro_helper;

#if defined(HAS_UCONTEXT)

#else  /* HAS_UCONTEXT */

/* This is the system dependent function.
 * Setup a jmp_buf so when we longjmp, it will invoke 'func' using 'stack'.
 * Important: 'func' must not return!
 *
 * Usually done by setting the program counter and stack pointer of a new, empty stack.
 * If you're adding a new platform, look in the setjmp.h for PC and SP members 
 * of the stack structure
 *
 * If you don't see those members, Kentaro suggests writting a simple 
 * test app that calls setjmp and dumps out the contents of the jmp_buf.  
 * (The PC and SP should be in jmp_buf->__jmpbuf).  
 *
 * Using something like GDB to be able to peek into register contents right 
 * before the setjmp occurs would be helpful also.
 */

static int _setup_context(jmp_buf buf, void (*func)(void), void *stack, size_t stacksize)
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
	
  setjmp(buf);
  
  /* #if defined(__MACOSX__) && defined(_BSD_PPC_SETJMP_H_) */
  #if defined(_BSD_PPC_SETJMP_H_) 
    /* OSX/PPC */
    #define setjmp _setjmp /* no need for signal masks */
    #define longjmp _longjmp
    memset(((char *)stack) + stacksize - 64, 0, 64);	/* very crude... */
    buf[0] = ((intptr_t)stack + stacksize - 64 + 15) & ~15;
    buf[21] = (intptr_t)func;

  #elif defined(sgi) && defined(_IRIX4_SIGJBLEN)
    /* Irix/SGI */
    buf[JB_SP] = (__uint64_t)((char *)stack + stacksize - 8);
    buf[JB_PC] = (__uint64_t)func;

  #elif defined(linux) && defined(JB_GPR1)
      /* Linux/PPC */
      buf->__jmpbuf[JB_GPR1] = ((int) stack + stacksize - 64 + 15) & ~15;
      buf->__jmpbuf[JB_LR] = (int) func;
    
  #elif defined(linux) && defined(JB_RBX)
      /* Linux/Opteron */
      buf->__jmpbuf[JB_RSP] = (long int )stack + stacksize;
      buf->__jmpbuf[JB_PC] = func;

  #elif defined(linux) && defined(JB_SP)
      /* Linux/x86 with glibc2 */
      buf->__jmpbuf[JB_SP] = (int)stack + stacksize;
      buf->__jmpbuf[JB_PC] = (int)func;
    
  #elif defined(linux) && defined(_I386_JMP_BUF_H)
      /* x86-linux with libc5 */
      buf->__sp = stackend;
      buf->__pc = func;
    
  #elif defined(linux) && defined(__JMP_BUF_SP)
      /* arm-linux on the sharp zauras */
      buf->__jmpbuf[__JMP_BUF_SP] = (int)stack + stacksize;
      buf->__jmpbuf[__JMP_BUF_SP+1] = (int)func;

  #elif defined(__CYGWIN__) 
    /* Cygwin */
    buf[7] = (int)((int)stack + stacksize);
    buf[8] = (int)func;
    
  #elif defined(__MINGW32__) 
    buf[4] = (int)((unsigned char *)stack + stacksize - 100);   /* esp */
    buf[5] = (int)func; /* eip */
      
  #elif defined(_MSC_VER)
    /* win32 visual c */
    /* should this be the same as __MINGW32__? */
    buf[4] = (int)((unsigned char *)stack + stacksize);  /* esp */
    buf[5] = (int)func; /* eip */
    
  #elif defined(__SYMBIAN32__)
    /* Symbinan. */
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = (unsigned int)stack + CSTACK_STACKSIZE - 64;
    buf[9] = (int)func;
    buf[8] =  buf[3] + 32;

  #elif defined(__FreeBSD__) && defined(_JBLEN) && (_JBLEN == 81)
    /* FreeBSD/Alpha */
    buf->_jb[2] = (long)func;     /* sc_pc */
    buf->_jb[26+4] = (long)func;  /* sc_regs[R_RA] */
    buf->_jb[27+4] = (long)func;  /* sc_regs[R_T12] */
    buf->_jb[30+4] = (long)(stack + stacksize); /* sc_regs[R_SP] */

  #elif  defined(__FreeBSD__) && defined(_JBLEN)
    /* FreeBSD on IA32 */
    buf->_jb[2] = (long)(stack + stacksize);
    buf->_jb[0] = (long)func;

  #elif defined(__NetBSD__) && defined(_JB_ATTRIBUTES)
    /* NetBSD i386. */
    buf[2] = (long)(stack + stacksize);
    buf[0] = (long)func;

  #elif defined(__SVR4) && defined(__sun) && defined(SUN_PROGRAM_COUNTER)
    /* SunOS 9 */
    buf[SUN_PROGRAM_COUNTER] = (JBTYPE)func;
    
    x = (JBTYPE)stack;
    while ((x % 8) != 0) x--; /* align on an even boundary */
    buf[SUN_STACK_START_INDEX] = (JBTYPE)x;
    x = (JBTYPE)((JBTYPE)stack-stacksize/2+15);
    while ((x % 8) != 0) x++; /* align on an even boundary */
    buf[SUN_STACK_END_INDEX] = (JBTYPE)x;

  #else
    #error "no setup_jumpbuf entry for this platform"
  #endif

  return 0;
}

static void _switch_context(zt_coro_ctx *old, zt_coro_ctx *new)
{
	if(!setjmp(old->ctx)) {
		longjmp(new->ctx, 1);
	}
}

#endif	/* HAS_UCONTEXT */


static void _coro_run(void) 
{
	zt_coro	*co = coro_current;

	co->target = co->caller;
	co->func(co->data);
	zt_coro_exit(co->data);
}


zt_coro *zt_coro_create(void *(*func)(void*), void *stack, int size)
{
	int	  alloc = 0;
	zt_coro	* co;

	assert((size &= ~(sizeof(int) - 1)) >= ZT_CORO_MIN_STACK_SIZE);

	if(!stack) {
		size = size + CORO_STACK_SIZE;
		stack = XMALLOC(unsigned char, size);
		alloc = size;
	}

	co = stack;
	stack = (char *) stack + CORO_STACK_SIZE;
	co->alloc = alloc;
	co->func = func;
	co->data = 0;
	
	if(_setup_context(co->ctx.ctx, _coro_run, stack, size - CORO_STACK_SIZE) < 0) {
		if(alloc) {
			free(co);
		}
		return NULL;
	}

	return co;
}

void zt_coro_delete(zt_coro *co) 
{
	if(co == coro_current) {
		printf("coroutine cannot delete itself\n");
		exit(1);
	}

	if(co->alloc){
		free(co);
	}
}


void *zt_coro_call(zt_coro *co, void *data)
{
	zt_coro	*old = coro_current;
	
	co->caller = coro_current;
	coro_current = co;
	
	co->data = data;
	
	_switch_context(&old->ctx, &co->ctx);
	
	return old->data;
}

void *zt_coro_yield(void *data)
{
	data = zt_coro_call(coro_current->target, data);
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
	static zt_coro *helper = NULL;
	static char stk[ZT_CORO_MIN_STACK_SIZE];

	if(!helper &&
	   !(helper = zt_coro_create(_del_helper, stk, sizeof(stk)))) {
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
