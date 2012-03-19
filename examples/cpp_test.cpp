#define ZT_WITH_UNIT
#include <zt.h>
#include <iostream>

extern void
test_fn(struct zt_unit_test * test, void * data UNUSED) {
    ZT_UNIT_ASSERT(test, 1 == 1); /* will pass */
    throw("Hi there!");
    ZT_UNIT_ASSERT(test, 1 == 1); /* will pass */
}

extern void
test2_fn(struct zt_unit_test * test, void * data UNUSED) {
    ZT_UNIT_ASSERT(test, 1 == 1); /* will pass */
}

int
main(int argc, char ** argv) {
    struct zt_unit       * unit;
    struct zt_unit_suite * suite;
    int                    result;

    unit   = zt_unit_init();
    suite  = zt_unit_register_suite(unit, "cpp test suite", NULL, NULL, NULL);
    zt_unit_register_test(suite, "cpp test 1", test_fn);
    zt_unit_register_test(suite, "cpp test 2", test2_fn);

    result = zt_unit_main(unit, argc, argv);
    zt_unit_release(&unit);
    return result;
}
