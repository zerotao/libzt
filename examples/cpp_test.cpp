#define ZT_WITH_UNIT
#include <zt.h>
#include <iostream>

extern void
test_fn(struct zt_unit_test *test, void * data UNUSED) {
    ZT_UNIT_ASSERT(test, 1 == 2);
}

int
main(int argc, char **argv)
{
    struct zt_unit       * unit;
    struct zt_unit_suite * suite;
    struct zt_unit_test  * test;
    int                    result;

    unit = zt_unit_init();
    suite = zt_unit_register_suite(unit, "unit test suite", NULL, NULL, NULL);
    test = zt_unit_register_test(suite, "unit test", test_fn);

    result = zt_unit_main(unit, argc, argv);
    zt_unit_release(&unit);
    return result;
}
