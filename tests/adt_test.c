/* 
 * adt_test.c        adt tests
 * 
 * Copyright (C) 2001, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * 
 * 
 * $Id: adt_test.c,v 1.1 2002/11/10 23:36:59 jshiffer Exp $
 */

#include <libzt/adt/list.h>
#include <libzt/zt_unit.h>

int test_array[] = {
	1,2,3,4,5,6,7,8,9,0
};

static void
basic_tests(struct zt_unit_test *test, void *data)
{
	zt_elist *list;
	zt_elist_elem *elem = NULL;
	int i;
	
	
	list = list_init(NULL);
	
	for(i=0; i < 10; i++)
		elem = list_ins_next(list, elem, (void *)i);

	ZT_UNIT_TEST(test, (int)list_data(list_head(list)) == 0);
	ZT_UNIT_TEST(test, (int)list_data(list_tail(list)) == 9);	

	elem = list_tail(list);
	ZT_UNIT_TEST(test, list_is_tail(list, elem));

	elem = list_head(list);
	ZT_UNIT_TEST(test, list_is_head(list, elem));

	list_rem_next(list, NULL, (void *)&i);
	ZT_UNIT_TEST(test, list_data(list_head(list)) == (void *)1);

	list_rem_next(list, list_head(list), (void *)&i);
	ZT_UNIT_TEST(test, i == 2);
	
	list_destroy(list);
	ZT_UNIT_TEST(test, list_size(list) == 0);

	list_free(&list);
	ZT_UNIT_TEST(test, list == NULL);
}

int
register_adt_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "adt", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}

