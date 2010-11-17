#define ZT_WITH_UNIT

#include <zt.h>

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    ZT_UNIT_ASSERT(test, 1); /* we compiled so we pass */
}


int
register_include_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "include", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
