/* 
 * Copyright (C) 2001, 2004, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * 
 * 
 * $Id: tree_test.c,v 1.1 2002/11/10 23:36:59 jshiffer Exp $
 * 
 */

/*
 * Description: tree tests
 */
#include <libzt/zt.h>
#include <libzt/zt_tree.h>

int ordered_dataset[] = {
	0,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,
	30,31,32,33,34,35,36,37,38,39,
	40,41,42,43,44,45,46,47,48,49,
	50,51,52,53,54,55,56,57,58,59,
	60,61,62,63,64,65,66,67,68,69,
	70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,
	90,91,92,93,94,95,96,97,98,99
};


int int_compare( void *n1, void *n2 )
{

	if(*(int *)n1 < *(int *)n2)
		return -1;
	else if(*(int *)n1 == *(int *)n2)
		return 0;
	else
		return 1;
}


int
main (int argc, char *argv[])
{
	struct zt_tree *br_root;
	int i;

	br_root = tree_blackred( NULL, sizeof(int), 0, int_compare ); /* key(), key_size, data_size, compare_func (ala qsort) */

	for(i=0; i < 100; i++){
		tree_insert( br_root, (void *) ordered_dataset[i] );
	}

	
	return 0;
}
