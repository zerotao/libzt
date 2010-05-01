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

zt_ipv4_tbl    *
zt_ipv4_tbl_init(size_t size)
{
    zt_ipv4_tbl    *tbl;
    size_t          rsize = size ? size : 128;
    static zt_mem_pool_desc pools[4];

    memset(pools, 0, sizeof(pools));

    pools[0].name = "zt_ipv4_node_hash";
    pools[0].elts = 1;
    pools[0].size = sizeof(zt_ipv4_node *) * rsize;

    pools[1].name = "zt_ipv4_node";
    pools[1].elts = 1;
    pools[1].size = sizeof(zt_ipv4_node);

    pools[2].name = "zt_ipv4_addr";
    pools[2].elts = 1;
    pools[2].size = sizeof(zt_ipv4_addr);

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

int
zt_ipv4_tbl_add_node(zt_ipv4_tbl * tbl, zt_ipv4_node * node)
{
    zt_ipv4_node   *node_slot;
    zt_mem_pool    *pool;
    uint32_t        key;
    uint8_t         blen;
    int             is_in;
    int             i;

    if (!tbl || !node)
        return -1;

    if (node->addr->bitlen == 0) {
        /*
         * this is a 0.0.0.0/0 network, set the any to this node 
         */
        tbl->any = node;
        return 0;
    }

    if (!(pool = zt_mem_pool_get("zt_ipv4_node_hash")))
        return -1;

    is_in = 0;
    blen = node->addr->bitlen - 1;

    if (tbl->tbl[blen] == NULL) {
        zt_ipv4_node  **hash_entry;

        if (!(hash_entry = zt_mem_pool_alloc(pool)))
            return -1;

        tbl->tbl[blen] = hash_entry;
    }

    key = node->addr->addr & (tbl->sz - 1);
    node_slot = tbl->tbl[blen][key];

    if (node_slot != NULL)
        node->next = node_slot;

    tbl->tbl[blen][key] = node;

    /* set our in array to this bitlen if it doesn't exist 
     * we use this to iterate through the prefixes. 
     *
     * For example if the bitlen is /32 and we add one node 
     *  tbl->in[0] would be 32.
     *
     * When we search for an address within our table the 
     * only work we have to do is checking bitlengths found
     * within this in array 
     */ 

    for (i = 0; i < 32; i++)
    {
	if (tbl->in[i] == 0)
	    break;

	if (tbl->in[i] == node->addr->bitlen)
	{
	    is_in = 1;
	    break;
	}
    }
    
    /* the bitlen doesn't exist, so add it to the last 
       open slot */
    if (is_in == 0)
	tbl->in[i] = node->addr->bitlen;

    return 0;
}

zt_ipv4_node   *
zt_ipv4_tbl_add_frm_str(zt_ipv4_tbl * tbl, const char *netstr)
{
    zt_ipv4_node   *ipv4_node = NULL;

    if (!tbl || !netstr)
        return NULL;

    if (tbl->any)
        /*
         * we had a 0.0.0.0/0, we don't need to insert anything more 
         */
        return tbl->any;

    if (!(ipv4_node = zt_ipv4_node_str_init(netstr)))
        return NULL;

    if (zt_ipv4_tbl_add_node(tbl, ipv4_node))
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
