/****h* libZT/Assertion
 * DESCRIPTION
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for detail.s
 *
 ****/

#ifndef _ZT_ASSERT_H_
#define _ZT_ASSERT_H_

#include <libzt/zt.h>
#include <libzt/zt_except.h>

extern char *assertion_failed;

BEGIN_C_DECLS

/****if* Assertion/_assert_fail
 * NAME
 *   _assert_fail - force an assertion
 *
 * NOTES
 *   Don't call this directly use assert instead
 *
 ****/
extern void _assert_fail (char *condition, char *file, 
						  unsigned int line, const char *function);

/****d* Assertion/assert
 * NAME
 *   assert -
 *
 * DESCRIPTION
 *   If NDEBUG is not defined then assert on failure
 *
 * SYNOPSIS
 *   assert(c)
 *
 * EXAMPLE
 *   assert("This would pass");
 *   assert(!"But this would fail")
 *   assert(foo == 1);
 *
 ****/
#if defined(NDEBUG) || defined(NO_ASSERT)
# define assert(c)
# define assert_nf(c)
#else
//# define assert(c) (((c) ? 1 : (_assert_fail(#c, __FILE__, __LINE__,  __FUNCTION__, 1), 0)))
# define assert(c) if (!(c)) {									\
		_assert_fail(#c, __FILE__, __LINE__, __FUNCTION__);	\
		TRY_THROW(assertion_failed);							\
	}

# define assert_nf(c) (((c) ? 1 : (_assert_fail(#c, __FILE__, __LINE__,  __FUNCTION__), 0)))
#endif 


END_C_DECLS
#endif /*_ZT_ASSERT_H_*/
