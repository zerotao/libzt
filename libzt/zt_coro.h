#ifndef _zt_coroutine_h_
#define _zt_coroutine_h_

#include <libzt/zt.h>
#include <setjmp.h>		/* setjmp/longjmp */


#define ZT_CORO_MIN_STACK_SIZE (4 * 1024)

BEGIN_C_DECLS

typedef struct zt_coro_ctx {
	jmp_buf		ctx;
} zt_coro_ctx;

typedef struct zt_coro {
	zt_coro_ctx	  ctx;
	int		  alloc;
	struct zt_coro	* caller;
	struct zt_coro	* target;
	void	      *(* func)(void *);
	void		* data;
} zt_coro;

zt_coro *zt_coro_create(void *(*func)(void *), void *stack, int size);
void zt_coro_delete(zt_coro *co);
void *zt_coro_call(zt_coro *co, void *data);
void *zt_coro_yield(void *data);
void zt_coro_exit_to(zt_coro *co, void *data) NORETURN;
void zt_coro_exit(void * data) NORETURN;
zt_coro *zt_coro_get_current(void);

END_C_DECLS
#endif /* _zt_coroutine_h_ */
