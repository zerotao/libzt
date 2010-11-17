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
#include <zt_exceptions.h>
#include <zt_log.h>

#undef NDEBUG
#include <zt_unit.h>
#include <zt_assert.h>

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    /* get rid of the log message for the moment */
    zt_log_ty * olog;
    zt_log_ty * log;

    log = zt_log_file("/dev/null", 0, 0);
    olog = zt_log_logger(log);

    zt_assert(1 == 1);
    ZT_UNIT_ASSERT_RAISES(test, zt_exception.assertion.failed, zt_assert(1 == 0));

    zt_log_logger(olog);
    zt_log_close(log);
}

int
register_assert_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "assert tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
