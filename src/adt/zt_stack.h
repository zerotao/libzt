#ifndef _ZT_STACK_H_
#define _ZT_STACK_H_

#include <zt_internal.h>
#include <adt/zt_list.h>

BEGIN_C_DECLS

typedef zt_elist_t zt_stack_t;
/* feelgood typedef */
typedef zt_stack_t zt_stack_elt_t;

#define zt_stack(N) zt_stack_t N = zt_stack_init(N)
#define zt_stack_init zt_elist_init
#define zt_stack_reset zt_elist_reset
#define zt_stack_empty zt_elist_empty
#define zt_stack_data zt_elist_data
#define zt_stack_peek zt_elist_get_next
#define zt_stack_push zt_elist_add

static INLINE zt_stack_t *
zt_stack_pop(zt_stack_t *stack)
{
    zt_stack_t * elt;

    if(zt_stack_empty(stack)) {
        return NULL;
    }

    elt = zt_elist_get_next(stack);
    zt_elist_remove(elt);
    return (elt);
}




END_C_DECLS
#endif /* _ZT_STACK_H_ */
