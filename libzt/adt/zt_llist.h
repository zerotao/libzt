/*
 * Copyright (C) 2007 Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 *   See file COPYING for details.
 */

#ifndef _ZT_LLIST_H_
#define _ZT_LLIST_H_

#include <libzt/zt.h>
#include <libzt/zt_assert.h>
BEGIN_C_DECLS

typedef struct zt_pair zt_pair;
struct zt_pair {
    void            * head;
    struct  zt_pair * tail;
};
typedef zt_pair    *zt_llist;

char *IncorrectType = "Incorrect Type";
char *ListTooShort = "List is too short";

#define zt_llist_head(_pair) \
    (_pair)->head
#define zt_llist_tail(_pair) \
    (_pair)->tail

static INLINE void
zt_llist_free(zt_pair *p)
{
    zt_pair * h;
    zt_pair * op;

    while(p != NULL) {
        op = p;
        h = zt_llist_head(p);
        p = zt_llist_tail(p);
        XFREE(op);
    }
}

static INLINE zt_llist
zt_llist_cons(void *head, zt_pair *tail)
{
    zt_llist llist;

    llist = XCALLOC(zt_pair, 1);
    zt_llist_head(llist) = head;
    zt_llist_tail(llist) = tail;
    return (llist);
}

static INLINE void *
zt_llist_nth(zt_llist pair, int offt)
{
    while (pair && offt > 0) {
        --offt;
        pair = zt_llist_tail(pair);
    }

    if (pair == NULL) {
        THROW(ListTooShort);
    }

    return (zt_llist_head(pair));
}

static INLINE zt_llist
zt_llist_nthtail(zt_llist pair, int offt)
{
    while (pair && offt > 0) {
        --offt;
        pair = zt_llist_tail(pair);
    }

    if (pair == NULL) {
        THROW(ListTooShort);
    }
    return (zt_llist_tail(pair));
}

static INLINE zt_llist
zt_llist_ppop(zt_llist *list)
{
    /* pop a pair off the list */
    zt_llist rlist;

    rlist = *list;
    if (rlist != NULL) {
        *list = zt_llist_tail(rlist);
    }
    return (rlist);
}

static INLINE void *
zt_llist_pop(zt_llist *list)
{
    /* pop an item off the list */
    void    * item;
    zt_pair * z;

    z = zt_llist_ppop(list);
    if (z == NULL) {
        return (z);
    }
    item = zt_llist_head(z);
    XFREE(z);
    return (item);
}

static INLINE zt_llist
zt_llist_push(void *item, zt_llist *list)
{
    return (*list = zt_llist_cons(item, *list));
}

static INLINE zt_llist
zt_llist_ppush(zt_pair *p, zt_llist *list)
{
    zt_llist_tail(p) = *list;
    return (*list = p);
}

static INLINE zt_llist
zt_llist_nreverse(zt_llist llist)
{
    zt_llist rlist = NULL;

    while (llist != NULL) {
        zt_llist_ppush(zt_llist_ppop(&llist), &rlist);
    }
    return (rlist);
}

static INLINE zt_llist
zt_llist_reverse(zt_llist llist)
{
    zt_llist rlist = NULL;
    zt_llist p = llist;

    while (p != NULL) {
        void * i = zt_llist_head(p);
        rlist = zt_llist_cons(i, rlist);
        p = zt_llist_tail(p);
    }
    return (rlist);
}

static INLINE zt_llist
zt_llist_nmerge(zt_llist l1, zt_llist l2, int (*cmp)())
{
    zt_llist rlst = NULL;

    while ((l1 != NULL) && (l2 != NULL)) {
        zt_llist_ppush((cmp(zt_llist_head(l2), zt_llist_head(l1)) ? zt_llist_ppop(&l1) : zt_llist_ppop(&l2)), &rlst);
    }

    while ( l1 != NULL ) {
        zt_llist_ppush(zt_llist_ppop(&l1), &rlst);
    }
    while ( l2 != NULL ) {
        zt_llist_ppush(zt_llist_ppop(&l2), &rlst);
    }

    return (zt_llist_nreverse(rlst));
}


zt_llist INLINE zt_llist_split(zt_llist lst)
{
    zt_llist tail;

    if (lst == NULL) {
        return (NULL);
    }
    tail = zt_llist_tail(lst);

    if ((lst == NULL) || (tail == NULL)) {
        return (lst);
    }

    while ((tail != NULL) && ((tail = zt_llist_tail(tail)) != NULL)) {
        lst = zt_llist_tail(lst);
        tail = zt_llist_tail(tail);
    }
    tail = zt_llist_tail(lst);
    zt_llist_tail(lst) = NULL;
    return (tail);
}

static INLINE zt_llist
zt_llist_sort(zt_llist lst, int (*cmp)())
{
    zt_llist lst2;

    if ((lst == NULL) || (zt_llist_tail(lst) == NULL)) {
        return (lst);
    }

    lst2 = zt_llist_split(lst);
    return (zt_llist_nmerge(zt_llist_sort(lst, cmp), zt_llist_sort(lst2, cmp), cmp));
}


/*
 * #define zt_car(_cons) \
 *     zt_nth(_cons, 0)
 *
 * #define zt_cdr(_cons) \
 *     zt_nthtail(_cons, 0)
 *
 * #define zt_caar(_cons) \
 *     zt_car(zt_car(_cons))
 * #define cadr(_cons) \
 *     zt_car(zt_cdr(_cons))
 * #define cdar(_cons) \
 *     zt_cdr(zt_car(_cons))
 * #define cddr(_cons) \
 *     zt_cdr(zt_cdr(_cons))
 *
 * #define caaar(_cons) \
 *     zt_car(zt_caar(_cons))
 *
 * #define cdaar(_cons) \
 *     zt_cdr(zt_caar(_cons))
 */


END_C_DECLS
#endif /* _ZT_LLIST_H_ */
