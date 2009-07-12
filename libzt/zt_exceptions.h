#ifndef _ZT_EXCEPTIONS_H_
#define _ZT_EXCEPTIONS_H_

#ifdef ZT_EXCEPTIONS_DEFINE
# define EXCEPT_DEFINE
#endif
#include <libzt/zt_except.h>

EXCEPT_DESC(zt_exception, "Exceptions",
            EXCEPT_GROUP(assertion, "Assertions",
                         EXCEPTION(failed, "Assertion Failed"))
            EXCEPT_GROUP(memory, "Memory Exception",
                         EXCEPTION(no_mem, "Could not allocate memory")
                         EXCEPT_GROUP(pool, "Memory Pool Exception",
                                      EXCEPTION(does_not_exist, "Memory pool does not exist")
                                      EXCEPT_GROUP(group, "Memory Pool Group exception",
                                                   EXCEPTION(does_not_exist, "Memory Pool Group does not exist"))))
            EXCEPT_GROUP(unit, "Unit Test Exception",
                         EXCEPTION(test_failed, "Unit test failed"))
            EXCEPT_GROUP(format, "Format Exception",
                         EXCEPTION(overflow, "Format overflow"))
            EXCEPT_GROUP(gc, "GC Exception",
                         EXCEPTION(unbalanced_enable, "Attempt to enable GC when it was already enabled")
                         EXCEPTION(free_while_disabled, "Attempt to free the GC while garbage collection was disabled"))
            EXCEPT_GROUP(math, "Math Exception",
                         EXCEPTION(divide_by_zero, "Divide By Zero")
                         EXCEPTION(overflow, "Arithmetic overflow")));

#endif  /* _ZT_EXCEPTIONS_H_ */
