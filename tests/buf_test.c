#include <string.h>

#define ZT_WITH_UNIT
#include <zt.h>

static void
basic_tests(struct zt_unit_test * test, void * data UNUSED) {
    zt_buf_t * buf;

    buf = zt_buf_new();

    ZT_UNIT_ASSERT_NOT_EQUAL(test, buf, NULL);
    ZT_UNIT_ASSERT_EQUAL(test, zt_buf_length(buf), 0);

    zt_buf_add(buf, "abcd", 4);
    ZT_UNIT_ASSERT_EQUAL(test, zt_buf_length(buf), 4);
    zt_buf_add(buf, "efgh", 4);
    ZT_UNIT_ASSERT_EQUAL(test, zt_buf_length(buf), 8);
}

int
register_buf_suite(struct zt_unit * unit) {
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "buf tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}

