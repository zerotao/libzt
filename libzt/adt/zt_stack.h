#ifndef _ZT_STACK_H_
#define _ZT_STACK_H_

#include <libzt/zt_internal.h>
#include <libzt/adt/zt_list.h>

BEGIN_C_DECLS

typedef zt_elist zt_stack;
/* feelgood typedef */
typedef zt_stack zt_stack_elt;

#define zt_stack(N) zt_stack N = zt_stack_init(N)
#define zt_stack_init zt_elist_init
#define zt_stack_reset zt_elist_reset
#define zt_stack_empty zt_elist_empty
#define zt_stack_data zt_elist_data
#define zt_stack_peek zt_elist_get_next
#define zt_stack_push zt_elist_add

static INLINE zt_stack *
zt_stack_pop(zt_stack *stack)
{
    zt_stack * elt;

    if(zt_stack_empty(stack)) {
        return NULL;
    }

    elt = zt_elist_get_next(stack);
    zt_elist_remove(elt);
    return (elt);
}




END_C_DECLS
#endif /* _ZT_STACK_H_ */
