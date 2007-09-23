#ifndef _zt_coroutine_h_
#define _zt_coroutine_h_

#include <libzt/zt.h>
#include <libzt/zt_except.h>
#include <libzt/zt_assert.h>

#include <libzt/ucontext/portableucontext.h>

#if defined(HAS_UCONTEXT)
# include <ucontext.h>
#else
# include <setjmp.h>        /* setjmp/longjmp */
#endif

#define ZT_CORO_MIN_STACK_SIZE (4 * 1024)

BEGIN_C_DECLS

typedef struct zt_coro_ctx zt_coro_ctx;

typedef struct zt_coro {
	ucontext_t			  ctx;
	struct zt_coro		* caller;
	struct zt_coro		* target;
	void				*(* func)(zt_coro_ctx *, void *);
	void				* data;
	size_t                size;
	struct except_Frame	* except_stack;
} zt_coro;

struct zt_coro_ctx {
	zt_coro				  main;
	zt_coro				* current;
	zt_coro				* helper;
};


extern char     * zt_coro_except_exit;

int zt_coro_init_ctx(zt_coro_ctx *ctx);
zt_coro *zt_coro_create(zt_coro_ctx *ctx, void *(*func)(zt_coro_ctx *, void *), zt_coro *co, size_t size);
void zt_coro_delete(zt_coro_ctx *ctx, zt_coro *co);
void *zt_coro_call(zt_coro_ctx *ctx, zt_coro *co, void *data);
void *zt_coro_yield(zt_coro_ctx *ctx, void *data);
void zt_coro_exit_to(zt_coro_ctx *ctx, zt_coro *co, void *data) NORETURN;
void zt_coro_exit(zt_coro_ctx *ctx, void * data) NORETURN;
zt_coro *zt_coro_get_current(zt_coro_ctx *ctx);
int zt_coro_stack_left(zt_coro_ctx *ctx);

END_C_DECLS
#endif /* _zt_coroutine_h_ */
