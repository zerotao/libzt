/* 
 * adt_test.c        adt tests
 * 
 * Copyright (C) 2001, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * 
 * 
 * $Id: adt_test.c,v 1.1 2002/11/10 23:36:59 jshiffer Exp $
 */

#include "test.h"
#include <libzt/adt/list.h>

int test_array[] = {
	1,2,3,4,5,6,7,8,9,0
};

int
main( argc, argv )
	int argc;
	char *argv[];
{
	ztlist *list;
	ztlistelem *elem = NULL;
	int i;
	
	
	list = list_init(NULL);
	
	for(i=0; i < 10; i++)
		elem = list_ins_next(list, elem, (void *)i);

	TEST("list[0]: ", (int)list_data(list_head(list)) == 0);
	TEST("list[1]: ", (int)list_data(list_tail(list)) == 9);	

	elem = list_tail(list);
	TEST("list[3]: ", list_is_tail(list, elem));

	elem = list_head(list);
	TEST("list[4]: ", list_is_head(list, elem));

	list_rem_next(list, NULL, (void *)&i);
	TEST("list[5]: ", list_data(list_head(list)) == (void *)1);

	list_rem_next(list, list_head(list), (void *)&i);
	TEST("list[6]: ", i == 2);
	
	list_destroy(list);
	TEST("list[7]: ", list_size(list) == 0);

	list_free(&list);
	TEST("list[8]: ", list == NULL);
	return 0;
}

