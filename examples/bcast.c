#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <zt.h>

static int
_test_dispatcher(const char * sname, void * barg UNUSED, void * sarg UNUSED, void * narg, void * arg) {
    printf("sname = '%s', node_arg = '%s', dispatch_arg = '%s'\n", sname, (char *)narg, (char *)arg);
    return 0;
}

int
main(int argc UNUSED, char ** argv UNUSED) {
    zt_bcast_t      * bcast  = NULL;
    zt_bcast_node_t * node_a = NULL;
    zt_bcast_node_t * node_b = NULL;
    zt_bcast_node_t * node_c = NULL;
    zt_bcast_node_t * node_d = NULL;

    if (!(bcast = zt_bcast_new(NULL, NULL))) {
        return -1;
    }

    node_a = zt_bcast_node_new(bcast, "set_a", (void *)"node_a set_a");
    node_b = zt_bcast_node_new(bcast, "set_b", (void *)"node_b set_b");
    node_c = zt_bcast_node_new(bcast, "set_b", (void *)"node_c set_b");
    node_d = zt_bcast_node_new(bcast, "set_b", (void *)"node_d set_b");

    zt_bcast_dispatch(bcast, "set_b", _test_dispatcher, (void *)"set_b foo");
    zt_bcast_dispatch(bcast, "set_a", _test_dispatcher, (void *)"set_a bar");

    return 0;
}

