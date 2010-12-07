/*
 * Copyright (C) 2002-2005, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 *   See file COPYING for details.
 */

#ifndef _ZT_LIST_H_
#define _ZT_LIST_H_

#include <zt_internal.h>
BEGIN_C_DECLS
/*
 *  elists are based on an article I read about linux kernel linked lists.
 *
 */
typedef struct zt_elist zt_elist_t;
struct zt_elist {
    struct zt_elist *prev, *next;
};

/* convienience typedef */
typedef zt_elist_t zt_elist_elt;

#define zt_elist_init(N) { &(N), &(N) }

#define zt_elist(N) zt_elist_t N = zt_elist_init(N)

#define zt_elist_reset(P) do { \
        (P)->prev = (P);       \
        (P)->next = (P);       \
} while (0)

#define zt_elist_get_next(P) ((P)->next)

#define zt_elist_get_prev(P) ((P)->prev)

#define zt_elist_data(PTR, TYPE, ELT) \
    containerof(PTR, TYPE, ELT)

#define zt_elist_for_each(h, p)       \
    for ((p) = (h)->next; (p) != (h); \
         (p) = (p)->next)

#define zt_elist_for_each_safe(h, p, n)    \
    for ((p) = (h)->next, (n) = (p)->next; \
         (p) != (h); (p) = (n), (n) = (n)->next)

static INLINE int
zt_elist_empty(zt_elist_t *head)
{
    return (head->next == head);
}


static INLINE void
zt_elist_remove2(zt_elist_t * prev, zt_elist_t * next)
{
    next->prev = prev;
    prev->next = next;
}

static INLINE void
zt_elist_remove(zt_elist_t *entry)
{
    zt_elist_remove2(entry->prev, entry->next);
}

static INLINE void
zt_elist_add2(zt_elist_t * nlist, zt_elist_t * prev, zt_elist_t * next)
{
    next->prev = nlist;
    nlist->next = next;
    nlist->prev = prev;
    prev->next = nlist;
}

static INLINE void
zt_elist_add(zt_elist_t *head, zt_elist_t *nlist)
{
    zt_elist_add2(nlist, head, head->next);
}

static INLINE void
zt_elist_add_tail(zt_elist_t *head, zt_elist_t *nlist)
{
    zt_elist_add2(nlist, head->prev, head);
}

static INLINE void
zt_elist_join(zt_elist_t *head, zt_elist_t *list)
{
    zt_elist_t *first = list->next;
    zt_elist_t *last = list->prev;
    zt_elist_t *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

END_C_DECLS
#endif  /* _ZT_LIST_H_ */
