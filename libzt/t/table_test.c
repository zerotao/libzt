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
#include <stdio.h>
#include <libzt/zt_table.h>

#include "test.h"

#define STR_TEST_PRE "strings "

static int count = 0;

static int str_iterator (void *key, void *datum, void *param);
static int int_iterator (void *key, void *datum, void *param);
static int str_free(void *key, void *datum, void *param);


int
main ( int argc, char *argv[] ){

	char	  	  buf[1024];
	int 		  i;
	int 		  nn = 0;
	int		  numbers[10] = { 9,8,7,6,5,4,3,2,1,0 };
	zt_table	* table_int = NULL;
	zt_table 	* table_str = NULL;
  
	table_int = table_init("test int table", table_hash_int, table_compare_int, 10, 0);
	table_str = table_init("test string table", table_hash_string,
			       table_compare_string, 10, 0);
  
	for( i=9; i >= 0; i--)
		table_set (table_int, (void *)i, (void *)numbers[i]);
  
	table_for_each(table_int, int_iterator, NULL);

	TEST("num items == 10", count == 10);
	
	nn = (int)table_get ( table_int, (void *)5 );
	TEST("lookup 5", nn == 4);

	/* string test */
	printf("Testing strings\n");
	
	count=0;		/* GLOBAL */
	
	for( i=9; i >= 0; i--) {
		char	* b2 = NULL;
		sprintf(buf, "%s%d", STR_TEST_PRE, i);
		b2 = XSTRDUP(buf);
		table_set(table_str, b2, (void *)numbers[i]);
	}
	
	table_for_each(table_str, str_iterator, NULL);
	TEST("num items == 10", count == 10);	

	table_for_each(table_str, str_free, NULL);
	
	table_destroy(table_int);
	table_destroy(table_str);
	return 0;
}

static int
str_free(void *key, void *datum, void *param)
{
	XFREE(key);
	return 0;
}

static int
str_iterator (void *key, void *datum, void *param)
{
	char	  buf[1024];
	int 	  d = (int)datum;	
	int 	  k = atoi((key + strlen(STR_TEST_PRE)));

	sprintf(buf, "key+datum(%d+%d) == 9", k, d);
	TEST(buf, (k + d == 9));
	
	count++;		/* GLOBAL */
	return 0;
}

static int
int_iterator (void *key, void *datum, void *param)
{
	char	  buf[1024];
	int	  d = (int)datum;
	int	  k = (int)key;
	

	sprintf(buf, "key+datum(%d+%d) == 9", k, d);
	TEST(buf, (k + d == 9));
	
	count++;		/* GLOBAL */
	return 0;
}
