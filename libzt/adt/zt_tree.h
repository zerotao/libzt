/*
 * Copyright 2005 Jason L. Shiffer <jshiffer@zerotao.org>
 *
 * See zt_tree.c for a description of changes
 */
/*
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _ZT_TREE_H_
#define _ZT_TREE_H_

#include <libzt/zt_internal.h>
BEGIN_C_DECLS

typedef struct zt_rbt zt_rbt;
struct zt_rbt {
    enum { zt_rbt_red, zt_rbt_black } colour;
    struct zt_rbt *left, *right, *parent;
};

/* for readability */
typedef zt_rbt zt_rbt_node;

#define zt_rbt_init(n) n = { NULL, NULL, NULL }
#define zt_rbt(n) zt_rbt n = { NULL, NULL, NULL }

#define zt_rbt_data(ptr, type, elt) \
    containerof(ptr, type, elt)

#define zt_rbt_left(e) ((e)->left)
#define zt_rbt_right(e) ((e)->right)
#define zt_rbt_parent(e) ((e)->parent)
#define zt_rbt_colour(e) ((e)->colour)
#define zt_rbt_empty(e) ((e)->parent == NULL)

zt_rbt_node *zt_rbt_remove(zt_rbt **head, zt_rbt_node *elm);
zt_rbt_node *zt_rbt_insert(zt_rbt * *head, zt_rbt_node * elm, int (*cmp)(zt_rbt_node *, zt_rbt_node *));
zt_rbt_node *zt_rbt_find(zt_rbt * *head, zt_rbt_node * elm, int (*cmp)(zt_rbt_node *, zt_rbt_node *));
zt_rbt_node *zt_rbt_next(zt_rbt_node *elm);
zt_rbt_node *zt_rbt_minmax(zt_rbt **head, int val);

#define ZT_RBT_NEGINF    -1
#define ZT_RBT_INF     1

#define zt_rbt_min(x) zt_rbt_minmax((x), ZT_RBT_NEGINF)
#define zt_rbt_max(x) zt_rbt_minmax((x), ZT_RBT_INF)

#define zt_rbt_for_each(head, x) \
    for ((x) = zt_rbt_min(head); \
         (x) != NULL;            \
         (x) = zt_rbt_next((x)))

#define zt_rbt_for_each_safe(head, x, n)                 \
    for ((x) = zt_rbt_min(head), (n) = zt_rbt_next((x)); \
         (x) != NULL;                                    \
         (x) = (n), (n) = (n) ? zt_rbt_next((n)) : NULL)


END_C_DECLS
#endif /* _ZT_TREE_H_ */
