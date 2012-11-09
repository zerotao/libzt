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
#ifdef HAVE_CONFIG_H
# include "zt_config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#include <math.h>

#define ZT_WITH_UNIT
#include <zt.h>

/*
 * void *
 * time_uuid4(void * data) {
 * zt_uuid_t     uuid;
 * zt_uuid4(&uuid);
 * return 0;
 * }
 */

static void
uuid4_tests(struct zt_unit_test * test, void * data UNUSED) {
    zt_uuid_t    uuid;
    int          i;
    int          fail_ver;
    int volatile fail_variant;

    memset(&uuid, 0, sizeof(zt_uuid_t));

    fail_ver     = 0;
    fail_variant = 0;
    for (i = 0; i < 100; i++) {
        uint8_t y;

        zt_uuid4(&uuid);

        if (((uuid.data.field.time_hi_and_version & 0x0F) >> 4) == UUID_VER_PSEUDORANDOM) {
            fail_ver = 1;
        }

        y = (uuid.data.field.clock_seq_hi_and_reserved >> 4);
        if (y < 8 || y > 0x0b) {
            fail_variant = 1;
        }
    }

    ZT_UNIT_ASSERT(test, fail_ver == 0);
    ZT_UNIT_ASSERT(test, fail_variant == 0);
}

static void
uuid5_tests(struct zt_unit_test * test, void * data UNUSED) {
    /* get rid of the log message for the moment */
    char       * tdata[]      = { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                                  "abc",
                                  "http://www.google.com" };

    char       * rdata[]      = { "028eea4f-70e7-5c44-a3d2-fc85d9847fb8",
                                  "7697a46f-b283-5da3-8e7c-62c11c03dd9e",
                                  "df65da1c-a04a-5bab-841a-e87753378510" };

    char       * rdatas[]     = { "028eea4f70e75c44a3d2fc85d9847fb8",
                                  "7697a46fb2835da38e7c62c11c03dd9e",
                                  "df65da1ca04a5bab841ae87753378510" };

    zt_uuid_ns   namespaces[] = { UUID_NS_DNS,
                                  UUID_NS_URL,
                                  UUID_NS_OID,
                                  UUID_NS_X500 };

    zt_uuid_t    uuid;
    char       * uuid_s;
    int          i;
    int          x;
    int          fail_ver     = 0;
    int volatile fail_variant = 0;
    ssize_t      s = 0;

    for (i = 0; i < (int)sizeof_array(tdata); i++) {
        zt_uuid5(tdata[i], strlen(tdata[i]), UUID_NS_OID, &uuid);

        s = zt_uuid_tostr(&uuid, &uuid_s, zt_uuid_std_fmt);
        ZT_UNIT_ASSERT(test, memcmp(uuid_s, rdata[i], 36) == 0);
        zt_free(uuid_s);

        s = zt_uuid_tostr(&uuid, &uuid_s, zt_uuid_short_fmt);
        ZT_UNIT_ASSERT(test, memcmp(uuid_s, rdatas[i], 32) == 0);
        zt_free(uuid_s);
    }

    fail_ver     = 0;
    fail_variant = 0;
    for (x = 0; x < (int)sizeof_array(namespaces); x++) {
        for (i = 0; i < 100; i++) {
            uint8_t y;

            zt_uuid5(tdata[2], strlen(tdata[2]), namespaces[x], &uuid);
            if ((uuid.data.field.time_hi_and_version & 0xF0) >> 4 != UUID_VER_NAMESPACE_SHA1) {
                fail_ver = 1;
            }

            y = (uuid.data.field.clock_seq_hi_and_reserved >> 4);
            if (y < 8 || y > 0xb) {
                fail_variant = 1;
            }
        }
    }

    ZT_UNIT_ASSERT(test, fail_ver == 0);
    ZT_UNIT_ASSERT(test, fail_variant == 0);
} /* uuid5_tests */

static void
uuid_generic_tests(struct zt_unit_test * test, void * data UNUSED) {
    char    * uuids1;
    char    * uuids2;
    zt_uuid_t uuid1;
    zt_uuid_t uuid2;

    zt_uuid4(&uuid1);

    zt_uuid_tostr(&uuid1, &uuids1, zt_uuid_std_fmt);
    zt_uuid_fromstr(uuids1, &uuid2, zt_uuid_std_fmt);
    zt_uuid_tostr(&uuid2, &uuids2, zt_uuid_std_fmt);

    ZT_UNIT_ASSERT(test, zt_uuid_cmp(&uuid1, &uuid2) == 0);
    ZT_UNIT_ASSERT(test, strcmp(uuids1, uuids2) == 0);
    ZT_UNIT_ASSERT(test, zt_uuid_isvalid(uuids1, zt_uuid_std_fmt) == 0);
    ZT_UNIT_ASSERT(test, zt_uuid_isvalid(uuids2, zt_uuid_std_fmt) == 0);

    zt_free(uuids1);
    zt_free(uuids2);

    uuids1 = 0;
    uuids2 = 0;

    zt_uuid_tostr(&uuid1, &uuids1, zt_uuid_short_fmt);
    ZT_UNIT_ASSERT(test, uuids1 != NULL);

    zt_uuid_fromstr(uuids1, &uuid2, zt_uuid_short_fmt);

    zt_uuid_tostr(&uuid2, &uuids2, zt_uuid_short_fmt);
    ZT_UNIT_ASSERT(test, uuids2 != NULL);

    ZT_UNIT_ASSERT(test, zt_uuid_isvalid(uuids1, zt_uuid_short_fmt) == 0);
    ZT_UNIT_ASSERT(test, zt_uuid_isvalid(uuids2, zt_uuid_short_fmt) == 0);

    ZT_UNIT_ASSERT(test, zt_uuid_cmp(&uuid1, &uuid2) == 0);
    ZT_UNIT_ASSERT(test, strcmp(uuids1, uuids2) == 0);

    zt_free(uuids1);
    zt_free(uuids2);

    uuids1 = 0;
    uuids2 = 0;

    zt_uuid4(&uuid1);

    ZT_UNIT_ASSERT(test, zt_uuid_tostr(&uuid1, &uuids1, zt_uuid_base62_fmt) == UUID_BASE62_STR_LEN);
    ZT_UNIT_ASSERT(test, zt_uuid_fromstr(uuids1, &uuid2, zt_uuid_base62_fmt) == 0);
    ZT_UNIT_ASSERT(test, zt_uuid_tostr(&uuid2, &uuids2, zt_uuid_base62_fmt) == UUID_BASE62_STR_LEN);

    ZT_UNIT_ASSERT(test, zt_uuid_isvalid(uuids1, zt_uuid_base62_fmt) == 0);
    ZT_UNIT_ASSERT(test, zt_uuid_isvalid(uuids2, zt_uuid_base62_fmt) == 0);
    ZT_UNIT_ASSERT(test, zt_uuid_cmp(&uuid1, &uuid2) == 0);
    ZT_UNIT_ASSERT(test, strcmp(uuids1, uuids2) == 0);

    zt_free(uuids1);
    zt_free(uuids2);

    uuids1 = 0;
    uuids2 = 0;

    {
        int    i;
        char * cmp[] = {
            "611e34722c264145a936a854984ea10c", "8kXjP4L7XohewIBQdYqnog",
            "e8a29b34d94a43288a380d795da3a277", "jYjjP0AIK5qbRJEaJQtHvh",
            "e40c0b36c5af463e8286a94686634e03", "jzT7BxJwiLAbcMN0MZJKi7",
            "52a317170ad841448835a8351f276c75", "75SgelhCYU4bH2vEIrFZxr",
            "d026bd1056a74e60a0c8a63249d34e70", "hRZ2OT6eWoodNQAWnbBswo",
            "6bff38471eb54d1597d30513f358aa12", "9gRDZQIhxFHd29K97UR0EG",
            "a99a8f28e4fb466a868d3346ee670e32", "eyNoLsM1IHobydjKIgtGZI",
            "d2b65062269f4971a2474c1833d58454", "i5CbxnEth73dVNTae6gRyA",
            "52c58a378b7d446c8b2326613a68266c", "76AFIL4rBwgbWCIZbkTcMQ",
            "64c87715d65b435a970bc96348d5824b", "8EsSVjVjAF4cY0TSeKY0RR",
            "9b0b8a189a91422a8e38d14eedaea12f", "djiGctnwwIOcd2Jp0FlKV1",
            "a469246fd76d49048cd60d257490e17f", "e79zn6dR1m4c5FnI171pOT",
            "2d15d864ad9e4457bd6330709814112c", "3RZgwGf2YZ1gg6vB5xN4qE",
            "cb53926dd43646038c6dbb3e74ee2116", "hsiIErt7Ugbc3uTE0IpQ3k",
            "b8323d6dd2fa4e04a25630488be98d4f", "fOthA3kVltidW75oKalObB",
            "f899b876c49d4c60bebe12244a5f432e", "llifXgqjtUkgnjH5HCCkOO",
            "501b714d8ae2480585c7691ab4e3e862", "6SplayVXuFDbu6kQLkept8",
            "603e6c5f1cd3427ebdb86b2efb49f677", "8giPs68iH0yghSnNPv7YyP",
            "b664a528cbf14c7da8f897275acec917", "fESdjyjObnvevqAXbGlVk3",
            "a25fb601f5ce454c8e5eab17cf748e66", "dWjmv2mdnPucdPwJcp9QHA",
            "a26d5a248bc24b07a8899f126dc1ec11", "dWAWO6Kw9tZet7xrd9PIEp",
            "5ff9110bf4f64a51a1af0e28182c4f78", "8eRpV41x8VrdSDCNutXOKk",
            "c6f1495644064f70a315dd47bf8b024d", "h4XUOIUJFQse06bCLYYzrT",
            "08a4bb5061d44f6b89eb457b58bf2c1e", "0K0yOOBu62TbQ8FduZp1xk",
            "ebb628718eb24f158da315014bf59450", "keGB72glaiVc9VHmJgZYg8",
            "ab85e2130a5c412f863f8b4861ea873c", "eJ0NdappLOfbwBcRlHKhJO",
            "d54dbe2c2f384370bcb8515478ad742e", "ijprXzOkIP6gcyed18zVEi",
            "2407c93c8a174d6b87220315c6742361", "35MV6LPhu0HbBj9X22Hqpz",
            "16dad873afab4227b336107375d3ea7e", "1XEFk7dvD8zfnW5DJqGqxE",
            "a3a2fd7814e64e1b8dff39776a94474f", "e327uRNbLKzcbQu6OxwHq7",
            "58ec5d0b3015473fa9204a1c6190195c", "7DkRVTroOQfewfM0OYnGEY",
            "92e9062b320b4017b94f467a7da02f1c", "cBZZi1TvhPxfUoJPNCTqJ6",
            "c1bd3f2dc6f24b7b8995c46c6c432241", "gDgHN2ribrBbOmr3DPPjFf",
            "d04edd2a0fd54f358d2daa7da69c9b57", "hSOLXgxKmclc7ukytvlQur",
            "3e0d732589e64b519e4a100662143c62", "5kiRmzLuX3bdAzm6B06hqy",
            "14fef83d656c431ba8895f432ad8d131", "1NLbBDN4cNZet7dw117ew1",
            "1518b6425bbf4f369fabbc30b8bab33b", "1Oin845aYOGdHViny5kIdR",
            "70a58e512dab4627a24ffb0ac891ec5c", "9FCaXSIDPZddVZ5cYzIscQ",
            "5dc00d674b6f4527a9220639efa91412", "831RQVhoE7Bewi0FNjcUOC",
            "7e7ad53ee3714c33ad4a442e3f38156c", "aRfrweQc5PBeSpZdtxVYcI",
            "a964a82e36e0481bab7b372d7ab38559", "exFUtsNId1VeIN2zMyQmMx",
            "45b5585078a9412aa050213184c2cb75", "5Z3tvNARA3EdLle4MSOfe5",
            "0290dd7c3e9a4137a6d60758168ed63a", "0dENNGjwBAXek3ZsQjSQJ4",
            "a30645520f60471b8066a86cb81efb14", "dZM5gkZJ1Uvb1tu0P3Ss28",
            "6a1fd751df11451db1d9ae50dac46523", "96TEiBa0Mk5fgGY7rAkI1R",
            "0cbd5b4593284a5ba35652006a7d692c", "15Op7cEbZere1rhrNsscck",
            "df97ef02cc78483999277e3e5d12c541", "jcbl5jM7VBTd9eFr1Sts53",
            "afeaa46cc671426c9c4ada2d594f4d1b", "f6oMLcmRV6sdpWkb3DNoyT",
            "fc51cb0747c6415b9302d37442072458", "lF5D2fmfDMncCxfzXWw7UI",
            "bf6ff305f352442586c9ef4dc1ffd002", "gr10wK2YtBXbztC7VNXMqu",
            "31ed255daca04725978da73dd4f36a2f", "4hKV7V8zO7zd0IjPtv5meb",
            "bb005f4117f44f2a8d12664426203613", "g3p4jDLUDuWc6VbcE5UbR1",
            "f705b547feeb441580e59b3603c3100d", "lcTq7DbmakRb478IGNHPbv",
            "9114bf70a33b4e36ad407a3970acae73", "csgiZhJyKLYeSdmOnBZiSf",
            "70b44670c76748778dbe73351f9feb5c", "9FV9roYF035cauYPqoKPoM",
            "8dd9ba6469094e63b8ee38788a2b866c", "cb496ypVgQ3fSnCsJWk6gk",
            "b0cf5f3e4bf14b6d8c32aa44703f5344", "fb9EDcgUjeZc2gKBqaU1GQ",
            "3e44001392fc4912b13f24476f312670", "5lrbsY2DyvwfdtK8zAWr3W",
            "3e9d913809cd4b048425911ffa9df079", "5niEmgZzRgoblpFw5GBhZn",
            "20c11b2fd1374763a0be260d17c7d076", "2OlJfObCyundND3Ey9Kage",
            "cf230c7920a342439b8ae1036038cb69", "hMAgnBxtcAjdlWQaCG458B",
            "c4deb07a88ca4b3d91e4795d3493f76a", "gTVTlCerL1XcwA65WXRm4i",
            "4f3254359ea34d129332e347dd0b0f1a", "6NyOU6xA0g2cDxd3wjmuAG",
            "08ad3b76cc204c40a7379506b87c9b34", "0KbwfGh8b3aem5KHmFmit6",
            "1712282d336a4f4ba8bcee7856562840", "1YNYffFB4VBeubGxVv24lq",
            "c566d95d5afb424085874e3003046b16", "gWLqirsmqUobsLH48gPt78",
            "63c8de4409ad4f4fbda15b108389fa73", "8z9nChPE0HlghoEqJCM1fd",
            "dae4d6331d60421d9b50cb6aa908e32c", "iNamaGzMj7LdkJXC80VRFO",
            "3e034561b6da4c6e8a41ae1602e2f55b", "5k5JNqQOsjQbRW3FvEBmUX",
            "3ea5082cdb254874b675ed468ed75c61", "5nsgZE6RTQ8fFeuBsXvnPP",
            "79c9150789a7400eabe36b7b8176517d", "asgcc6XMYQmeKXnoQYZB8x",
            "55c8ec4ed21a4102baf3ec316dda147c", "7mD6HbuG6ZQg391g2Lyug4",
            "0585404d62af4b2a83303d3ccaeb192b", "0tnU5i1Gj4Cbgjp0EVMJVp",
            "bcaaee6a88b84b6c8def84411f73cd2f", "gcgYgt7EfQ8cbweC7QZyYD",
            "92656b3ccb914051a3fcc7236c4a4270", "czgknc8jUGte4TS2onbi2k",
            "e8f1626f3e4d430e9653251d26f5a750", "jZWSovpi5EGcUaRRJFBEK4",
            "f427407d20944333a8d79d2c32cd4829", "kXCADBJ9VYveuK5dnCJIvv",
            "fbb9fb275e4c4b7380a4a50a7483112f", "lBVV23WOzdhb2LotRgbqDd",
            "e8f35b022b884106b6f9622c3dbc4851", "jZZpVNY6fLUfHXXE3xrlDP",
            "fda2120830ed4f5eaa965d4d02285355", "lM58X2i0luCeE210zqasXH",
            "929c2b096dc74a098c136d004f471a74", "cAoUmii7c2Rc1CtIbXd588",
            "f67f26769a034241aebae72388e59132", "la5XcHMKNnXf05dx4v4706",
            "6595d21311b74f47b42fd4224d873503", "8IJCxepPJFJft84LlykZXB",
            "4f0443560a83493fb47cdd53432c8353", "6MBqXkX1lcrfuJo4G1FUgb",
            "2a17cd739c534b00b6f9cb7c25d1c81b", "3C3IoS5NrvafHYuwDBeXOj",
            "fb9f0674369e41079a54da4ae3936276", "lBnavDvdFVJdfvarPUt7dY",
            "6226830d904e4d77a050ab475fc99515", "8qs3jgkccK3dLlVbMhaQpT",
            "c03b8d55cbe64815a2f1e86a52d8b85e", "gvfu3neQpDvdZlPHqwIoqi",
            "38aea31e2e05466ba11fd3522e2eca14", "4RIvAz6a8S7dPEYKx0Cue8",
            "c808482d10da4a76b6135c472d9e1a41", "haLAjFMkeCafDbqpf1kE7v",
            "21916a3eab43406abb2550447195ad14", "2SGgVkU7nkKg4aGXWTYaR6",
            "b5c6292aefa14d18b4c13068e0ddf61d", "fBzU0sH8jwsfw9t6FLbSAl",
            "8bf5a8182abb4c6485afbf398695af0c", "c106klsiL8EbtBPtfbiL1i",
            "61596e6c9f034a04a9291203c37ff179", "8mbFFm88hz6ewr5QF6o1XH",
            "2f52d77b8a794d4aa249870fef8d6451", "43TWSLHhG7EdVQLqKe07cJ",
            "5560c65f843a407a82661d308d0e6a7e", "7ksQmhtdoGCbc6PAhuoRFY",
            "b33fc665e3854002bc3c34137b480e13", "fo8BNuThTpMg9Ye1ETbHW3",
            "f35fab797250405aa9e6285415f11538", "kTtitCATBYSeAmRaN3uHa0",
            "1d94c044240c49188686c34cd35aea6e", "2xsttFg0Cg0by51dY3Fmou",
            "13aea6303a484434b338e10c9ea34f49", "1GLCeZz5aBKfnZICJ6pOBj",
            NULL,                               NULL,
        };

        i = 0;
        while (cmp[i] != NULL) {
            zt_uuid_t uuid;
            char    * uuids = NULL;

            zt_uuid_fromstr(cmp[i], &uuid, zt_uuid_short_fmt);
            zt_uuid_tostr(&uuid, &uuids, zt_uuid_base62_fmt);
            ZT_UNIT_ASSERT(test, strcmp(uuids, cmp[i + 1]) == 0);

            zt_uuid_fromstr(cmp[i + 1], &uuid, zt_uuid_base62_fmt);
            zt_uuid_tostr(&uuid, &uuids, zt_uuid_short_fmt);
            ZT_UNIT_ASSERT(test, strcmp(uuids, cmp[i]) == 0);

            i += 2;
        }
    }
} /* uuid_generic_tests */

int
register_uuid_suite(struct zt_unit * unit) {
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "uuid tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "uuid4", uuid4_tests);
    zt_unit_register_test(suite, "uuid5", uuid5_tests);
    zt_unit_register_test(suite, "uuid_generic", uuid_generic_tests);
    return 0;
}
