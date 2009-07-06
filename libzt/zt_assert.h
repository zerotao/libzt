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

extern char *zt_assertion_failed;

BEGIN_C_DECLS

/****if* Assertion/_zt_assert_fail
 * NAME
 *   _zt_assert_fail - force an assertion
 *
 * NOTES
 *   Don't call this directly use assert instead
 *
 ****/
extern void _zt_assert_fail (char *condition, char *file, 
                             unsigned int line, const char *function);

/****d* Assertion/zt_assert
 * NAME
 *   zt_assert -
 *
 * DESCRIPTION
 *   If NDEBUG is not defined then assert on failure
 *
 * SYNOPSIS
 *   zt_assert(c)
 *
 * EXAMPLE
 *   zt_assert("This would pass");
 *   zt_assert(!"But this would fail")
 *   zt_assert(foo == 1);
 *
 ****/
#if defined(NDEBUG) || defined(NO_ASSERT)
# define zt_assert(c)
# define zt_assert_nf(c)
#else
/* # define zt_assert(c) (((c) ? 1 : (_zt_assert_fail(#c, __FILE__, __LINE__,  __FUNCTION__, 1), 0))) */
# define zt_assert(c) if (!(c)) {									\
		_zt_assert_fail(#c, __FILE__, __LINE__, __FUNCTION__);      \
		TRY_THROW(zt_assertion_failed);                             \
	}

# define zt_assert_nf(c) (((c) ? 1 : (_zt_assert_fail(#c, __FILE__, __LINE__,  __FUNCTION__), 0)))
#endif 


END_C_DECLS
#endif /*_ZT_ASSERT_H_*/
