/*!
 * Filename: zt_assert.c
 * Description: assertion tools
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_ASSERT_H_
#define _ZT_ASSERT_H_

#include <zt_internal.h>

BEGIN_C_DECLS

/*!
 * Name: _zt_assert_fail
 * Description: for an assertion
 * Notes: Don't call this directly, use assert instead
 */
extern void _zt_log_assertion(const char *condition, const char *file,
                              unsigned int line, const char *function);

/*!
 * Name: _zt_assert_fail
 * Description: for an assertion
 * Notes: Don't call this directly, use assert instead
 */
extern void _zt_log_abort(const char *condition, const char *file,
                          unsigned int line, const char *function);

/*!
 * Name: zt_assert
 * Description: if NDEBUG is not defined then assert on failure
 * Example:
 *      zt_assert("This would pass");
 *      zt_assert(!"But this would fail")
 *      zt_assert(foo == 1);
 */
#if defined(NDEBUG) || defined(NO_ASSERT)
# define zt_assert(c)
# define zt_assert_nf(c)
#else
# define zt_assert(c) (((c) ? 1 : (_zt_log_assertion(# c, __FILE__, __LINE__, __FUNCTION__), abort(), 0)))
# define zt_assert_nf(c) (((c) ? 1 : (_zt_log_assertion(# c, __FILE__, __LINE__, __FUNCTION__), 0)))
#endif /* if defined(NDEBUG) || defined(NO_ASSERT) */

/*!
 * Name: zt_abort
 * Description: abort with an error message
 * Eample:
 *      zt_abort("Could not alloc");
 *      zt_abort("Error while reading");
 */

#define zt_abort(msg) (_zt_log_abort(msg, __FILE__, __LINE__, __FUNCTION__), abort())

END_C_DECLS
#endif /*_ZT_ASSERT_H_*/
