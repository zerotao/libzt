/*
 * zt_malloc.c        wrappers for malloc et. el.
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: malloc.c,v 1.4 2003/06/09 16:55:09 jshiffer Exp $
 *
 */


#include "zt.h"
#include "zt_log.h"

void_p
xcalloc_p ( num, size )
size_t num;
size_t size;
{
	void_p mem = NULL;	
	if(!(mem = (void_p *)calloc((num), (size)))){
		log_printf( log_emerg, "Could not calloc memory in xcalloc.");
	}
	return mem;
}

void_p
xmalloc_p ( num )
size_t num;
{
	void_p mem = NULL;	
	if(!(mem = (void_p *)malloc(num))){
		log_printf( log_emerg, "Could not malloc memory in xmalloc.");
	}
	return mem;		
}

void_p
xrealloc_p ( p, num )
void_p p;
size_t num;
{
	void_p mem = NULL;	
	if(!(mem = (void_p *)realloc(p, num))){
		log_printf( log_emerg, "Could not realloc memory in xrealloc.");
	}
	return mem;
}

void
xfree_p ( stale )
void_p stale;
{
	if(stale)
		free(stale);
}

char *
xstrdup ( string )
const char *string;
{
	char *new_string = NULL;		
	if( string ){
		if(!(new_string = strdup(string))){
			log_printf( log_emerg, "Could not xstrdup string.");
		}
	}
	return ( new_string );
}

