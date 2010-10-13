#ifndef _ZT_QUEUE_H_
#define _ZT_QUEUE_H_

#include <libzt/zt_internal.h>
#include <libzt/adt/zt_list.h>
BEGIN_C_DECLS

typedef zt_elist zt_queue;

/* feelgood typedef */
typedef zt_queue zt_queue_elt;

#define zt_queue(N) zt_queue N = zt_queue_init(N)
#define zt_queue_init zt_elist_init
#define zt_queue_reset zt_elist_reset
#define zt_queue_empty zt_elist_empty
#define zt_queue_data zt_elist_data
#define zt_queue_peek zt_elist_next
#define zt_queue_enqueue zt_elist_add

static INLINE zt_queue *
zt_queue_dequeue(zt_queue *queue)
{
    zt_queue * elt;

    elt = zt_elist_get_prev(queue);
    zt_elist_remove(elt);
    return (elt);
}


END_C_DECLS
#endif /* _ZT_QUEUE_H_ */
