/*
 * Copyright 2005 Jason L. Shiffer <jshiffer@zerotao.org>
 *
 * Altered from the BSD sys/tree.h for the most part only the
 * red-black algorithm remains.  It now uses an embedded type, similar
 * if not the same as the embedded list api.
 *
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


#include <libzt/zt_internal.h>
#include "zt_tree.h"


static void zt_rbt_set(zt_rbt_node * e, zt_rbt_node *p);
static void zt_rbt_set_blackred(zt_rbt_node *black, zt_rbt_node *red);
static void zt_rbt_insert_colour(zt_rbt **head, zt_rbt_node *elt);
static void zt_rbt_remove_colour(zt_rbt **head, zt_rbt_node *parent, zt_rbt_node *elm);

static void
zt_rbt_set(zt_rbt_node * e, zt_rbt_node *p)
{
    zt_rbt_parent(e) = p;
    zt_rbt_left(e) = zt_rbt_right(e) = NULL;
    zt_rbt_colour(e) = zt_rbt_red;
}

static void
zt_rbt_set_blackred(zt_rbt_node *black, zt_rbt_node *red)
{
    zt_rbt_colour(black) = zt_rbt_black;
    zt_rbt_colour(red) = zt_rbt_red;
}

#define zt_rbt_rotate_left(head, elm, tmp) do {                       \
        (tmp) = zt_rbt_right(elm);                                    \
        if ((zt_rbt_right(elm) = zt_rbt_left(tmp))) {                 \
                 zt_rbt_parent(zt_rbt_left(tmp)) = (elm);             \
             }                                                        \
             if ((zt_rbt_parent(tmp) = zt_rbt_parent(elm))) {         \
                      if ((elm) == zt_rbt_left(zt_rbt_parent(elm))) { \
                          zt_rbt_left(zt_rbt_parent(elm)) = (tmp); }  \
                      else {                                          \
                          zt_rbt_right(zt_rbt_parent(elm)) = (tmp); } \
                  } else {                                            \
                      *(head) = (tmp); }                              \
                  zt_rbt_left(tmp) = (elm);                           \
                  zt_rbt_parent(elm) = (tmp);                         \
                  } while (0)

#define zt_rbt_rotate_right(head, elm, tmp) do {                  \
    (tmp) = zt_rbt_left(elm);                                     \
    if ((zt_rbt_left(elm) = zt_rbt_right(tmp))) {                 \
             zt_rbt_parent(zt_rbt_right(tmp)) = (elm);            \
         }                                                        \
         if ((zt_rbt_parent(tmp) = zt_rbt_parent(elm))) {         \
                  if ((elm) == zt_rbt_left(zt_rbt_parent(elm))) { \
                      zt_rbt_left(zt_rbt_parent(elm)) = (tmp); }  \
                  else {                                          \
                      zt_rbt_right(zt_rbt_parent(elm)) = (tmp); } \
              } else {                                            \
                  *(head) = (tmp); }                              \
              zt_rbt_right(tmp) = (elm);                          \
              zt_rbt_parent(elm) = (tmp);                         \
              } while (0)


static void
zt_rbt_insert_colour(zt_rbt **head, zt_rbt_node *elt)
{
    zt_rbt_node * parent;
    zt_rbt_node * gparent;
    zt_rbt_node * tmp;

    while ((parent = zt_rbt_parent(elt)) &&
           zt_rbt_colour(parent) == zt_rbt_red) {
        gparent = zt_rbt_parent(parent);
        if (parent == zt_rbt_left(gparent)) {
            tmp = zt_rbt_right(gparent);
            if (tmp && zt_rbt_colour(tmp) == zt_rbt_red) {
                zt_rbt_colour(tmp) = zt_rbt_black;
                zt_rbt_set_blackred(parent, gparent);
                elt = gparent;
                continue;
            }
            if (zt_rbt_right(parent) == elt) {
                zt_rbt_rotate_left(head, parent, tmp);
                tmp = parent;
                parent = elt;
                elt = tmp;
            }
            zt_rbt_set_blackred(parent, gparent);
            zt_rbt_rotate_right(head, gparent, tmp);
        } else {
            tmp = zt_rbt_left(gparent);
            if (tmp && zt_rbt_colour(tmp) == zt_rbt_red) {
                zt_rbt_colour(tmp) = zt_rbt_black;
                zt_rbt_set_blackred(parent, gparent);
                elt = gparent;
                continue;
            }
            if (zt_rbt_left(parent) == elt) {
                zt_rbt_rotate_right(head, parent, tmp);
                tmp = parent;
                parent = elt;
                elt = tmp;
            }
            zt_rbt_set_blackred(parent, gparent);
            zt_rbt_rotate_left(head, gparent, tmp);
        }
    }
    zt_rbt_colour(*head) = zt_rbt_black;
} /* zt_rbt_insert_colour */

void
zt_rbt_remove_colour(zt_rbt **head, zt_rbt_node *parent, zt_rbt_node *elm)
{
    zt_rbt_node * tmp;

    while ((elm == NULL || zt_rbt_colour(elm) == zt_rbt_black) &&
           elm != *head) {
        if (zt_rbt_left(parent) == elm) {
            tmp = zt_rbt_right(parent);
            if (zt_rbt_colour(tmp) == zt_rbt_red) {
                zt_rbt_set_blackred(tmp, parent);
                zt_rbt_rotate_left(head, parent, tmp);
                if ((tmp = zt_rbt_right(parent)) == NULL) {
                    zt_log_dprintf(zt_log_err, "Tree corruption error"), abort();
                }
            }
            if ((zt_rbt_left(tmp) == NULL ||
                 zt_rbt_colour(zt_rbt_left(tmp)) == zt_rbt_black) &&
                (zt_rbt_right(tmp) == NULL ||
                 zt_rbt_colour(zt_rbt_right(tmp)) == zt_rbt_black)) {
                zt_rbt_colour(tmp) = zt_rbt_red;
                elm = parent;
                parent = zt_rbt_parent(elm);
            } else {
                if (zt_rbt_right(tmp) == NULL ||
                    zt_rbt_colour(zt_rbt_right(tmp)) == zt_rbt_black) {
                    zt_rbt_node * oleft;

                    if ((oleft = zt_rbt_left(tmp))) {
                        zt_rbt_colour(oleft) = zt_rbt_black;
                    }
                    zt_rbt_colour(tmp) = zt_rbt_red;
                    zt_rbt_rotate_right(head, tmp, oleft);
                    tmp = zt_rbt_right(parent);
                }
                zt_rbt_colour(tmp) = zt_rbt_colour(parent);
                zt_rbt_colour(parent) = zt_rbt_black;
                if (zt_rbt_right(tmp)) {
                    zt_rbt_colour(zt_rbt_right(tmp)) = zt_rbt_black;
                }
                zt_rbt_rotate_left(head, parent, tmp);
                elm = *head;
                break;
            }
        } else {
            tmp = zt_rbt_left(parent);
            if (zt_rbt_colour(tmp) == zt_rbt_red) {
                zt_rbt_set_blackred(tmp, parent);
                zt_rbt_rotate_right(head, parent, tmp);
                if ((tmp = zt_rbt_left(parent)) == NULL) {
                    zt_log_dprintf(zt_log_err, "Tree corruption error"), abort();
                }
            }
            if ((zt_rbt_left(tmp) == NULL ||
                 zt_rbt_colour(zt_rbt_left(tmp)) == zt_rbt_black) &&
                (zt_rbt_right(tmp) == NULL ||
                 zt_rbt_colour(zt_rbt_right(tmp)) == zt_rbt_black)) {
                zt_rbt_colour(tmp) = zt_rbt_red;
                elm = parent;
                parent = zt_rbt_parent(elm);
            } else {
                if (zt_rbt_left(tmp) == NULL ||
                    zt_rbt_colour(zt_rbt_left(tmp)) == zt_rbt_black) {
                    zt_rbt_node * oright;

                    if ((oright = zt_rbt_right(tmp))) {
                        zt_rbt_colour(oright) = zt_rbt_black;
                    }
                    zt_rbt_colour(tmp) = zt_rbt_red;
                    zt_rbt_rotate_left(head, tmp, oright);
                    tmp = zt_rbt_left(parent);
                }
                zt_rbt_colour(tmp) = zt_rbt_colour(parent);
                zt_rbt_colour(parent) = zt_rbt_black;
                if (zt_rbt_left(tmp)) {
                    zt_rbt_colour(zt_rbt_left(tmp)) = zt_rbt_black;
                }
                zt_rbt_rotate_right(head, parent, tmp);
                elm = *head;
                break;
            }
        }
    }
    if (elm) {
        zt_rbt_colour(elm) = zt_rbt_black;
    }
} /* zt_rbt_remove_colour */


zt_rbt_node *
zt_rbt_remove(zt_rbt **head, zt_rbt_node *elm)
{
    zt_rbt_node * child;
    zt_rbt_node * parent;
    zt_rbt_node * old = elm;

    int           colour;

    if (zt_rbt_left(elm) == NULL) {
        child = zt_rbt_right(elm);
    } else if (zt_rbt_right(elm) == NULL) {
        child = zt_rbt_left(elm);
    } else {
        zt_rbt_node * left;

        elm = zt_rbt_right(elm);
        while ((left = zt_rbt_left(elm))) {
            elm = left;
        }

        child = zt_rbt_right(elm);
        parent = zt_rbt_parent(elm);
        colour = zt_rbt_colour(elm);
        if (child) {
            zt_rbt_parent(child) = parent;
        }
        if (parent) {
            if (zt_rbt_left(parent) == elm) {
                zt_rbt_left(parent) = child;
            } else {
                zt_rbt_right(parent) = child;
            }
        } else {
            *head = child;
        }
        if (zt_rbt_parent(elm) == old) {
            parent = elm;
        }

        zt_rbt_left(elm) = zt_rbt_left(old);
        zt_rbt_right(elm) = zt_rbt_right(old);
        zt_rbt_parent(elm) = zt_rbt_parent(old);
        zt_rbt_colour(elm) = zt_rbt_colour(old);

        if (zt_rbt_parent(old)) {
            if (zt_rbt_left(zt_rbt_parent(old)) == old) {
                zt_rbt_left(zt_rbt_parent(old)) = elm;
            } else {
                zt_rbt_right(zt_rbt_parent(old)) = elm;
            }
        } else {
            *head = elm;
        }

        zt_rbt_parent(zt_rbt_left(old)) = elm;
        if (zt_rbt_right(old)) {
            zt_rbt_parent(zt_rbt_right(old)) = elm;
        }
        /*
         * if (parent) {
         *     left = parent;
         *     while ((left = zt_rbt_parent(left)));
         * }
         */
        goto colour;
    }
    parent = zt_rbt_parent(elm);
    colour = zt_rbt_colour(elm);
    if (child) {
        zt_rbt_parent(child) = parent;
    }
    if (parent) {
        if (zt_rbt_left(parent) == elm) {
            zt_rbt_left(parent) = child;
        } else {
            zt_rbt_right(parent) = child;
        }
    } else {
        *head = child;
    }
colour:
    if (colour == zt_rbt_black) {
        zt_rbt_remove_colour(head, parent, child);
    }
    return old;
} /* zt_rbt_remove */

/* Inserts a node into the RB tree */
zt_rbt_node *
zt_rbt_insert(zt_rbt **head, zt_rbt_node *elm, int (*cmp)(zt_rbt_node *, zt_rbt_node *))
{
    zt_rbt_node * tmp;
    zt_rbt_node * parent = NULL;
    int           comp = 0;

    tmp = *head;
    while (tmp) {
        parent = tmp;
        comp = (cmp)(elm, parent);
        if (comp < 0) {
            tmp = zt_rbt_left(tmp);
        } else if (comp > 0) {
            tmp = zt_rbt_right(tmp);
        } else {
            return tmp;
        }
    }
    zt_rbt_set(elm, parent);
    if (parent != NULL) {
        if (comp < 0) {
            zt_rbt_left(parent) = elm;
        } else {
            zt_rbt_right(parent) = elm;
        }
    } else {
        *head = elm;
    }

    zt_rbt_insert_colour(head, elm);
    return NULL;
}

/* Finds the node with the same key as elm */
zt_rbt_node *
zt_rbt_find(zt_rbt **head, zt_rbt_node *elm, int (*cmp)(zt_rbt_node *, zt_rbt_node *))
{
    zt_rbt_node *tmp = *head;

    int          comp;

    while (tmp) {
        comp = cmp(elm, tmp);
        if (comp < 0) {
            tmp = zt_rbt_left(tmp);
        } else if (comp > 0) {
            tmp = zt_rbt_right(tmp);
        } else {
            return tmp;
        }
    }
    return NULL;
}

/* find first node greater than or equal to the search key */
zt_rbt_node *
zt_rbt_nfind(zt_rbt **head, zt_rbt_node *elm, int (*cmp)(zt_rbt_node *, zt_rbt_node *))
{
    zt_rbt_node *tmp = *head;
    zt_rbt_node *res = NULL;
    int          comp;

    while (tmp) {
        comp = cmp(elm, tmp);
        if (comp < 0) {
            res = tmp;
            tmp = zt_rbt_left(tmp);
        } else if (comp > 0) {
            tmp = zt_rbt_right(tmp);
        } else {
            return tmp;
        }
    }

    return res;
}

zt_rbt_node *
zt_rbt_next(zt_rbt_node *elm)
{
    if (!elm) {
        return NULL;
    }

    if (zt_rbt_right(elm)) {
        elm = zt_rbt_right(elm);
        while (zt_rbt_left(elm)) {
            elm = zt_rbt_left(elm);
        }
    } else {
        if (zt_rbt_parent(elm) &&
            (elm == zt_rbt_left(zt_rbt_parent(elm)))) {
            elm = zt_rbt_parent(elm);
        } else {
            while (zt_rbt_parent(elm) &&
                   (elm == zt_rbt_right(zt_rbt_parent(elm)))) {
                elm = zt_rbt_parent(elm);
            }
            elm = zt_rbt_parent(elm);
        }
    }
    return elm;
}

zt_rbt_node *
zt_rbt_minmax(zt_rbt **head, int val)
{
    zt_rbt_node *tmp = *head;
    zt_rbt_node *parent = NULL;

    while (tmp) {
        parent = tmp;
        if (val < 0) {
            tmp = zt_rbt_left(tmp);
        } else {
            tmp = zt_rbt_right(tmp);
        }
    }
    return parent;
}
