#include <string.h>

#define ZT_WITH_UNIT
#include <zt.h>

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    zt_bstream_t bs, clone;
    char     * test_string = "this is a test string",
               string_test[256] = "";
    uint8_t    test_uint8 = 0xDE,
               uint8_test = 0;
    uint16_t   test_uint16 = 0xDEAD,
               uint16_test = 0;
    uint32_t   test_uint32 = 0xDEADBEEF,
               uint32_test = 0;
    uint64_t   test_uint64 = 0xDEADBEEF,
               uint64_test = 0;

    float  test_float = 1.0f,
           float_test = 0.0f;
    double test_double = 2.0,
           double_test = 0.0;



    bs = zt_bstream_new();

    zt_bstream_write(bs, test_string, strlen(test_string),
                     sizeof(char), 0);

    zt_bstream_write_float(bs, test_float);
    zt_bstream_write_uint8(bs, test_uint8);
    zt_bstream_write_uint16(bs, test_uint16);
    zt_bstream_write_uint32(bs, test_uint32);
    zt_bstream_write_uint64(bs, test_uint64);
    zt_bstream_write_double(bs, test_double);

    zt_bstream_rewind(bs);

    zt_bstream_read(bs, &string_test[0], strlen(test_string),
                    sizeof(char), 0);
    zt_bstream_read_float(bs, &float_test);
    zt_bstream_read_uint8(bs, &uint8_test);
    zt_bstream_read_uint16(bs, &uint16_test);
    zt_bstream_read_uint32(bs, &uint32_test);
    zt_bstream_read_uint64(bs, &uint64_test);
    zt_bstream_read_double(bs, &double_test);

    ZT_UNIT_ASSERT(test,
                   strcmp(test_string, string_test) == 0);

    ZT_UNIT_ASSERT(test,
                   uint8_test == test_uint8);

    ZT_UNIT_ASSERT(test,
                   uint16_test == test_uint16);

    ZT_UNIT_ASSERT(test,
                   uint32_test == test_uint32);

    ZT_UNIT_ASSERT(test,
                   uint64_test == test_uint64);

    ZT_UNIT_ASSERT(test,
                   float_test == test_float);

    ZT_UNIT_ASSERT(test,
                   double_test == test_double);


    clone = zt_bstream_clone(bs);

    zt_bstream_rewind(clone);

    zt_bstream_read(clone, &string_test[0], strlen(test_string),
                    sizeof(char), 0);

    ZT_UNIT_ASSERT(test,
                   strcmp(test_string, string_test) == 0);



    zt_bstream_rewind(clone);
    clone->flipendian = 1;

    zt_bstream_write(clone, test_string, strlen(test_string),
                     sizeof(char), 0);

    zt_bstream_rewind(clone);

    memset(string_test, 0, 256);
    zt_bstream_read(clone, string_test, strlen(test_string),
                    sizeof(char), 0);



    ZT_UNIT_ASSERT(test, strcmp(string_test, test_string) == 0);

    zt_bstream_free(&bs);
    zt_bstream_free(&clone);

} /* basic_tests */

int
register_bstream_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "bstream", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
