#ifndef _ZT_IPV4_TBL_H_
#define _ZT_IPV4_TBL_H_

typedef struct zt_ipv4_node zt_ipv4_node;
typedef struct zt_ipv4_tbl  zt_ipv4_tbl;
typedef struct zt_ipv4_addr zt_ipv4_addr;

struct zt_ipv4_addr {
    uint8_t      bitlen;
    uint32_t     addr;
    uint32_t     mask;
    uint32_t     broadcast;
};

struct zt_ipv4_node {
    zt_ipv4_node *next;
    zt_ipv4_addr *addr;
};

struct zt_ipv4_tbl {
    zt_mem_pool_group *pools;
    zt_ipv4_node **tbl[32];
    uint8_t        in[32];
    uint32_t       sz;
    zt_ipv4_node  *any;
};

/** 
 * Initialize a zt_ipv4_tbl hash
 * @param size the size of the hash
 * @return a zt_ipv4_tbl *
 */
zt_ipv4_tbl * zt_ipv4_tbl_init(size_t size);

/**
 * blah
 * @param netstr the network string
 * @return zt_ipv4_addr *
 */
zt_ipv4_addr * zt_ipv4_addr_frm_str(const char *netstr);

/**                                                                                           
 * Create a zt_ipv4_node from an input string
 * @param netstr a network address (can be CIDR) 
 * @return zt_ipv4_node *
 */
zt_ipv4_node * zt_ipv4_node_str_init(const char *netstr);

/**
 * Add a network to the ipv4_tbl from a string 
 * @param tbl the zt_ipv4_tbl hash table
 * @param netstr a network address (can be CIDR)
 * @return zt_ipv4_node *
 */
zt_ipv4_node *
zt_ipv4_tbl_add_frm_str(zt_ipv4_tbl *tbl, const char *netstr);

#endif
