#define ZT_WITH_UNIT
#include "zt.h"
#include "zt_internal.h"

extern void _zt_unit_test_passed(struct zt_unit_test * test);

int
zt_unit_try_cpp(struct zt_unit_suite * suite,
                     struct zt_unit_test  * test) {
    try {
        test->test_fn(test, suite->data);
        return 1;
    } catch (int i) {
        if (i!=1) {
            throw i;
        }
    }
    return 0;
}

