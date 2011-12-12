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
    } catch (zt_unit_exceptT&) {
        // one of ours please ignore
        return 0;
    } catch (char * str) {
        zt_unit_printf(&test->error, "unhandled exception: %s", str ? str : "NULL");
        zt_unit_test_add_exception(test);
        return 0;
    } catch (...) {
        // test threw out, log and continue
        zt_unit_printf(&test->error, "unhandled exception: unknown");
        zt_unit_test_add_exception(test);
        return 0;
    }
    return 0;
}

