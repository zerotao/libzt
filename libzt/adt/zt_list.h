/****h* libZT/LinkedList
 * DESCRIPTION
 *
 * COPYRIGHT
 *   Copyright (C) 2002-2005, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 *   See file COPYING for details.
 ****/
#ifndef _ZT_LIST_H_
#define _ZT_LIST_H_

#include <libzt/zt.h>

typedef struct zt_list {
	struct zt_list	*prev, *next;
}zt_list;

/****d* LinkedList/zt_list_init
 *  NAME
 *    zt_list_init
 *****/
#define zt_list_init(name) { &(name), &(name) }

/****d* LinkedList/zt_list
 *  NAME
 *    zt_list
 *****/
#define zt_list(name) zt_list name = zt_list_init(name)

/****d* LinkedList/zt_list_reset
 *  NAME
 *    zt_list_reset
 *****/
#define zt_list_reset(p) do{			\
		(p)->prev = (p);		\
		(p)->next = (p);		\
	}while(0)

/****d* LinkedList/zt_list_get_next
 *  NAME
 *    zt_list_get_next
 *****/
#define zt_list_get_next(p) ((p)->next)

/****d* LinkedList/zt_list_get_prev
 *  NAME
 *    zt_list_get_prev
 *****/
#define zt_list_get_prev(p) ((p)->prev)

/****d* LinkedList/zt_list_entry
 *  NAME
 *    zt_list_entry
 *****/
#define zt_list_entry(ptr, type, member) \
	containerof(ptr, type, member)

/****f* LinkedList/zt_list_add2
 *  NAME
 *    zt_list_add2
 *
 * DESCRIPTION
 *   Insert a new entry between two known consecutive entries. 
 *
 *   This is only for internal list manipulation where we know
 *   the prev/next entries already!
 *
 *  SYNOPSIS
 *    static INLINE void zt_list_add2(zt_list * new, zt_list *prev, zt_list *next)
 *****/
static INLINE void zt_list_add2(zt_list * new, zt_list * prev, zt_list * next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}

/****f* LinkedList/zt_list_add
 *  NAME
 *    zt_list_add
 *  SYNOPSIS
 *    static INLINE void zt_list_add(zt_list *head, zt_list *new)
 *****/
static INLINE void zt_list_add(zt_list *head, zt_list *new)
{
        zt_list_add2(new, head, head->next);
}

/****f* LinkedList/zt_list_add_tail
 *  NAME
 *    zt_list_add_tail
 *  SYNOPSIS
 *    static INLINE void zt_list_add_tail(zt_list *head, zt_list *new)
 *****/
static INLINE void zt_list_add_tail(zt_list *head,
				    zt_list *new)
{
        zt_list_add2(new, head->prev, head);
}

/****f* LinkedList/zt_list_remove2
 *  NAME
 *    zt_list_remove2
 *  SYNOPSIS
 *    static INLINE void zt_list_remove2(zt_list * prev, zt_list *next)
 *****/
static INLINE void zt_list_remove2(zt_list * prev,
				   zt_list * next)
{
        next->prev = prev;
        prev->next = next;
}

/****f* LinkedList/zt_list_remove
 *  NAME
 *    zt_list_remove
 *  SYNOPSIS
 *    static INLINE void zt_list_remove(zt_list *entry)
 *****/
static INLINE void zt_list_remove(zt_list *entry)
{
        zt_list_remove2(entry->prev, entry->next);
}

/****f* LinkedList/zt_list_empty
 *  NAME
 *    zt_list_empty
 *  SYNOPSIS
 *    static INLINE int zt_list_empty(zt_list *head)
 *****/
static INLINE int zt_list_empty(zt_list *head)
{
        return head->next == head;
}

/****f* LinkedList/zt_list_splice
 *  NAME
 *    zt_list_splice
 *  SYNOPSIS
 *    static INLINE void zt_list_splice(zt_list *head, zt_list *list)
 *****/
static INLINE void zt_list_splice(zt_list *head,
				  zt_list *list)
{
        zt_list *first = list->next;

        if (first != list) {
                zt_list *last = list->prev;
                zt_list *at = head->next;

                first->prev = head;
                head->next = first;

                last->next = at;
                at->prev = last;
        }
}

/****d* LinkedList/zt_list_for_each
 *  NAME
 *    zt_list_for_each
 * NOTES
 *   zt_list_for_each is NOT safe for removing entries from the list
 *   for this you need to use zt_list_for_each_safe or a counting loop.
 *****/
#define zt_list_for_each(h, p)				\
	for ((p) = (h)->next; (p) != (h); 		\
	     (p) = (p)->next)

/****d* LinkedList/zt_list_for_each_safe
 *  NAME
 *    zt_list_for_each_safe
 *****/
#define zt_list_for_each_safe(h, p, n)			\
	for ((p) = (h)->next, (n) = (p)->next;		\
	     (p) != (h); (p) = (n), (n) = (p)->next)

#endif	/* _ZT_LIST_H_ */
