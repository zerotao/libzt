#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libzt/zt_stdint.h>

#define ZT_WITH_UNIT
#include <zt.h>

static struct _test_s {
    char    *addr;
    uint8_t  bitlen;
    uint32_t start;
    uint32_t end;
} network_tests[] = {
    /* 192.168.0.0 - 192.168.0.255 */
    { "192.168.0.1/24",   24, 3232235520U,  3232235775U  },
    /* 192.168.1.0 - 192.168.1.127 */
    { "192.168.1.0/25",   25, 3232235776LU, 3232235903LU },
    /* 192.168.1.130 - 192.168.1.131 */
    { "192.168.1.130/31", 31, 3232235906LU, 3232235907LU },
    /* some /32's */
    { "192.168.2.3/32",   32, 3232236035LU, 3232236035LU },
    { "192.168.2.5",      32, 3232236037LU, 3232236037LU },
    { "192.168.2.7/32",   32, 3232236039LU, 3232236039LU },
    /* 192.168.3.32 - 192.168.3.63 */
    { "192.168.3.32/27",  27, 3232236320LU, 3232236351LU },
    { NULL,               0,  0,            0            }
};

static struct _test_good {
    char *addr;
    char *should_match;
} tests_good[] = {
    /* should match 192.168.0.0/24 */
    { "192.168.0.5/32",   "192.168.0.0/24"   },
    { "192.168.0.75/32",  "192.168.0.0/24"   },
    { "192.168.0.30/31",  "192.168.0.0/24"   },
    { "192.168.0.0/24",   "192.168.0.0/24"   },
    /* should match 192.168.1.0/25 */
    { "192.168.1.10/32",  "192.168.1.0/25"   },
    { "192.168.1.16/30",  "192.168.1.0/25"   },
    { "192.168.1.0/25",   "192.168.1.0/25"   },
    /* should match 192.168.1.130/31 */
    { "192.168.1.130/32", "192.168.1.130/31" },
    { "192.168.1.131/32", "192.168.1.130/31" },
    { "192.168.1.130/31", "192.168.1.130/31" },
    /* these /32's should match */
    { "192.168.2.3/32",   "192.168.2.3/32"   },
    { "192.168.2.5/32",   "192.168.2.5/32"   },
    { "192.168.2.7/32",   "192.168.2.7/32"   },
    /* should match 192.168.3.32/27 */
    { "192.168.3.32/32",  "192.168.3.32/27"  },
    { "192.168.3.34/30",  "192.168.3.32/27"  },
    { "192.168.3.32/27",  "192.168.3.32/27"  },
    { NULL,               NULL               }
};

char *tests_bad[] = {
    /*
     * make sure this won't match in 192.168.0.1/24
     */
    "192.168.0.0/23",
    /*
     * make sure this won't match in 192.168.1.0/25
     */
    "192.168.1.128/32",
    /*
     * make sure this won't match in 192.168.3.32/27
     */
    "192.168.3.30/31",
    /*
     * random tests
     */
    "0.0.0.0/0",
    "5.5.5.0/24",
    "9.9.9.0/32",      NULL
};

static zt_ipv4_tbl *
test_table_init(struct zt_unit_test *test, void *data UNUSED)
{
    zt_ipv4_tbl *table = NULL;
    int          i;

    table = zt_ipv4_tbl_init(1024);

    ZT_UNIT_ASSERT_EQUAL(test, table->sz, 1024);
    ZT_UNIT_ASSERT_EQUAL(test, table->any, NULL);
    ZT_UNIT_ASSERT_NOT_EQUAL(test, table->pools, NULL);

    for (i = 0; network_tests[i].addr != NULL; i++) {
        zt_ipv4_node *node = NULL;

        node = zt_ipv4_tbl_add_frm_str(table, network_tests[i].addr);

        ZT_UNIT_ASSERT_NOT_EQUAL(test, node, NULL);
        ZT_UNIT_ASSERT_NOT_EQUAL(test, node->addr, NULL);
        ZT_UNIT_ASSERT_EQUAL(test, node->next, NULL);

        ZT_UNIT_ASSERT_EQUAL(test, node->addr->addr,
                             network_tests[i].start);

        ZT_UNIT_ASSERT_EQUAL(test, node->addr->broadcast,
                             network_tests[i].end);

        ZT_UNIT_ASSERT_EQUAL(test, node->addr->bitlen,
                             network_tests[i].bitlen);
    }

    return table;
}

static void
positive_tests(struct zt_unit_test *test, void *data)
{
    zt_ipv4_tbl *table = NULL;
    int          i;

    table = test_table_init(test, data);

    for (i = 0; tests_good[i].addr != NULL; i++) {
        int           cmp;
        char          test_buf[1024];
        zt_ipv4_node *matched;

        matched = zt_ipv4_tbl_search_from_str(table, tests_good[i].addr);

        ZT_UNIT_ASSERT_NOT_EQUAL(test, matched, NULL);
        ZT_UNIT_ASSERT_NOT_EQUAL(test, matched->addr, NULL);

        snprintf(test_buf, sizeof(test_buf) - 1, "%s/%d",
                 zt_ipv4_int2ip(matched->addr->addr),
                 matched->addr->bitlen);

        cmp = strcmp(test_buf, tests_good[i].should_match);

        ZT_UNIT_ASSERT_EQUAL(test, cmp, 0);
    }


    return;
}

static void
negative_tests(struct zt_unit_test *test, void *data)
{
    zt_ipv4_tbl *table = NULL;
    int          count = 0;
    const char  *str;

    table = test_table_init(test, data);

    while ((str = tests_bad[count++])) {
        zt_ipv4_node *matched;

        if (str == NULL) {
            break;
        }

        matched = zt_ipv4_tbl_search_from_str(table, str);

        ZT_UNIT_ASSERT_EQUAL(test, matched, NULL);
    }

    return;
}

static void
helper_tests(struct zt_unit_test *test UNUSED, void *data UNUSED)
{
    return;
}

int
register_ipv4_tbl_suite(struct zt_unit *unit)
{
    struct zt_unit_suite *suite;

    suite = zt_unit_register_suite(unit, "ipv4_tbl tests",
                                   NULL, NULL, NULL);

    zt_unit_register_test(suite, "helper tests", helper_tests);
    zt_unit_register_test(suite, "positive match tests", positive_tests);
    zt_unit_register_test(suite, "negative match tests", negative_tests);

    return 0;
}
