/*
 * sha1_test.c        test assertions
 *
 * Copyright (C) 2008, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 */

/*
 * Description:
 */
#include <math.h>

#include <libzt/zt.h>
#include <libzt/zt_unit.h>
#include <libzt/zt_uuid.h>
#include <libzt/zt_time.h>

/* void * */
/* time_uuid4(void * data) { */
    /* zt_uuid_t     uuid; */
    /* zt_uuid4(&uuid); */
    /* return 0; */
/* } */

static void
uuid4_tests(struct zt_unit_test *test, void *data)
{
    zt_uuid_t             uuid;
    int                   i;
    int                   fail_ver;
    int                   fail_variant;

    memset(&uuid, 0, sizeof(zt_uuid_t));

    fail_ver = 0;
    fail_variant = 0;
    for(i=0; i < 100; i++) {
        uint8_t   y;

        zt_uuid4(&uuid);

        if(((uuid.data.field.time_hi_and_version & 0x0F) >> 4) == UUID_VER_PSEUDORANDOM) {
            fail_ver = 1;
        }

        y = (uuid.data.field.clock_seq_hi_and_reserved >> 4);
        if(y < 8 || y > 0x0b) {
            fail_variant = 1;
        }
    }

    ZT_UNIT_ASSERT(test, fail_ver == 0);
    ZT_UNIT_ASSERT(test, fail_variant == 0);
}

static void
uuid5_tests(struct zt_unit_test *test, void *data)
{
    /* get rid of the log message for the moment */
    char    * tdata[] = { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                          "abc",
                          "http://www.google.com" };

    char    * rdata[] = { "028eea4f-70e7-5c44-a3d2-fc85d9847fb8",
                          "7697a46f-b283-5da3-8e7c-62c11c03dd9e",
                          "df65da1c-a04a-5bab-841a-e87753378510" };

    zt_uuid_ns  namespaces[] = { UUID_NS_DNS,
                                 UUID_NS_URL,
                                 UUID_NS_OID,
                                 UUID_NS_X500 };

    zt_uuid_t uuid;
    char    * uuid_s;
    int       i;
    int       x;
    int       fail_ver = 0;
    int       fail_variant = 0;

    for (i = 0; i < sizeof_array(tdata); i++) {
        zt_uuid5(tdata[i], strlen(tdata[i]), UUID_NS_OID, &uuid);
        zt_uuid_tostr(&uuid, &uuid_s);
        ZT_UNIT_ASSERT(test, memcmp(uuid_s, rdata[i], 36) == 0);
    }

    fail_ver = 0;
    fail_variant = 0;
    for (x=0; x < sizeof_array(namespaces); x++) {
        for (i = 0; i < 100; i++) {
            uint8_t   y;

            zt_uuid5(tdata[2], strlen(tdata[2]), namespaces[x], &uuid);
            if((uuid.data.field.time_hi_and_version & 0xF0) >> 4 != UUID_VER_NAMESPACE_SHA1) {
                fail_ver = 1;
            }

            y = (uuid.data.field.clock_seq_hi_and_reserved >> 4);
            if(y < 8 || y > 0xb) {
                fail_variant = 1;
            }
        }
    }

    ZT_UNIT_ASSERT(test, fail_ver == 0);
    ZT_UNIT_ASSERT(test, fail_variant == 0);
}

int
register_uuid_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "uuid tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "uuid4", uuid4_tests);
    zt_unit_register_test(suite, "uuid5", uuid5_tests);
    return(0);
}
