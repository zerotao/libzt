/*
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 *
 */

/*
 * Description: 
 */
#include <stdlib.h>
#include <stdio.h>

#include <libzt/zt_list.h>

int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

struct list_elt {
	zt_list	  list;
	int	  value;
};


int
main(int argc, char *argv[])
{
	zt_list	* al1;
	zt_list	* al2;
	int	  i;
	
	zt_list(list1);
	zt_list(list2);

	al1 = XCALLOC(zt_list, sizeof_array(values));
	al2 = XCALLOC(zt_list, sizeof_array(values));
	
	for(i=0; i < sizeof_array(values); i++){
		
		//zt_list_add(list1, 
	}
	
	
	return 0;
}
