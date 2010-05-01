#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include "zt.h"
#include "zt_mem.h"
#include "zt_ipv4_tbl.h"

#define DEFAULT_ELT_NUM 32

static uint32_t netmask_tbl[] = {
    0x00000000, 0x80000000, 0xC0000000, 0xE0000000, 0xF0000000,
    0xF8000000, 0xFC000000, 0xFE000000, 0xFF000000, 0xFF800000,
    0xFFC00000, 0xFFE00000, 0xFFF00000, 0xFFF80000, 0xFFFC0000,
    0xFFFE0000, 0xFFFF0000, 0xFFFF8000, 0xFFFFC000, 0xFFFFE000,
    0xFFFFF000, 0xFFFFF800, 0xFFFFFC00, 0xFFFFFE00, 0xFFFFFF00,
    0xFFFFFF80, 0xFFFFFFC0, 0xFFFFFFE0, 0xFFFFFFF0, 0xFFFFFFF8,
    0xFFFFFFFC, 0xFFFFFFFE, 0xFFFFFFFF
};

#if 0
typedef struct zt_ipv4_node zt_ipv4_node;
typedef struct zt_ipv4_tbl zt_ipv4_tbl;
typedef struct zt_ipv4_addr zt_ipv4_addr;

struct zt_ipv4_addr {
    uint8_t         bitlen;
    uint32_t        addr;
    uint32_t        mask;
    uint32_t        broadcast;
};

struct zt_ipv4_node {
    zt_ipv4_node   *next;
    zt_ipv4_addr   *addr;
};

struct zt_ipv4_tbl {
    zt_mem_pool_group *pools;
    zt_ipv4_node  **tbl[32];
    uint8_t         in[32];
    uint32_t        sz;
    zt_ipv4_node   *any;
};
#endif

zt_ipv4_tbl    *
zt_ipv4_tbl_init(size_t size)
{
    zt_ipv4_tbl    *tbl;
    size_t          rsize = size ? size : 128;

    static zt_mem_pool_desc pools[] = {
        /*
         * allocate all the buckets for each prefix in the tbl 
         */
        {"zt_ipv4_node_hash", 32, sizeof(zt_ipv4_node *), NULL, NULL, 0}
        ,
        {"zt_ipv4_node", 1, sizeof(zt_ipv4_node), NULL, NULL, 0}
        ,
        {"zt_ipv4_addr", 1, sizeof(zt_ipv4_addr), NULL, NULL, 0}
        ,
        {NULL, 0, 0, NULL, NULL, 0}
    };

    if (!(tbl = calloc(sizeof(zt_ipv4_tbl), 1)))
        return NULL;

    if (!(tbl->pools = zt_mem_pool_group_init(pools, 3)))
        return NULL;

    tbl->sz = rsize;
    return tbl;
}

#define MAXLINE 16

zt_ipv4_addr   *
zt_ipv4_addr_frm_str(const char *netstr)
{
    zt_ipv4_addr   *addr = NULL;
    zt_mem_pool    *pool = NULL;
    int             bitlen = 32;
    char           *cp = NULL;
    char           *save = NULL;

    if (!netstr)
        return NULL;

    if (!(pool = zt_mem_pool_get("zt_ipv4_addr")))
        return NULL;

    if (!(save = calloc(MAXLINE, 1)))
        return NULL;

    if (!(addr = zt_mem_pool_alloc(pool))) {
        free(save);
        return NULL;
    }

    if ((cp = strchr(netstr, '/'))) {
        bitlen = atoi(cp + 1);
        memcpy(save, netstr, cp - netstr);
        save[cp - netstr] = '\0';
    }

    if (save[0] == '\0') {
        free(save);
        return NULL;
    }

    if (bitlen > 32)
        bitlen = 32;

    addr->addr = ntohl(inet_addr(save));
    addr->mask = netmask_tbl[bitlen];
    addr->bitlen = bitlen;
    addr->addr = addr->mask;
    addr->broadcast = addr->addr | (0xFFFFFFFF & ~addr->mask);

    return addr;
}

zt_ipv4_node   *
zt_ipv4_node_str_init(const char *netstr)
{
    zt_ipv4_node   *node;
    zt_ipv4_addr   *addr;
    zt_mem_pool    *pool;

    if (!netstr)
        return NULL;

    if (!(pool = zt_mem_pool_get("zt_ipv4_node")))
        return NULL;

    if (!(node = zt_mem_pool_alloc(pool)))
        return NULL;

    if (!(addr = zt_ipv4_addr_frm_str(netstr)))
        return NULL;

    node->addr = addr;
    node->next = NULL;

    return node;
}

zt_ipv4_node   *
zt_ipv4_tbl_add_frm_str(zt_ipv4_tbl * tbl, const char *netstr)
{
    zt_ipv4_node   *ipv4_node = NULL;

    if (!tbl || !netstr)
        return NULL;

    if (!(ipv4_node = zt_ipv4_node_str_init(netstr)))
        return NULL;

    return ipv4_node;
}

int
main(int argc, char **argv)
{
    int             count = 0;
    const char     *tnet;
    const char     *test_nets[] = {
        "192.168.0.0/24",
        "192.168.1.0/24",
        "172.16.0.0/16",
        "192.168.2.2/31",
        NULL
    };

    zt_ipv4_tbl    *nettbl;

    if (!(nettbl = zt_ipv4_tbl_init(1024)))
        return -1;

    while ((tnet = test_nets[count++])) {
        zt_ipv4_node   *node;

        if (!(node = zt_ipv4_tbl_add_frm_str(nettbl, tnet))) {
            fprintf(stderr, "couldn't add stuff to tbl\n");
            return -1;
        }
    }

    zt_mem_pool_group_display(0, nettbl->pools, DISPLAY_POOL_ALL);

    return 0;
}
