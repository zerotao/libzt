/*
 * zt_assert.h        ZeroTao Assertions
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: assert.h,v 1.1 2002/11/10 23:36:42 jshiffer Exp $
 */

#ifndef _ZT_ASSERT_H_
#define _ZT_ASSERT_H_

#include <libzt/zt.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif

/* Don't call this directly use assert instead */
extern void _assert_fail _((char *condition, char *file, 
			    unsigned int line, char *function)) NORETURN;

/** mf
 * If NDEBUG is not defined then assert on failure of c
 * @param c expression to be tested
 * @usage
 * assert("This would pass");
 * assert(!"But this would fail")
 * assert(foo == 1);
 */
#ifdef NDEBUG
# define assert(c) ((void) 0)
#else
# define assert(c) ((void)((c) ? 0 : _assert_fail(#c, __FILE__, __LINE__,  __FUNCTION__)))
#endif 


#ifdef __cplusplus
}
#endif

#endif /*_ZT_ASSERT_H_*/
