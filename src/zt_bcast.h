/*
 * API for "broadcasting" data to multiple destinations.
 *
 * The zt_bcast structure includes a tree of "sets".
 * A set can be defined as a group, each set is identified by a unique name.
 * Each set consists of 0 or more zt_bcast_t nodes.
 *
 * If the zt_bcast_dispatch() function is called with a set named "foo", all
 * zt_bcast_node's will be dispatched to a user-defined callback.
 */


#ifndef __ZT_BCAST_H__
#define __ZT_BCAST_H__

#include <zt_internal.h>

BEGIN_C_DECLS

typedef struct zt_bcast       zt_bcast_t;
typedef struct zt_bcast_cfg   zt_bcast_cfg_t;
typedef struct zt_bcast_set   zt_bcast_set_t;
typedef struct zt_bcast_sets  zt_bcast_sets_t;
typedef struct zt_bcast_nodes zt_bcast_nodes_t;
typedef struct zt_bcast_node  zt_bcast_node_t;

struct zt_bcast;
struct zt_bcast_cfg;
struct zt_bcast_set;
struct zt_bcast_sets;
struct zt_bcast_nodes;
struct zt_bcast_node;

typedef int (*zt_bcast_dispatch_cb)(const char * set_name, void * bcast_arg, void * set_arg, void * node_arg, void * arg);

int               zt_bcast_dispatch(zt_bcast_t * bcast, const char * set_name, zt_bcast_dispatch_cb dispatch_cb, void * arg);
zt_bcast_cfg_t  * zt_bcast_cfg_new(void * stub);
zt_bcast_set_t  * zt_bcast_set_find(zt_bcast_t * bcast, const char * name);
zt_bcast_set_t  * zt_bcast_set_new(zt_bcast_t * bcast, const char * name, void * arg);
zt_bcast_node_t * zt_bcast_node_new(zt_bcast_t * bcast, const char * set_name, void * arg);
zt_bcast_t      * zt_bcast_new(zt_bcast_cfg_t * cfg, void * arg);

END_C_DECLS

#endif /* __ZT_BCAST_H__ */
