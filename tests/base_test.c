#define ZT_WITH_UNIT
#include <zt.h>

#include <string.h>
#include <zt_base.h>

#define test_encoding(_base, _data, _len, _result1, _result2)     \
    do { unsigned char * cdata = (unsigned char *)_data;          \
         char            result[20];                              \
         int             ret;                                     \
         size_t          len;                                     \
         memset(result, 0, sizeof(result));                       \
         len = sizeof(result);                                    \
         ZT_BIT_UNSET(_base->flags, zt_base_encode_with_padding); \
         ret = zt_base_encode(_base, cdata, _len, result, &len);  \
         ZT_UNIT_ASSERT(test, !ret);                              \
         ZT_UNIT_ASSERT(test, strcmp(result, _result1) == 0);     \
         ZT_UNIT_ASSERT(test, strlen(result) == len);             \
         ZT_BIT_SET(_base->flags, zt_base_encode_with_padding);   \
                                                                  \
         memset(result, 0, sizeof(result));                       \
         len = sizeof(result);                                    \
         ret = zt_base_encode(_base, cdata, _len, result, &len);  \
         ZT_UNIT_ASSERT(test, !ret);                              \
         ZT_UNIT_ASSERT(test, strcmp(result, _result2) == 0);     \
         ZT_UNIT_ASSERT(test, strlen(result) == len); } while (0)

#define test_decoding(_base, _data, _len, _result1, _result2)                                  \
    do { char  * result = malloc(20);                                                          \
         int     ret;                                                                          \
         size_t  len;                                                                          \
         void ** rptr   = (void **)&result;                                                    \
         memset(result, 0, 20);                                                                \
         len = 20;                                                                             \
         ret = zt_base_decode(_base, (unsigned char *)_result1, strlen(_result1), rptr, &len); \
         ZT_UNIT_ASSERT(test, !ret);                                                           \
         ZT_UNIT_ASSERT(test, strcmp(result, (char *)_data) == 0);                             \
         ZT_UNIT_ASSERT(test, strlen(result) == len);                                          \
                                                                                               \
         memset(result, 0, 20);                                                                \
         len = 20;                                                                             \
         ret = zt_base_decode(_base, _result2, strlen(_result2), rptr, &len);                  \
         ZT_UNIT_ASSERT(test, !ret);                                                           \
         ZT_UNIT_ASSERT(test, strcmp(result, (char *)_data) == 0);                             \
         ZT_UNIT_ASSERT(test, strlen(result) == len); } while (0)

static void
encoding_tests(struct zt_unit_test * test, void * _data UNUSED) {
    char * data = NULL;
    char   result[20];

    #define OLEN sizeof(result)
    size_t len  = OLEN;
    int    ret;

    memset(result, 0, 20);

    /* base 64 tests */

    /* check bad param, no out_len */
    data = "wee";
    ret  = zt_base_encode(zt_base64_rfc, data, strlen(data), result, NULL);
    ZT_UNIT_ASSERT(test, ret == -1);

    /* check null buff but valid bufsize */
    ret  = zt_base_encode(zt_base64_rfc, data, strlen(data), NULL, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, len == 4);

    /* check insufficient output space */
    len  = 2;
    ret  = zt_base_encode(zt_base64_rfc, data, strlen(data), result, &len);
    ZT_UNIT_ASSERT(test, ret == -2);
    ZT_UNIT_ASSERT(test, len == 4);

    /* check missing input */
    len  = OLEN;
    ret  = zt_base_encode(zt_base64_rfc, NULL, 0, result, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, len == 0);

    /* check empty input */
    len  = OLEN;
    ret  = zt_base_encode(zt_base64_rfc, "", 0, result, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, len == 0);

    /* check one char input */
    test_encoding(zt_base64_rfc, "x", 1, "eA", "eA==");

    /* check two char input */
    test_encoding(zt_base64_rfc, "xy", 2, "eHk", "eHk=");

    /* simple non-padded input */
    test_encoding(zt_base64_rfc, "One", 3, "T25l", "T25l");

    /* simple double padded input */
    test_encoding(zt_base64_rfc, "Once", 4, "T25jZQ", "T25jZQ==");

    /* simple single padded input */
    test_encoding(zt_base64_rfc, "Ounce", 5, "T3VuY2U", "T3VuY2U=");

    /* roll over */
    test_encoding(zt_base64_rfc, "Amount", 6, "QW1vdW50", "QW1vdW50");

    /*
     * non-ascii data
     * one byte
     */
    {
        unsigned char udata[2] = { 0xF0, 0x00 };
        test_encoding(zt_base64_rfc, udata, 1, "8A", "8A==");
    }

    /* two bytes */
    {
        unsigned char udata[3] = { 0xFF, 0xFD, 0x00 };
        test_encoding(zt_base64_rfc, udata, 2, "//0", "//0=");
    }

    /* three bytes */
    {
        unsigned char udata[4] = { 0xFF, 0xFF, 0xBE, 0x00 };
        test_encoding(zt_base64_rfc, udata, 3, "//++", "//++");
    }

    /* null bytes */
    {
        unsigned char udata[3] = { 0xF0, 0x00, 0x00 };
        test_encoding(zt_base64_rfc, udata, 2, "8AA", "8AA=");
    }

    /* RFC4648 test vectors */
    test_encoding(zt_base64_rfc, "", 0, "", "");
    test_encoding(zt_base64_rfc, "f", 1, "Zg", "Zg==");
    test_encoding(zt_base64_rfc, "fo", 2, "Zm8", "Zm8=");
    test_encoding(zt_base64_rfc, "foo", 3, "Zm9v", "Zm9v");
    test_encoding(zt_base64_rfc, "foob", 4, "Zm9vYg", "Zm9vYg==");
    test_encoding(zt_base64_rfc, "fooba", 5, "Zm9vYmE", "Zm9vYmE=");
    test_encoding(zt_base64_rfc, "foobar", 6, "Zm9vYmFy", "Zm9vYmFy");


    /* base 32 tests */

    test_encoding(zt_base32_rfc, "", 0, "", "");
    test_encoding(zt_base32_rfc, "f", 1, "MY", "MY======");
    test_encoding(zt_base32_rfc, "fo", 2, "MZXQ", "MZXQ====");
    test_encoding(zt_base32_rfc, "foo", 3, "MZXW6", "MZXW6===");
    test_encoding(zt_base32_rfc, "foob", 4, "MZXW6YQ", "MZXW6YQ=");
    test_encoding(zt_base32_rfc, "fooba", 5, "MZXW6YTB", "MZXW6YTB");
    test_encoding(zt_base32_rfc, "foobar", 6, "MZXW6YTBOI", "MZXW6YTBOI======");

    /* base 32 hex tests */

    test_encoding(zt_base32_hex, "", 0, "", "");
    test_encoding(zt_base32_hex, "f", 1, "CO", "CO======");
    test_encoding(zt_base32_hex, "fo", 2, "CPNG", "CPNG====");
    test_encoding(zt_base32_hex, "foo", 3, "CPNMU", "CPNMU===");
    test_encoding(zt_base32_hex, "foob", 4, "CPNMUOG", "CPNMUOG=");
    test_encoding(zt_base32_hex, "fooba", 5, "CPNMUOJ1", "CPNMUOJ1");
    test_encoding(zt_base32_hex, "foobar", 6, "CPNMUOJ1E8", "CPNMUOJ1E8======");

    /* base 16 tests */

    test_encoding(zt_base16_rfc, "", 0, "", "");
    test_encoding(zt_base16_rfc, "f", 1, "66", "66");
    test_encoding(zt_base16_rfc, "fo", 2, "666F", "666F");
    test_encoding(zt_base16_rfc, "foo", 3, "666F6F", "666F6F");
    test_encoding(zt_base16_rfc, "foob", 4, "666F6F62", "666F6F62");
    test_encoding(zt_base16_rfc, "fooba", 5, "666F6F6261", "666F6F6261");
    test_encoding(zt_base16_rfc, "foobar", 6, "666F6F626172", "666F6F626172");
} /* encoding_tests */

static void
decoding_tests(struct zt_unit_test * test, void * _data UNUSED) {
    char * result = alloca(20);
    char * resnul = NULL;
    size_t len;
    int    ret;

    memset(result, 0, 20);

    /* bad param, no out len */
    ret    = zt_base_decode(zt_base64_rfc, "Zg", strlen("Zg"), (void **)&result, NULL);
    ZT_UNIT_ASSERT(test, ret == -1);

    /* output size calculation */
    len    = 1;
    ret    = zt_base_decode(zt_base64_rfc, "Zg", strlen("Zg"), NULL, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, len == 2);

    /* check insufficient output space */
    len    = 1;
    ret    = zt_base_decode(zt_base64_rfc, "Zm8", strlen("Zm8"), (void **)&result, &len);
    ZT_UNIT_ASSERT(test, ret == -2);
    ZT_UNIT_ASSERT(test, len == 3);

    /* check missing input */
    len    = 2;
    ret    = zt_base_decode(zt_base64_rfc, NULL, 0, (void **)&result, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, len == 0);

    /* check empty input */
    len    = 2;
    ret    = zt_base_decode(zt_base64_rfc, "", 0, (void **)&result, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, len == 0);

    /* make sure dynamic allocation works */
    len    = 0;
    resnul = NULL;
    ret    = zt_base_decode(zt_base64_rfc, "Zm9vYmFy", 8, (void **)&resnul, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, resnul != NULL);
    ZT_UNIT_ASSERT(test, len == 6);
    ZT_UNIT_ASSERT(test, strncmp("foobar", resnul, len) == 0);
    zt_free(resnul);
    resnul = NULL;

    /* RFC4648 test vectors */
    test_decoding(zt_base64_rfc, "", 0, "", "");
    test_decoding(zt_base64_rfc, "f", 1, "Zg", "Zg==");
    test_decoding(zt_base64_rfc, "fo", 2, "Zm8", "Zm8=");
    test_decoding(zt_base64_rfc, "foo", 3, "Zm9v", "Zm9v");
    test_decoding(zt_base64_rfc, "foob", 4, "Zm9vYg", "Zm9vYg==");
    test_decoding(zt_base64_rfc, "fooba", 5, "Zm9vYmE", "Zm9vYmE=");
    test_decoding(zt_base64_rfc, "foobar", 6, "Zm9vYmFy", "Zm9vYmFy");


    /* base 32 tests */

    test_decoding(zt_base32_rfc, "", 0, "", "");
    test_decoding(zt_base32_rfc, "f", 1, "MY", "MY======");
    test_decoding(zt_base32_rfc, "fo", 2, "MZXQ", "MZXQ====");
    test_decoding(zt_base32_rfc, "foo", 3, "MZXW6", "MZXW6===");
    test_decoding(zt_base32_rfc, "foob", 4, "MZXW6YQ", "MZXW6YQ=");
    test_decoding(zt_base32_rfc, "fooba", 5, "MZXW6YTB", "MZXW6YTB");
    test_decoding(zt_base32_rfc, "foobar", 6, "MZXW6YTBOI", "MZXW6YTBOI======");

    /* base 32 hex tests */

    test_decoding(zt_base32_hex, "", 0, "", "");
    test_decoding(zt_base32_hex, "f", 1, "CO", "CO======");
    test_decoding(zt_base32_hex, "fo", 2, "CPNG", "CPNG====");
    test_decoding(zt_base32_hex, "foo", 3, "CPNMU", "CPNMU===");
    test_decoding(zt_base32_hex, "foob", 4, "CPNMUOG", "CPNMUOG=");
    test_decoding(zt_base32_hex, "fooba", 5, "CPNMUOJ1", "CPNMUOJ1");
    test_decoding(zt_base32_hex, "foobar", 6, "CPNMUOJ1E8", "CPNMUOJ1E8======");

    /* base 16 tests */

    test_decoding(zt_base16_rfc, "", 0, "", "");
    test_decoding(zt_base16_rfc, "f", 1, "66", "66");
    test_decoding(zt_base16_rfc, "fo", 2, "666F", "666F");
    test_decoding(zt_base16_rfc, "foo", 3, "666F6F", "666F6F");
    test_decoding(zt_base16_rfc, "foob", 4, "666F6F62", "666F6F62");
    test_decoding(zt_base16_rfc, "fooba", 5, "666F6F6261", "666F6F6261");
    test_decoding(zt_base16_rfc, "foobar", 6, "666F6F626172", "666F6F626172");
} /* decoding_tests */

int
register_base_suite(struct zt_unit * unit) {
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "baseN encoding tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "encoding", encoding_tests);
    zt_unit_register_test(suite, "decoding", decoding_tests);

    return 0;
}

