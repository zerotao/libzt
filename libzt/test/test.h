/*
 * test.h        test macros
 *
 * Copyright (C) 2000, 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: test.h,v 1.1 2002/11/10 23:36:59 jshiffer Exp $
 *
 * $Log: test.h,v $
 * Revision 1.1  2002/11/10 23:36:59  jshiffer
 * Initial revision
 *
 * Revision 1.1.1.1  2002/01/21 01:03:15  jshiffer
 * Base Import
 *
 * Revision 1.1.1.1  2001/11/08 17:02:34  jshiffer
 * base import
 *
 * Revision 1.1.1.1  2001/07/30 14:32:20  jshiffer
 * base Import
 *
 * Revision 1.1.1.1  2001/07/03 01:46:24  jshiffer
 * Base import new deve server
 *
 * Revision 1.1.1.1  2001/02/12 03:54:30  jshiffer
 * Base Import libzt
 *
 * Revision 1.2  2000/10/25 17:37:04  jshiffer
 * General readability cleanups
 *
 * Revision 1.1.1.1  2000/10/25 16:05:43  jshiffer
 * Base checkin
 *
 */

/*
 * Description: 
 */

#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <libzt/zt.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif

#define TEST(n, t)							\
	do{									\
		int offset = 0;							\
		offset = printf(n);						\
		if(t)								\
			printf(BLANK "=> success\n", INDENT_TO(45, 5, offset)); \
		else								\
			printf(BLANK "=> failure @%s:%d\n", INDENT_TO(45, 5, offset), __FILE__, __LINE__); \
	}while(0)


#define TEST_N(n, i, t)						\
	do{							\
		int offset = 0;					\
		offset = printf("%s[%d]", n, i);		\
		i += 1;						\
		if(t)						\
			printf(BLANK "=> success\n",		\
			       INDENT_TO(45, 5, offset));	\
		else						\
			printf(BLANK "=> failure @%s:%d\n", 	\
			       INDENT_TO(45, 5, offset),	\
			       __FILE__, __LINE__);		\
	} while(0)

#ifdef __cplusplus
}
#endif

#endif /*_TEST_H_*/
