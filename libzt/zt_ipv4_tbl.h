#ifndef _ZT_IPV4_TBL_H_
#define _ZT_IPV4_TBL_H_

#include <zt_mem.h>

typedef struct zt_ipv4_node zt_ipv4_node;
typedef struct zt_ipv4_tbl zt_ipv4_tbl;
typedef struct zt_ipv4_addr zt_ipv4_addr;

struct zt_ipv4_addr {
    uint8_t  bitlen;
    uint32_t addr;
    uint32_t mask;
    uint32_t broadcast;
};

struct zt_ipv4_node {
    zt_ipv4_node *next;
    zt_ipv4_addr *addr;
};

struct zt_ipv4_tbl {
    zt_mem_pool_group *pools;
    zt_ipv4_node     **tbl[32];
    uint8_t            in[32];
    uint32_t           sz;
    zt_ipv4_node      *any;
};

/**
 * Initialize a zt_ipv4_tbl hash
 * @param size the size of the hash
 * @return a zt_ipv4_tbl *
 */
zt_ipv4_tbl    *zt_ipv4_tbl_init(size_t size);

/**
 * Frees up all memory for a zt_ipv4_tbl hash
 * @param tbl the zt_ipv4_tbl hash
 * @return void
 */
void            zt_ipv4_tbl_destroy(zt_ipv4_tbl *tbl);

/**
 * blah
 * @param netstr the network string
 * @return zt_ipv4_addr *
 */
zt_ipv4_addr *zt_ipv4_addr_frm_str(const char *netstr);

/**
 * Create a zt_ipv4_node from an input string
 * @param netstr a network address (can be CIDR)
 * @return zt_ipv4_node *
 */
zt_ipv4_node *zt_ipv4_node_str_init(const char *netstr);

/**
 * Add a network to the ipv4_tbl from a string
 * @param tbl the zt_ipv4_tbl hash table
 * @param netstr a network address (can be CIDR)
 * @return zt_ipv4_node *
 */
zt_ipv4_node *zt_ipv4_tbl_add_frm_str(zt_ipv4_tbl *tbl,
                                      const char *netstr);

/**
 * Add a zt_ipv4_node to the zt_ipv4_tb in the correct prefix bucket
 * and table.
 * @param tbl the zt_ipv4_tbl hash table
 * @param node the zt_ipv4_node to insert
 * @return 0 on success -1 on error
 */
int zt_ipv4_tbl_add_node(zt_ipv4_tbl *tbl, zt_ipv4_node *node);

/**
 * Converts an input string into an zt_ipv4_node and adds it to
 * the hash zt_ipv4_tbl hash.
 * @param tbl the zt_ipv4_tbl hash
 * @param netstr a string representation of a network address (can be cidr)
 * @return a zt_ipv4_node on success, NULL on error
 */
zt_ipv4_node *zt_ipv4_tbl_add_frm_str(zt_ipv4_tbl *tbl,
                                      const char *netstr);

/**
 * Compares the zt_ipv4_addr needle to the zt_ipv4_addr haystack. Does
 * this by making sure needed falls WITHIN the haystack network address
 * @param haystack the zt_ipv4_addr to compare against
 * @param needle the zt_ipv4_addr to attempt to match
 * @return 1 on success 0 on failure
 */
int ipv4_tbl_addr_cmp(zt_ipv4_addr *haystack, zt_ipv4_addr *needle);

/**
 * Searches the zt_ipv4_tbl hash to determine if the zt_ipv4_node is
 * found WITHIN or EQUAL to any of the networks.
 * @param tbl the zt_ipv4_tbl hash table.
 * @param node the zt_ipv4_node to search for
 * @return the zt_ipv4_node which matched or NULL if no matches were found
 */
zt_ipv4_node *zt_ipv4_tbl_search_node(zt_ipv4_tbl *tbl,
                                      zt_ipv4_node *node);
/**
 * Converts an input string into a zt_ipv4_node and searches the
 * zt_ipv4_tbl using zt_ipv4_tbl_search_node();
 * @param tbl the zt_ipv4_tbl hash table
 * @param *netstr a string representation of a network address (can be cidr)
 * @return a zt_ipv4_node which matched, or NULL if no matches were found
 */
zt_ipv4_node *zt_ipv4_tbl_search_from_str(zt_ipv4_tbl *tbl,
                                          const char *netstr);

/* HELPERS */

/**
 * Convert a string into a bitlen and a host ordered addr
 * @param str the actual string representation of an address (can be cidr)
 * @param outaddr a pointer to where to place the host byte ordered address.
 * @param outbitlen a pointer to where to place the bit length (default /32)
 * @return int -1 on error, 0 on success.
 */
int zt_ipv4_str2net(const char *str, uint32_t *outaddr, int *outbitlen);

/**
 * Convert a string into a host ordered uint32_t
 * @param str ip address
 * @return the host ordered address
 */
uint32_t zt_ipv4_ip2int(const char *str);

/**
 * Converts a uint32_t address into a string
 * @param addr the uint32_t representation of an IP address
 * @return the string representation of the IP address
 */
const char *zt_ipv4_int2ip(uint32_t addr);

#endif /* ifndef _ZT_IPV4_TBL_H_ */
