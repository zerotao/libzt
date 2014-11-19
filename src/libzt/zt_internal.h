/*
 * Filename: zt_interal.h
 * Description: internal definitions and support SHOULD NOT BE EXPORTED!!
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright: Copyright (C) 2000-2010, Jason L. Shiffer
 * Created: 02-28-2000
 *
 * Notes:
 *
 */

#ifndef _ZT_INTERNAL_H_
#define _ZT_INTERNAL_H_

/* only cross platform headers C99  or replacements */
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <limits.h>

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif

#ifdef WIN32
# include <zt_win32.h>
#else
# include <zt_unix.h>
#endif

#if WITH_DMALLOC
# include <dmalloc.h>
#endif /* WITH_DMALLOC */


#ifdef __GNUC__
#  ifndef const
#    define const __const
#  endif
#  ifndef signed
#    define signed __signed
#  endif
#  ifndef volatile
#    define volatile __volatile
#  endif
#else /* ifdef __GNUC__ */
#  if defined(__STDC__) || defined(__STDC_HOSTED__)
#    undef  signed
#    define signed
#    undef  volatile
#    define volatile
#  endif
#endif /* ifdef __GNUC__ */


#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS  0
#  define EXIT_FAILURE  1
#endif

#ifndef _
#  define _(x) x
#endif /* ifndef _ */

/************ Convienience Definitions  ************/

#ifndef HEX_DIGITS
# define HEX_DIGITS "0123456789abcdef"
#endif

#if defined (_WIN32)
# if defined libzt_EXPORTS
#  define ZT_EXTERN __declspec(dllexport)
# else    /* SHARED_LIB */
#  define ZT_EXTERN __declspec(dllimport)
# endif
#else  /* _WIN32 */
# define ZT_EXTERN
#endif    /* _WIN32 */

#ifndef __GNUC__
#ifdef _WIN32 // SGC
# define __PRETTY_FUNCTION__ ""
#else /* _WIN32 */
# define __FUNCTION__        ""
# define __PRETTY_FUNCTION__ ""
#endif /* _WIN32 */
#endif

/* local defines */
#define ZT_FAIL 0
#define ZT_PASS 1


/* Taken Directly from sqlite3
 *
 * The ALWAYS and NEVER macros surround boolean expressions which
 * are intended to always be true or false, respectively.  Such
 * expressions could be omitted from the code completely.  But they
 * are included in a few cases in order to enhance the resilience
 * of SQLite to unexpected behavior - to make the code "self-healing"
 * or "ductile" rather than being "brittle" and crashing at the first
 * hint of unplanned behavior.
 *
 * In other words, ALWAYS and NEVER are added for defensive code.
 *
 * When doing coverage testing ALWAYS and NEVER are hard-coded to
 * be true and false so that the unreachable code then specify will
 * not be counted as untested code.
 */
#if defined(LIBZT_COVERAGE_TESTING)
# define ALWAYS(X)      (1)
# define NEVER(X)       (0)
#elif !defined(NDEBUG)
# define ALWAYS(X)      ((X) ? 1 : (assert(0), 0))
# define NEVER(X)       ((X) ? (assert(0), 1) : 0)
#else /* if defined(LIBZT_COVERAGE_TESTING) */
# define ALWAYS(X)      (X)
# define NEVER(X)       (X)
#endif /* if defined(LIBZT_COVERAGE_TESTING) */

#include <zt_macros.h>


#if defined(WIN32)
# include <windows.h>
#else
# include <errno.h>
#endif


#ifndef PRIsize_t
# define PRIsize_t "zu"
#endif

#include <zt_replace.h>
#endif /*_ZT_INTERNAL_H_*/
