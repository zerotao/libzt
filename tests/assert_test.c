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
#include <libzt/zt_assert.h>
#include <libzt/zt_except.h>
#include <libzt/zt_log.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
	/* get rid of the log message for the moment */
#warning "Hiding log messages for assertions"
	log_ty	* olog;
	log_ty 	* log = log_file("/dev/null", 0, 0);
	
	olog = log_logger(log);
	
	assert(1==1);			/* will produce a warning: statement with no effect which is too true */
	ZT_UNIT_ASSERT_RAISES(test, assertion_failed, assert(1==0));

	log_logger(olog);
}

int
register_assert_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "assert tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
