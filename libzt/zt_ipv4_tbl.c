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

    if (!(tbl = calloc(sizeof(zt_ipv4_tbl), 1))) {
        return NULL;
    }

    if (!(tbl->pools = zt_mem_pool_group_init(pools, 3))) {
        return NULL;
    }

    tbl->sz = rsize;
    return tbl;
}


zt_ipv4_addr   *
zt_ipv4_addr_frm_str(const char *netstr)
{
    zt_ipv4_addr   *addr = NULL;
    zt_mem_pool    *pool = NULL;
    int             bitlen = 32;
    uint32_t        ipaddr;

    if (!netstr) {
        return NULL;
    }

    if (!(pool = zt_mem_pool_get("zt_ipv4_addr"))) {
        return NULL;
    }

    if (zt_ipv4_str2net(netstr, &ipaddr, &bitlen)) {
        return NULL;
    }

    if (!(addr = zt_mem_pool_alloc(pool))) {
        return NULL;
    }

    addr->addr = ipaddr;
    addr->mask = netmask_tbl[bitlen];
    addr->bitlen = bitlen;
    addr->addr = addr->addr & addr->mask;
    addr->broadcast = addr->addr | (0xFFFFFFFF & ~addr->mask);

    return addr;
}

zt_ipv4_node   *
zt_ipv4_node_str_init(const char *netstr)
{
    zt_ipv4_node   *node;
    zt_ipv4_addr   *addr;
    zt_mem_pool    *pool;

    if (!netstr) {
        return NULL;
    }

    if (!(pool = zt_mem_pool_get("zt_ipv4_node"))) {
        return NULL;
    }

    if (!(node = zt_mem_pool_alloc(pool))) {
        return NULL;
    }

    if (!(addr = zt_ipv4_addr_frm_str(netstr))) {
        return NULL;
    }

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

    if (!tbl || !node) {
        return -1;
    }

    if (node->addr->bitlen == 0) {
        /*
         * this is a 0.0.0.0/0 network, set the any to this node 
         */
        tbl->any = node;
        return 0;
    }

    if (!(pool = zt_mem_pool_get("zt_ipv4_node_hash"))) {
        return -1;
    }

    is_in = 0;
    blen = node->addr->bitlen - 1;

    if (tbl->tbl[blen] == NULL) {
        zt_ipv4_node  **hash_entry;

        if (!(hash_entry = zt_mem_pool_alloc(pool))) {
            return -1;
	}

        memset(hash_entry, 0, sizeof(zt_ipv4_node *) * tbl->sz);

        tbl->tbl[blen] = hash_entry;
    }

    key = node->addr->addr & (tbl->sz - 1);
    node_slot = tbl->tbl[blen][key];

    if (node_slot != NULL) {
        node->next = node_slot;
    }

    tbl->tbl[blen][key] = node;

    /*
     * set our in array to this bitlen if it doesn't exist 
     * we use this to iterate through the prefixes. 
     * 
     * For example if the bitlen is /32 and we add one node 
     * tbl->in[0] would be 32.
     * 
     * When we search for an address within our table the 
     * only work we have to do is checking bitlengths found
     * within this in array 
     */

    for (i = 0; i <= 32; i++) {
        if (tbl->in[i] == 0) {
            break;
	}

        if (tbl->in[i] == node->addr->bitlen) {
            is_in = 1;
            break;
        }
    }

    /*
     * the bitlen doesn't exist, so add it to the last 
     * open slot 
     */
    if (is_in == 0) {
        tbl->in[i] = node->addr->bitlen;
    }

    return 0;
}

zt_ipv4_node   *
zt_ipv4_tbl_add_frm_str(zt_ipv4_tbl * tbl, const char *netstr)
{
    zt_ipv4_node   *ipv4_node = NULL;

    if (!tbl || !netstr) {
        return NULL;
    }

    if (tbl->any) {
        /*
         * we had a 0.0.0.0/0, we don't need to insert anything more 
         */
        return tbl->any;
    }

    if (!(ipv4_node = zt_ipv4_node_str_init(netstr))) {
        return NULL;
    }

    if (zt_ipv4_tbl_add_node(tbl, ipv4_node)) {
        return NULL;
    }

    return ipv4_node;
}

int
ipv4_tbl_addr_cmp(zt_ipv4_addr * haystack, zt_ipv4_addr * needle)
{
    if (!haystack || !needle) {
        return 0;
    }

    if (needle->addr >= haystack->addr &&
        needle->broadcast <= haystack->broadcast) {
        return 1;
    }

    return 0;
}

zt_ipv4_node   *
zt_ipv4_tbl_search_node(zt_ipv4_tbl * tbl, zt_ipv4_node * node)
{
    int             bit_iter;
    int             i = 0;
    zt_ipv4_node   *match = NULL;

    if (tbl->any) {
        return tbl->any;
    }

    while (1) {
        uint32_t        addr_masked;
        uint32_t        key;

        bit_iter = tbl->in[i++];

        if (!bit_iter) {
            break;
	}

        addr_masked = node->addr->addr & netmask_tbl[bit_iter];
        key = addr_masked & (tbl->sz - 1);

        match = tbl->tbl[bit_iter - 1][key];

        while (match) {
            if (ipv4_tbl_addr_cmp(match->addr, node->addr)) {
                return match;
	    }

            match = match->next;
        }
    }

    return NULL;
}

zt_ipv4_node   *
zt_ipv4_tbl_search_from_str(zt_ipv4_tbl * tbl, const char *netstr)
{
    zt_ipv4_node   *ipv4_node = NULL;
    zt_ipv4_node   *matched = NULL;

    if (!tbl || !netstr) {
        return NULL;
    }

    if (!(ipv4_node = zt_ipv4_node_str_init(netstr))) {
        return NULL;
    }

    matched = zt_ipv4_tbl_search_node(tbl, ipv4_node);

    zt_mem_pool_release((void **) &ipv4_node);

    return matched;
}

/*
 * Helper functions 
 */
#define MAXLINE 16
uint32_t
zt_ipv4_ip2int(const char *str)
{
    return ntohl(inet_addr(str));
}

const char     *
zt_ipv4_int2ip(uint32_t addr)
{
    uint32_t        a;

    a = ntohl(addr);

    return inet_ntoa(*(struct in_addr *) &a);
}

int
zt_ipv4_str2net(const char *str, uint32_t * outaddr, int *outbitlen)
{
    char           *save;
    char           *str_cp;
    char           *cp;
    int             bitlen = 32;

    if (!str || !outaddr || !outbitlen) {
        return -1;
    }

    if (!(str_cp = strdup(str))) {
        return -1;
    }

    if (!(save = calloc(MAXLINE, 1))) {
        free(str_cp);
        return -1;
    }

    if ((cp = strchr(str_cp, '/')) != NULL) {
        bitlen = atoi(cp + 1);

        if ((cp - str_cp) > MAXLINE - 1) {
            free(save);
            free(str_cp);
            return -1;
        }

        memcpy(save, str, cp - str_cp);
    } else {
        bitlen = 32;
        memcpy(save, str, MAXLINE);
    }

    if (bitlen > 32) {
        bitlen = 32;
    }

    *outaddr = ntohl(inet_addr(save));
    *outbitlen = bitlen;

    free(save);
    free(str_cp);

    return 0;
}
