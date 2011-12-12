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

    zt_buf_free(buf);
}

static void
iov_tests(struct zt_unit_test * test, void * data UNUSED) {
    zt_buf_t * buf;
    zt_iov_t   iov[4];
    char     * out;

    buf = zt_buf_new();

    iov[0].iov_len  = 3;
    iov[0].iov_data = "foo";
    iov[1].iov_len  = 3;
    iov[1].iov_data = "bar";
    iov[2].iov_len  = 3;
    iov[2].iov_data = "baz";
    iov[3].iov_len  = 1;
    iov[3].iov_data = "\0";

    ZT_UNIT_ASSERT_EQUAL(test, zt_buf_iov_add(buf, iov, 4), 0);
    ZT_UNIT_ASSERT_EQUAL(test, zt_buf_length(buf), 10);

    out = (char *)zt_buf_get(buf);
    ZT_UNIT_ASSERT_NOT_EQUAL(test, out, NULL);
    ZT_UNIT_ASSERT(test, strcmp("foobarbaz", out) == 0);

    zt_buf_free(buf);
}

static void
drain_tests(struct zt_unit_test * test, void * data UNUSED) {
    zt_buf_t * buf;

    buf = zt_buf_new();

    zt_buf_add(buf, "foobarbaz\0", 10);
    ZT_UNIT_ASSERT_EQUAL(test, zt_buf_length(buf), 10);
    ZT_UNIT_ASSERT(test, strcmp("foobarbaz", (char *)zt_buf_get(buf)) == 0);
    ZT_UNIT_ASSERT(test, zt_buf_drain(buf, 3) == 7);
    ZT_UNIT_ASSERT(test, strcmp("barbaz", (char *)zt_buf_get(buf)) == 0);
    ZT_UNIT_ASSERT_EQUAL(test, zt_buf_length(buf), 7);
    ZT_UNIT_ASSERT(test, zt_buf_drain(buf, 2) == 5);
    ZT_UNIT_ASSERT(test, strcmp("rbaz", (char *)zt_buf_get(buf)) == 0);
    ZT_UNIT_ASSERT_EQUAL(test, zt_buf_length(buf), 5);
    ZT_UNIT_ASSERT(test, zt_buf_drain(buf, -1) == 0);

    zt_buf_free(buf);
}

int
register_buf_suite(struct zt_unit * unit) {
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "zt_buf", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    zt_unit_register_test(suite, "iov", iov_tests);
    zt_unit_register_test(suite, "drain", drain_tests);
    return 0;
}

