#define ZT_WITH_UNIT
#include <zt.h>

#include <time.h>
#include <string.h>
#include <ctype.h>
#include <zt_base.h>

#define test_encoding(_base, _data, _len, _result1, _result2)                                   \
    do { unsigned char * cdata = (unsigned char *)_data;                                        \
         char          * result = malloc(20);                                                   \
         int             ret;                                                                   \
         size_t          len;                                                                   \
         bool            bitisset = false;                                                      \
         void          **rptr = (void **)&result;                                               \
         memset(result, 0, 20);                                                                 \
         len = 20;                                                                              \
         bitisset = ZT_BIT_ISSET(_base->flags, zt_base_encode_with_padding);                    \
         ZT_BIT_UNSET(_base->flags, zt_base_encode_with_padding);                               \
         ret = zt_base_encode(_base, cdata, _len, rptr, &len);                                  \
         ZT_UNIT_ASSERT(test, !ret);                                                            \
         ZT_UNIT_ASSERT(test, strcmp(result, _result1) == 0);                                   \
         ZT_UNIT_ASSERT(test, strlen(result) == len);                                           \
         if (bitisset) ZT_BIT_SET(_base->flags, zt_base_encode_with_padding);                   \
                                                                                                \
         memset(result, 0, 20);                                                                 \
         len = 20;                                                                              \
         ret = zt_base_encode(_base, cdata, _len, rptr, &len);                                  \
         ZT_UNIT_ASSERT(test, !ret);                                                            \
         ZT_UNIT_ASSERT(test, strcmp(result, _result2) == 0);                                   \
         ZT_UNIT_ASSERT(test, strlen(result) == len); zt_free(result);} while (0)

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
         ZT_UNIT_ASSERT(test, strlen(result) == len); zt_free(result);} while (0)

static void
encoding_tests(struct zt_unit_test * test, void * _data UNUSED) {
    char * data = NULL;
    char   result[20];

    #define OLEN sizeof(result)
    size_t len  = OLEN;
    int    ret;
	void ** rptr = 0;

    memset(result, 0, 20);
    rptr = (void **)&result;
    /* base 64 tests */

    /* check bad param, no out_len */
    data = "wee";
    ret  = zt_base_encode(zt_base64_rfc, data, strlen(data), rptr, NULL);
    ZT_UNIT_ASSERT(test, ret == -1);

    /* check null buff but valid bufsize */
    ret  = zt_base_encode(zt_base64_rfc, data, strlen(data), NULL, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, len == 4);

    /* check insufficient output space */
    len  = 2;
    ret  = zt_base_encode(zt_base64_rfc, data, strlen(data), rptr, &len);
    ZT_UNIT_ASSERT(test, ret == -2);
    ZT_UNIT_ASSERT(test, len == 4);

    /* check missing input */
    len  = OLEN;
    ret  = zt_base_encode(zt_base64_rfc, NULL, 0, rptr, &len);
    ZT_UNIT_ASSERT(test, ret == 0);
    ZT_UNIT_ASSERT(test, len == 0);

    /* check empty input */
    len  = OLEN;
    ret  = zt_base_encode(zt_base64_rfc, "", 0, rptr, &len);
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

    /* RFC4648 nopad test vectors */
    test_encoding(zt_base64_rfc_nopad, "", 0, "", "");
    test_encoding(zt_base64_rfc_nopad, "f", 1, "Zg", "Zg");
    test_encoding(zt_base64_rfc_nopad, "fo", 2, "Zm8", "Zm8");
    test_encoding(zt_base64_rfc_nopad, "foo", 3, "Zm9v", "Zm9v");
    test_encoding(zt_base64_rfc_nopad, "foob", 4, "Zm9vYg", "Zm9vYg");
    test_encoding(zt_base64_rfc_nopad, "fooba", 5, "Zm9vYmE", "Zm9vYmE");
    test_encoding(zt_base64_rfc_nopad, "foobar", 6, "Zm9vYmFy", "Zm9vYmFy");

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

#ifdef WIN32
    test_decoding(zt_base64_rfc, "\r\nfoobar", 7, "Zm9vYmFy", "Zm9vYmFy");
    test_decoding(zt_base64_rfc, "f\r\noobar", 7, "Zm9vYmFy", "Zm9vYmFy");
    test_decoding(zt_base64_rfc, "fo\r\nobar", 7, "Zm9vYmFy", "Zm9vYmFy");
    test_decoding(zt_base64_rfc, "foo\r\nbar", 7, "Zm9vYmFy", "Zm9vYmFy");
    test_decoding(zt_base64_rfc, "foob\r\nar", 7, "Zm9vYmFy", "Zm9vYmFy");
    test_decoding(zt_base64_rfc, "fooba\r\nr", 7, "Zm9vYmFy", "Zm9vYmFy");
    test_decoding(zt_base64_rfc, "foobar\r\n", 7, "Zm9vYmFy", "Zm9vYmFy");
#endif


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

    {
        size_t    i;
        char    * in = NULL;
        size_t    len = 0;
        char    * out = NULL;
        size_t    olen = 0;
        char    * enc = NULL;
        size_t    elen = 0;

#if HAVE_SRANDOMDEV
        srandomdev();
#else
        srandom(time(NULL));
#endif

        while((len = random() % 1024) < 20);

        in = zt_calloc(char, len);
        /* out = zt_calloc(char, len); */

        for(i=0; i < len; i++) {
            char      k;

            while((k = random()) && !isprint(k));

            in[i] = k;
        }

        zt_base_encode(zt_base64_rfc, in, len, (void **)&enc, &elen);
        zt_base_decode(zt_base64_rfc, enc, elen, (void **)&out, &olen);

        if(len != olen ||
           strcmp(in, out) != 0) {
            ZT_UNIT_FAIL(test, "%s failed to encode/decode correctly %s:%d", in, __FILE__, __LINE__);
        }
    }
} /* decoding_tests */


static void
block_tests(struct zt_unit_test * test, void * _data UNUSED) {
    char      text[] = {
        47,  42,  10,  32,  42,  32,  97,  115, 115, 101, 114, 116, 95,  116, 101, 115, 116, 46,  99,  32,  32,  32,
        32,  32,  32,  32,  32,  116, 101, 115, 116, 32,  97,  115, 115, 101, 114, 116, 105, 111, 110, 115, 10,
        32,  42,  10,  32,  42,  32,  67,  111, 112, 121, 114, 105, 103, 104, 116, 32,  40,  67,  41,  32,  50,  48, 48,
        48,  45,  50,  48,  48,  50,  44,  32,  50,  48,  48,  52,  44,  32,  50,  48,  48,  53,  44,  32,  74,  97, 115, 111,
        110, 32,  76,  46,  32,  83,  104, 105, 102, 102, 101, 114, 32,  60,  106, 115, 104, 105, 102, 102, 101,
        114, 64,  122, 101, 114, 111, 116, 97,  111, 46,  99,  111, 109, 62,  46,  32,  32,  65,  108, 108, 32,  82,
        105, 103, 104, 116, 115, 32,  82,  101, 115, 101, 114, 118, 101, 100, 46,  10,  32,  42,  32,  83,  101,
        101, 32,  102, 105, 108, 101, 32,  67,  79,  80,  89,  73,  78,  71,  32,  102, 111, 114, 32,  100, 101, 116,
        97,  105, 108, 115, 46,  10,  32,  42,  10,  32,  42,  32,  36,  73,  100, 58,  32,  97,  115, 115, 101, 114,
        116, 95,  116, 101, 115, 116, 46,  99,  44,  118, 32,  49,  46,  50,  32,  50,  48,  48,  51,  47,  48,  54, 47,
        48,  57,  32,  49,  51,  58,  52,  50,  58,  49,  50,  32,  106, 115, 104, 105, 102, 102, 101, 114, 32,  69,
        120, 112, 32,  36,  10,  32,  42,  10,  32,  42,  47,  10,  10,  47,  42,  10,  32,  42,  32,  68,  101, 115,99,  114,
        105, 112, 116, 105, 111, 110, 58,  10,  32,  42,  47,  10,  35,  117, 110, 100, 101, 102, 32,  78,  68,  69,
        66,  85,  71,  10,  35,  100, 101, 102, 105, 110, 101, 32,  90,  84,  95,  87,  73,  84,  72,  95,  85,  78, 73,
        84,  10,  35,  105, 110, 99,  108, 117, 100, 101, 32,  60,  122, 116, 46,  104, 62,  10,  10,  35,  100, 101,
        102, 105, 110, 101, 32,  68,  85,  77,  77,  89,  95,  76,  79,  71,  32,  34,  100, 117, 109, 109, 121, 46,
        108, 111, 103, 34,  10,  10,  105, 110, 116, 32,  114, 114, 95,  118, 97,  108, 32,  61,  32,  48,  59,  10,
        10,  118, 111, 105, 100, 10,  116, 101, 115, 116, 95,  114, 101, 116, 117, 114, 110, 40,  105, 110,
        116, 32,  120, 41,  32,  123, 10,  32,  32,  32,  32,  122, 116, 95,  97,  115, 115, 101, 114, 116, 95,  114,
        101, 116, 117, 114, 110, 40,  49,  61,  61,  120, 41,  59,  10,  32,  32,  32,  32,  114, 114, 95,  118, 97,
        108, 32,  61,  32,  49,  59,  10,  125, 10,  10,  105, 110, 116, 10,  116, 101, 115, 116, 95,  114, 101,
        116, 117, 114, 110, 118, 40,  105, 110, 116, 32,  120, 41,  32,  123, 10,  32,  32,  32,  32,  122, 116,
        95,  97,  115, 115, 101, 114, 116, 95,  114, 101, 116, 117, 114, 110, 86,  40,  49,  61,  61,  120, 44,
        32,  49,  41,  59,  10,  10,  32,  32,  32,  32,  114, 101, 116, 117, 114, 110, 32,  50,  59,  10,  125, 10, 10,
        115, 116, 97,  116, 105, 99,  32,  118, 111, 105, 100, 10,  98,  97,  115, 105, 99,  95,  116, 101, 115,
        116, 115, 40,  115, 116, 114, 117, 99,  116, 32,  122, 116, 95,  117, 110, 105, 116, 95,  116, 101,
        115, 116, 32,  42,  116, 101, 115, 116, 32,  85,  78,  85,  83,  69,  68,  44,  32,  118, 111, 105, 100, 32,
        42,  100, 97,  116, 97,  32,  85,  78,  85,  83,  69,  68,  41,  10,  123, 10,  32,  32,  32,  32,  47,  42, 32,  103,
        101, 116, 32,  114, 105, 100, 32,  111, 102, 32,  116, 104, 101, 32,  108, 111, 103, 32,  109, 101,
        115, 115, 97,  103, 101, 32,  102, 111, 114, 32,  116, 104, 101, 32,  109, 111, 109, 101, 110, 116,
        32,  42,  47,  10,  32,  32,  32,  32,  122, 116, 95,  108, 111, 103, 95,  116, 121, 32,  42,  32,  111, 108,
        111, 103, 59,  10,  32,  32,  32,  32,  122, 116, 95,  108, 111, 103, 95,  116, 121, 32,  42,  32,  108, 111,
        103, 59,  10,  32,  32,  32,  32,  105, 110, 116, 32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 32,  61, 32,  49,
        59,  10,  32,  32,  32,  32,  105, 110, 116, 32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 105, 95,  118,97,
        108, 32,  61,  32,  48,  59,  10,  10,  32,  32,  32,  32,  108, 111, 103, 32,  61,  32,  122, 116, 95,  108,111,
        103, 95,  102, 105, 108, 101, 40,  68,  85,  77,  77,  89,  95,  76,  79,  71,  44,  32,  48,  44,  32,  48, 41,  59,
        10,  32,  32,  32,  32,  111, 108, 111, 103, 32,  61,  32,  122, 116, 95,  108, 111, 103, 95,  108, 111,
        103, 103, 101, 114, 40,  108, 111, 103, 41,  59,  10,  10,  32,  32,  32,  32,  122, 116, 95,  97,  115, 115,
        101, 114, 116, 40,  49,  32,  61,  61,  32,  105, 41,  59,  10,  10,  32,  32,  32,  32,  114, 114, 95,  118,97,
        108, 32,  61,  32,  48,  59,  10,  32,  32,  32,  32,  116, 101, 115, 116, 95,  114, 101, 116, 117, 114, 110,
        40,  48,  41,  59,  10,  32,  32,  32,  32,  90,  84,  95,  85,  78,  73,  84,  95,  65,  83,  83,  69,  82, 84,  40,  116,
        101, 115, 116, 44,  32,  114, 114, 95,  118, 97,  108, 32,  61,  61,  32,  48,  41,  59,  10,  10,  32,  32, 32,
        32,  114, 114, 95,  118, 97,  108, 32,  61,  32,  48,  59,  10,  32,  32,  32,  32,  116, 101, 115, 116, 95,
        114, 101, 116, 117, 114, 110, 40,  49,  41,  59,  10,  32,  32,  32,  32,  90,  84,  95,  85,  78,  73,  84, 95,
        65,  83,  83,  69,  82,  84,  40,  116, 101, 115, 116, 44,  32,  114, 114, 95,  118, 97,  108, 32,  61,  61, 32,
        49,  41,  59,  10,  10,  32,  32,  32,  32,  114, 105, 95,  118, 97,  108, 32,  61,  32,  116, 101, 115, 116,95,
        114, 101, 116, 117, 114, 110, 118, 40,  48,  41,  59,  10,  32,  32,  32,  32,  90,  84,  95,  85,  78,  73, 84,
        95,  65,  83,  83,  69,  82,  84,  40,  116, 101, 115, 116, 44,  32,  114, 105, 95,  118, 97,  108, 32,  61, 61,
        32,  49,  41,  59,  10,  10,  32,  32,  32,  32,  114, 105, 95,  118, 97,  108, 32,  61,  32,  116, 101, 115,116,
        95,  114, 101, 116, 117, 114, 110, 118, 40,  49,  41,  59,  10,  32,  32,  32,  32,  90,  84,  95,  85,  78, 73,
        84,  95,  65,  83,  83,  69,  82,  84,  40,  116, 101, 115, 116, 44,  32,  114, 105, 95,  118, 97,  108, 32, 61,
        61,  32,  50,  41,  59,  10,  10,  32,  32,  32,  32,  122, 116, 95,  108, 111, 103, 95,  108, 111, 103, 103,
        101, 114, 40,  111, 108, 111, 103, 41,  59,  10,  32,  32,  32,  32,  122, 116, 95,  108, 111, 103, 95,  99,
        108, 111, 115, 101, 40,  108, 111, 103, 41,  59,  10,  10,  35,  105, 102, 32,  33,  100, 101, 102, 105,
        110, 101, 100, 40,  87,  73,  78,  51,  50,  41,  10,  32,  32,  32,  32,  117, 110, 108, 105, 110, 107, 40,
        68,  85,  77,  77,  89,  95,  76,  79,  71,  41,  59,  10,  35,  101, 110, 100, 105, 102, 10,  125, 10,  10, 105,
        110, 116, 10,  114, 101, 103, 105, 115, 116, 101, 114, 95,  97,  115, 115, 101, 114, 116, 95,  115,
        117, 105, 116, 101, 40,  115, 116, 114, 117, 99,  116, 32,  122, 116, 95,  117, 110, 105, 116, 32,
        42,  117, 110, 105, 116, 41,  10,  123, 10,  32,  32,  32,  32,  115, 116, 114, 117, 99,  116, 32,  122,
        116, 95,  117, 110, 105, 116, 95,  115, 117, 105, 116, 101, 32,  42,  32,  115, 117, 105, 116, 101,
        59,  10,  10,  32,  32,  32,  32,  115, 117, 105, 116, 101, 32,  61,  32,  122, 116, 95,  117, 110, 105, 116,
        95,  114, 101, 103, 105, 115, 116, 101, 114, 95,  115, 117, 105, 116, 101, 40,  117, 110, 105, 116,
        44,  32,  34,  97,  115, 115, 101, 114, 116, 32,  116, 101, 115, 116, 115, 34,  44,  32,  78,  85,  76,  76,
        44,  32,  78,  85,  76,  76,  44,  32,  78,  85,  76,  76,  41,  59,  10,  32,  32,  32,  32,  122, 116, 95, 117, 110,
        105, 116, 95,  114, 101, 103, 105, 115, 116, 101, 114, 95,  116, 101, 115, 116, 40,  115, 117, 105,
        116, 101, 44,  32,  34,  98,  97,  115, 105, 99,  34,  44,  32,  98,  97,  115, 105, 99,  95,  116, 101, 115,
        116, 115, 41,  59,  10,  32,  32,  32,  32,  114, 101, 116, 117, 114, 110, 32,  48,  59,  10,  125, 10, 0
    };

    const char * encoded = "LyoKICogYXNzZXJ0X3Rlc3QuYyAgICAgICAgdGVzdCBhc3NlcnRpb25zCiAqCiAq"
                           "IENvcHlyaWdodCAoQykgMjAwMC0yMDAyLCAyMDA0LCAyMDA1LCBKYXNvbiBMLiBT"
                           "aGlmZmVyIDxqc2hpZmZlckB6ZXJvdGFvLmNvbT4uICBBbGwgUmlnaHRzIFJlc2Vy"
                           "dmVkLgogKiBTZWUgZmlsZSBDT1BZSU5HIGZvciBkZXRhaWxzLgogKgogKiAkSWQ6"
                           "IGFzc2VydF90ZXN0LmMsdiAxLjIgMjAwMy8wNi8wOSAxMzo0MjoxMiBqc2hpZmZl"
                           "ciBFeHAgJAogKgogKi8KCi8qCiAqIERlc2NyaXB0aW9uOgogKi8KI3VuZGVmIE5E"
                           "RUJVRwojZGVmaW5lIFpUX1dJVEhfVU5JVAojaW5jbHVkZSA8enQuaD4KCiNkZWZp"
                           "bmUgRFVNTVlfTE9HICJkdW1teS5sb2ciCgppbnQgcnJfdmFsID0gMDsKCnZvaWQK"
                           "dGVzdF9yZXR1cm4oaW50IHgpIHsKICAgIHp0X2Fzc2VydF9yZXR1cm4oMT09eCk7"
                           "CiAgICBycl92YWwgPSAxOwp9CgppbnQKdGVzdF9yZXR1cm52KGludCB4KSB7CiAg"
                           "ICB6dF9hc3NlcnRfcmV0dXJuVigxPT14LCAxKTsKCiAgICByZXR1cm4gMjsKfQoK"
                           "c3RhdGljIHZvaWQKYmFzaWNfdGVzdHMoc3RydWN0IHp0X3VuaXRfdGVzdCAqdGVz"
                           "dCBVTlVTRUQsIHZvaWQgKmRhdGEgVU5VU0VEKQp7CiAgICAvKiBnZXQgcmlkIG9m"
                           "IHRoZSBsb2cgbWVzc2FnZSBmb3IgdGhlIG1vbWVudCAqLwogICAgenRfbG9nX3R5"
                           "ICogb2xvZzsKICAgIHp0X2xvZ190eSAqIGxvZzsKICAgIGludCAgICAgICAgIGkg"
                           "PSAxOwogICAgaW50ICAgICAgICAgcmlfdmFsID0gMDsKCiAgICBsb2cgPSB6dF9s"
                           "b2dfZmlsZShEVU1NWV9MT0csIDAsIDApOwogICAgb2xvZyA9IHp0X2xvZ19sb2dn"
                           "ZXIobG9nKTsKCiAgICB6dF9hc3NlcnQoMSA9PSBpKTsKCiAgICBycl92YWwgPSAw"
                           "OwogICAgdGVzdF9yZXR1cm4oMCk7CiAgICBaVF9VTklUX0FTU0VSVCh0ZXN0LCBy"
                           "cl92YWwgPT0gMCk7CgogICAgcnJfdmFsID0gMDsKICAgIHRlc3RfcmV0dXJuKDEp"
                           "OwogICAgWlRfVU5JVF9BU1NFUlQodGVzdCwgcnJfdmFsID09IDEpOwoKICAgIHJp"
                           "X3ZhbCA9IHRlc3RfcmV0dXJudigwKTsKICAgIFpUX1VOSVRfQVNTRVJUKHRlc3Qs"
                           "IHJpX3ZhbCA9PSAxKTsKCiAgICByaV92YWwgPSB0ZXN0X3JldHVybnYoMSk7CiAg"
                           "ICBaVF9VTklUX0FTU0VSVCh0ZXN0LCByaV92YWwgPT0gMik7CgogICAgenRfbG9n"
                           "X2xvZ2dlcihvbG9nKTsKICAgIHp0X2xvZ19jbG9zZShsb2cpOwoKI2lmICFkZWZp"
                           "bmVkKFdJTjMyKQogICAgdW5saW5rKERVTU1ZX0xPRyk7CiNlbmRpZgp9CgppbnQK"
                           "cmVnaXN0ZXJfYXNzZXJ0X3N1aXRlKHN0cnVjdCB6dF91bml0ICp1bml0KQp7CiAg"
                           "ICBzdHJ1Y3QgenRfdW5pdF9zdWl0ZSAqIHN1aXRlOwoKICAgIHN1aXRlID0genRf"
                           "dW5pdF9yZWdpc3Rlcl9zdWl0ZSh1bml0LCAiYXNzZXJ0IHRlc3RzIiwgTlVMTCwg"
                           "TlVMTCwgTlVMTCk7CiAgICB6dF91bml0X3JlZ2lzdGVyX3Rlc3Qoc3VpdGUsICJi"
                           "YXNpYyIsIGJhc2ljX3Rlc3RzKTsKICAgIHJldHVybiAwOwp9Cg==";

    const char * encoded2 = \
"LyoKICogYXNzZXJ0X3Rlc3QuYyAgICAgICAgdGVzdCBhc3NlcnRpb25zCiAqCiAq \
IENvcHlyaWdodCAoQykgMjAwMC0yMDAyLCAyMDA0LCAyMDA1LCBKYXNvbiBMLiBT \
aGlmZmVyIDxqc2hpZmZlckB6ZXJvdGFvLmNvbT4uICBBbGwgUmlnaHRzIFJlc2Vy \
dmVkLgogKiBTZWUgZmlsZSBDT1BZSU5HIGZvciBkZXRhaWxzLgogKgogKiAkSWQ6 \
IGFzc2VydF90ZXN0LmMsdiAxLjIgMjAwMy8wNi8wOSAxMzo0MjoxMiBqc2hpZmZl \
ciBFeHAgJAogKgogKi8KCi8qCiAqIERlc2NyaXB0aW9uOgogKi8KI3VuZGVmIE5E \
RUJVRwojZGVmaW5lIFpUX1dJVEhfVU5JVAojaW5jbHVkZSA8enQuaD4KCiNkZWZp \
bmUgRFVNTVlfTE9HICJkdW1teS5sb2ciCgppbnQgcnJfdmFsID0gMDsKCnZvaWQK \
dGVzdF9yZXR1cm4oaW50IHgpIHsKICAgIHp0X2Fzc2VydF9yZXR1cm4oMT09eCk7 \
CiAgICBycl92YWwgPSAxOwp9CgppbnQKdGVzdF9yZXR1cm52KGludCB4KSB7CiAg \
ICB6dF9hc3NlcnRfcmV0dXJuVigxPT14LCAxKTsKCiAgICByZXR1cm4gMjsKfQoK \
c3RhdGljIHZvaWQKYmFzaWNfdGVzdHMoc3RydWN0IHp0X3VuaXRfdGVzdCAqdGVz \
dCBVTlVTRUQsIHZvaWQgKmRhdGEgVU5VU0VEKQp7CiAgICAvKiBnZXQgcmlkIG9m \
IHRoZSBsb2cgbWVzc2FnZSBmb3IgdGhlIG1vbWVudCAqLwogICAgenRfbG9nX3R5 \
ICogb2xvZzsKICAgIHp0X2xvZ190eSAqIGxvZzsKICAgIGludCAgICAgICAgIGkg \
PSAxOwogICAgaW50ICAgICAgICAgcmlfdmFsID0gMDsKCiAgICBsb2cgPSB6dF9s \
b2dfZmlsZShEVU1NWV9MT0csIDAsIDApOwogICAgb2xvZyA9IHp0X2xvZ19sb2dn \
ZXIobG9nKTsKCiAgICB6dF9hc3NlcnQoMSA9PSBpKTsKCiAgICBycl92YWwgPSAw \
OwogICAgdGVzdF9yZXR1cm4oMCk7CiAgICBaVF9VTklUX0FTU0VSVCh0ZXN0LCBy \
cl92YWwgPT0gMCk7CgogICAgcnJfdmFsID0gMDsKICAgIHRlc3RfcmV0dXJuKDEp \
OwogICAgWlRfVU5JVF9BU1NFUlQodGVzdCwgcnJfdmFsID09IDEpOwoKICAgIHJp \
X3ZhbCA9IHRlc3RfcmV0dXJudigwKTsKICAgIFpUX1VOSVRfQVNTRVJUKHRlc3Qs \
IHJpX3ZhbCA9PSAxKTsKCiAgICByaV92YWwgPSB0ZXN0X3JldHVybnYoMSk7CiAg \
ICBaVF9VTklUX0FTU0VSVCh0ZXN0LCByaV92YWwgPT0gMik7CgogICAgenRfbG9n \
X2xvZ2dlcihvbG9nKTsKICAgIHp0X2xvZ19jbG9zZShsb2cpOwoKI2lmICFkZWZp \
bmVkKFdJTjMyKQogICAgdW5saW5rKERVTU1ZX0xPRyk7CiNlbmRpZgp9CgppbnQK \
cmVnaXN0ZXJfYXNzZXJ0X3N1aXRlKHN0cnVjdCB6dF91bml0ICp1bml0KQp7CiAg \
ICBzdHJ1Y3QgenRfdW5pdF9zdWl0ZSAqIHN1aXRlOwoKICAgIHN1aXRlID0genRf \
dW5pdF9yZWdpc3Rlcl9zdWl0ZSh1bml0LCAiYXNzZXJ0IHRlc3RzIiwgTlVMTCwg \
TlVMTCwgTlVMTCk7CiAgICB6dF91bml0X3JlZ2lzdGVyX3Rlc3Qoc3VpdGUsICJi \
YXNpYyIsIGJhc2ljX3Rlc3RzKTsKICAgIHJldHVybiAwOwp9Cg==";

    char    * out = NULL;
    size_t    outlen = 0;

    ZT_UNIT_ASSERT(test, zt_base_encode(zt_base64_rfc, text, strlen(text), (void **)&out, &outlen) == 0);
    ZT_UNIT_ASSERT(test, strcmp(out, encoded) == 0);
    free(out);

    out = NULL;
    outlen = 0;

    ZT_UNIT_ASSERT(test, zt_base_decode(zt_base64_rfc, encoded, strlen(encoded), (void **)&out, &outlen) == 0);
    ZT_UNIT_ASSERT(test, strcmp(out, text) == 0);
    free(out);

    out = NULL;
    outlen = 0;

    ZT_UNIT_ASSERT(test, zt_base_decode(zt_base64_rfc, encoded2, strlen(encoded2), (void **)&out, &outlen) == 0);
    ZT_UNIT_ASSERT(test, strcmp(out, text) == 0);
    free(out);

}

int
register_base_suite(struct zt_unit * unit) {
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "baseN", NULL, NULL, NULL);
    zt_unit_register_test(suite, "encoding", encoding_tests);
    zt_unit_register_test(suite, "decoding", decoding_tests);
    zt_unit_register_test(suite, "block", block_tests);

    return 0;
}

