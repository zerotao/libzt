#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/tree.h>
#include <sys/queue.h>

#include "zt_bcast.h"

struct zt_bcast_node {
    zt_bcast_t * bcast;
    void       * arg;

    TAILQ_ENTRY(zt_bcast_node) next;
};

struct zt_bcast_set {
    char * name;
    void * arg;
    int    nnodes;

    SPLAY_ENTRY(zt_bcast_set) next;
    TAILQ_HEAD(zt_bcast_setq, zt_bcast_node) nodes;
};

struct zt_bcast_cfg {
    void * stub;
};

SPLAY_HEAD(zt_bcast_sets, zt_bcast_set);
TAILQ_HEAD(zt_bcast_nodes, zt_bcast_node);

struct zt_bcast {
    zt_bcast_cfg_t * cfg;
    zt_bcast_sets_t  sets;
    unsigned int     nsets;
    void           * arg;
};

static int zt_bcast_set_cmp(zt_bcast_set_t *, zt_bcast_set_t *);

SPLAY_PROTOTYPE(zt_bcast_sets, zt_bcast_set, next, zt_bcast_set_cmp);
SPLAY_GENERATE(zt_bcast_sets, zt_bcast_set, next, zt_bcast_set_cmp);

static int
zt_bcast_set_cmp(zt_bcast_set_t * a, zt_bcast_set_t * b) {
    return strcmp(a->name, b->name);
}

zt_bcast_cfg_t *
zt_bcast_cfg_new(void * stub) {
    zt_bcast_cfg_t * cfg = NULL;

    if (!(cfg = zt_calloc(zt_bcast_cfg_t, sizeof(char)))) {
        return NULL;
    }

    cfg->stub = stub;
    return cfg;
}

zt_bcast_set_t *
zt_bcast_set_find(zt_bcast_t * bcast, const char * name) {
    zt_bcast_set_t find = {
        .name = (char *)name
    };

    if (bcast == NULL || name == NULL) {
        return NULL;
    }

    return SPLAY_FIND(zt_bcast_sets, &bcast->sets, &find);
}

zt_bcast_set_t *
zt_bcast_set_new(zt_bcast_t * bcast, const char * name, void * arg) {
    zt_bcast_set_t * set = NULL;

    if (bcast == NULL || name == NULL) {
        return NULL;
    }

    set = zt_bcast_set_find(bcast, name);

    if (set != NULL && set->arg == arg) {
        return arg;
    }

    if (!(set = zt_calloc(zt_bcast_set_t, sizeof(char)))) {
        return NULL;
    }

    set->name     = zt_strdup(name);
    set->arg      = arg;
    set->nnodes   = 0;
    bcast->nsets += 1;

    TAILQ_INIT(&set->nodes);
    SPLAY_INSERT(zt_bcast_sets, &bcast->sets, set);

    return set;
}

zt_bcast_node_t *
zt_bcast_node_new(zt_bcast_t * bcast, const char * set_name, void * arg) {
    zt_bcast_node_t * node = NULL;
    zt_bcast_set_t  * set  = NULL;

    if (bcast == NULL || set_name == NULL) {
        return NULL;
    }

    if (!(set = zt_bcast_set_find(bcast, set_name))) {
        if (!(set = zt_bcast_set_new(bcast, set_name, arg))) {
            return NULL;
        }
    }

    if (!(node = zt_calloc(zt_bcast_node_t, sizeof(char)))) {
        return NULL;
    }

    node->bcast  = bcast;
    node->arg    = arg;
    set->nnodes += 1;

    TAILQ_INSERT_TAIL(&set->nodes, node, next);

    return node;
}

zt_bcast_t *
zt_bcast_new(zt_bcast_cfg_t * cfg, void * arg) {
    zt_bcast_t * bcast = NULL;

    if (!(bcast = zt_calloc(zt_bcast_t, sizeof(char)))) {
        return NULL;
    }

    /* FIXME: create a cfg copy function once we actually have more config options */
    bcast->cfg   = zt_bcast_cfg_new(cfg ? cfg->stub : NULL);
    bcast->arg   = arg;
    bcast->nsets = 0;

    SPLAY_INIT(&bcast->sets);

    return bcast;
}

int
zt_bcast_dispatch(zt_bcast_t * bcast, const char * set_name, zt_bcast_dispatch_cb dispatch_cb, void * arg) {
    zt_bcast_set_t  * set       = NULL;
    zt_bcast_node_t * node      = NULL;
    void            * bcast_arg = NULL;
    void            * set_arg   = NULL;

    if (bcast == NULL || set_name == NULL || dispatch_cb == NULL) {
        return -1;
    }

    if (!(set = zt_bcast_set_find(bcast, set_name))) {
        return -1;
    }

    bcast_arg = bcast->arg;
    set_arg   = set->arg;

    TAILQ_FOREACH(node, &set->nodes, next) {
        void * node_arg = node->arg;

        if (dispatch_cb(set_name, bcast_arg, set_arg, node_arg, arg) < 0) {
            return -1;
        }
    }

    return 0;
}
