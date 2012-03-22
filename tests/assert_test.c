/*
 * assert_test.c        test assertions
 *
 * Copyright (C) 2000-2002, 2004, 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: assert_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description:
 */
#undef NDEBUG
#define ZT_WITH_UNIT
#include <zt.h>

#define DUMMY_LOG "dummy.log"

int rr_val = 0;

void
test_return(int x) {
    zt_assert_return(1==x);
    rr_val = 1;
}

int
test_returnv(int x) {
    zt_assert_returnV(1==x, 1);

    return 2;
}

static void
basic_tests(struct zt_unit_test *test UNUSED, void *data UNUSED)
{
    /* get rid of the log message for the moment */
    zt_log_ty * olog;
    zt_log_ty * log;
    int         i = 1;
    int         ri_val = 0;

    log = zt_log_file(DUMMY_LOG, 0, 0);
    olog = zt_log_logger(log);

    zt_assert(1 == i);

    rr_val = 0;
    test_return(0);
    ZT_UNIT_ASSERT(test, rr_val == 0);

    rr_val = 0;
    test_return(1);
    ZT_UNIT_ASSERT(test, rr_val == 1);

    ri_val = test_returnv(0);
    ZT_UNIT_ASSERT(test, ri_val == 1);

    ri_val = test_returnv(1);
    ZT_UNIT_ASSERT(test, ri_val == 2);

    zt_log_logger(olog);
    zt_log_close(log);

#if !defined(WIN32)
    unlink(DUMMY_LOG);
#endif
}

int
register_assert_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "assert tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
