/****h* libZT/LinkedList
 * DESCRIPTION
 *
 * COPYRIGHT
 *   Copyright (C) 2002-2005, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 *   See file COPYING for details.
 ****/
#ifndef _ZT_ELIST_H_
#define _ZT_ELIST_H_

#include <libzt/zt.h>

typedef struct zt_elist {
	struct zt_elist	*prev, *next;
}zt_elist;

/****d* LinkedList/zt_elist_init
 *  NAME
 *    zt_elist_init
 *****/
#define zt_elist_init(name) { &(name), &(name) }

/****d* LinkedList/zt_elist
 *  NAME
 *    zt_elist
 *****/
#define zt_elist(name) zt_elist name = zt_elist_init(name)

/****d* LinkedList/zt_elist_reset
 *  NAME
 *    zt_elist_reset
 *****/
#define zt_elist_reset(p) do{			\
		(p)->prev = (p);		\
		(p)->next = (p);		\
	}while(0)

/****d* LinkedList/zt_elist_get_next
 *  NAME
 *    zt_elist_get_next
 *****/
#define zt_elist_get_next(p) ((p)->next)

/****d* LinkedList/zt_elist_get_prev
 *  NAME
 *    zt_elist_get_prev
 *****/
#define zt_elist_get_prev(p) ((p)->prev)

/****d* LinkedList/zt_elist_entry
 *  NAME
 *    zt_elist_entry
 *****/
#define zt_elist_entry(ptr, type, member) \
	containerof(ptr, type, member)

/****f* LinkedList/zt_elist_add2
 *  NAME
 *    zt_elist_add2
 *
 * DESCRIPTION
 *   Insert a new entry between two known consecutive entries. 
 *
 *   This is only for internal list manipulation where we know
 *   the prev/next entries already!
 *
 *  SYNOPSIS
 *    static INLINE void zt_elist_add2(zt_elist * new, zt_elist *prev, zt_elist *next)
 *****/
static INLINE void zt_elist_add2(zt_elist * new, zt_elist * prev, zt_elist * next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}

/****f* LinkedList/zt_elist_add
 *  NAME
 *    zt_elist_add
 *  SYNOPSIS
 *    static INLINE void zt_elist_add(zt_elist *head, zt_elist *new)
 *****/
static INLINE void zt_elist_add(zt_elist *head, zt_elist *new)
{
        zt_elist_add2(new, head, head->next);
}

/****f* LinkedList/zt_elist_add_tail
 *  NAME
 *    zt_elist_add_tail
 *  SYNOPSIS
 *    static INLINE void zt_elist_add_tail(zt_elist *head, zt_elist *new)
 *****/
static INLINE void zt_elist_add_tail(zt_elist *head,
				    zt_elist *new)
{
        zt_elist_add2(new, head->prev, head);
}

/****f* LinkedList/zt_elist_remove2
 *  NAME
 *    zt_elist_remove2
 *  SYNOPSIS
 *    static INLINE void zt_elist_remove2(zt_elist * prev, zt_elist *next)
 *****/
static INLINE void zt_elist_remove2(zt_elist * prev,
				   zt_elist * next)
{
        next->prev = prev;
        prev->next = next;
}

/****f* LinkedList/zt_elist_remove
 *  NAME
 *    zt_elist_remove
 *  SYNOPSIS
 *    static INLINE void zt_elist_remove(zt_elist *entry)
 *****/
static INLINE void zt_elist_remove(zt_elist *entry)
{
        zt_elist_remove2(entry->prev, entry->next);
}

/****f* LinkedList/zt_elist_empty
 *  NAME
 *    zt_elist_empty
 *  SYNOPSIS
 *    static INLINE int zt_elist_empty(zt_elist *head)
 *****/
static INLINE int zt_elist_empty(zt_elist *head)
{
        return head->next == head;
}

/****f* LinkedList/zt_elist_splice
 *  NAME
 *    zt_elist_splice
 *  SYNOPSIS
 *    static INLINE void zt_elist_splice(zt_elist *head, zt_elist *list)
 *****/
static INLINE void zt_elist_splice(zt_elist *head,
				  zt_elist *list)
{
        zt_elist *first = list->next;

        if (first != list) {
                zt_elist *last = list->prev;
                zt_elist *at = head->next;

                first->prev = head;
                head->next = first;

                last->next = at;
                at->prev = last;
        }
}

/****d* LinkedList/zt_elist_for_each
 *  NAME
 *    zt_elist_for_each
 * NOTES
 *   zt_elist_for_each is NOT safe for removing entries from the list
 *   for this you need to use zt_elist_for_each_safe or a counting loop.
 *****/
#define zt_elist_for_each(h, p)				\
	for ((p) = (h)->next; (p) != (h); 		\
	     (p) = (p)->next)

/****d* LinkedList/zt_elist_for_each_safe
 *  NAME
 *    zt_elist_for_each_safe
 *****/
#define zt_elist_for_each_safe(h, p, n)			\
	for ((p) = (h)->next, (n) = (p)->next;		\
	     (p) != (h); (p) = (n), (n) = (p)->next)

#endif	/* _ZT_ELIST_H_ */
