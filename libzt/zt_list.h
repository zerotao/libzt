/*
 * zt_list.h        ZeroTao Lists
 *
 * Copyright (C) 2002-2004, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 */
#ifndef _ZT_LIST_H_
#define _ZT_LIST_H_

#include <libzt/zt.h>

typedef struct zt_list {
	struct zt_list	*prev, *next;
}zt_list;

#define zt_list_init(name) { &(name), &(name) }
#define zt_list(name) zt_list name = zt_list_init(name)
#define zt_list_reset(p) do{			\
		(p)->prev = (p);		\
		(p)->next = (p);		\
	}while(0)

#define zt_list_get_next(p) ((p)->next)
#define zt_list_get_prev(p) ((p)->prev)

#define zt_list_entry(ptr, type, member) \
	containerof(ptr, type, member)

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static INLINE void zt_list_add2(zt_list * new,
				zt_list * prev,
				zt_list * next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}

static INLINE void zt_list_add(zt_list *head,
			       zt_list *new)
{
        zt_list_add2(new, head, head->next);
}

static INLINE void zt_list_add_tail(zt_list *head,
				    zt_list *new)
{
        zt_list_add2(new, head->prev, head);
}

static INLINE void zt_list_remove2(zt_list * prev,
				   zt_list * next)
{
        next->prev = prev;
        prev->next = next;
}

static INLINE void zt_list_remove(zt_list *entry)
{
        zt_list_remove2(entry->prev, entry->next);
}

static INLINE int zt_list_empty(zt_list *head)
{
        return head->next == head;
}

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

/*
 * zt_list_for_each is NOT safe for removing entries from the list
 * for this you need to use zt_list_for_each_safe or a counting loop.
 */

#define zt_list_for_each(h, p)				\
	for ((p) = (h)->next; (p) != (h); 		\
	     (p) = (p)->next)

#define zt_list_for_each_safe(h, p, n)			\
	for ((p) = (h)->next, (n) = (p)->next;		\
	     (p) != (h); (p) = (n), (n) = (p)->next)

#endif	/* _ZT_LIST_H_ */
