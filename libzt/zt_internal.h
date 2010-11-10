/*
 * Filename: zt.h
 * Description: main header for libzt
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
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>

#ifdef HAVE_ERRNO_H
# include <errno.h>
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

#ifdef __cplusplus
# define BEGIN_C_DECLS      extern "C" {
# define END_C_DECLS      }
#else
# define BEGIN_C_DECLS
# define END_C_DECLS
#endif

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

#if defined(WIN32)
# define STR(x)    #x
# define CONC(x, y)    x##y
# define CONC3(x, y, z)   x##y##z
typedef void* void_p;
#elif defined(__STDC__) || defined(__STDC_HOSTED__)
# define STR(x)    # x
# define CONC(x, y)    x ## y
# define CONC3(x, y, z)   x ## y ## z
typedef void* void_p;
#else
# define STR(x)    "x"
# define CONC(x, y)    x /**/ y
# define CONC3(x, y, z)    x /**/ y /**/ z
typedef char* void_p;
#endif /* if defined(__STDC__) || defined(__STDC_HOSTED__) */

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS  0
#  define EXIT_FAILURE  1
#endif

#ifndef _
#  define _(x) x
#endif /* ifndef _ */

/************ Convienience Definitions  ************/

#ifndef NULL
# define NULL ((void *)0)
#endif

#ifndef FALSE
# define FALSE (0)
#endif

#ifndef TRUE
# define TRUE (!FALSE)
#endif

#ifndef WHITESPACE
# define WHITESPACE " \t"
#endif

#ifdef _WIN32
# ifndef PATH_SEPERATOR
#  define PATH_SEPERATOR "\\"
# endif
# ifndef ENV_SEPERATOR
#  define ENV_SEPERATOR ";"
# endif
#else /* _WIN32 */
/* Unix Path */
# ifndef PATH_SEPERATOR
#  define PATH_SEPERATOR "/"
# endif
# ifndef ENV_SEPERATOR
#  define ENV_SEPERATOR ":"
# endif
#endif /* _WIN32 else */

#ifndef HEX_DIGITS
# define HEX_DIGITS "0123456789abcdef"
#endif


/* Cover the cases where I am not using a GNU compiler */
#if (__GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC__MINOR__ > 4)) && (!defined(__STRICT_ANSI__) || __STRICT_ANSI__ == 0)
# define NORETURN  __attribute__((noreturn))
# define CONST     __attribute__((const))
# define UNUSED    __attribute__((unused))
# define FORMAT(x) __attribute__((format x))
#else
# define NORETURN
#ifndef WIN32
# define CONST
#endif
# define UNUSED
# define FORMAT(x)
#endif /* if (__GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC__MINOR__ > 4)) && (!defined(__STRICT_ANSI__) || __STRICT_ANSI__ == 0) */

#if (__GNUC__ == 4 && __GNUC_MINOR__ >= 3 && defined(__GNUC_STDC_INLINE__) && !defined(C99_INLINE)) || \
    (__APPLE_CC__ > 5400 && !defined(C99_INLINE) && __STDC_VERSION__ >= 199901L)
# define C99_INLINE 1
#endif

#if C99_INLINE || __GNUC_GNU_INLINE__
# define INLINE     inline
# define HAS_ININE  1
#else
# define INLINE                                            /* no inline */
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


#ifndef PATH_MAX
# define PATH_MAX FILENAME_MAX
#endif

#if defined(WIN32)
# include <windows.h>
#else
# include <errno.h>
#endif

extern char *memoryError;
#define XCALLOCS(size, num)     (xcalloc ((num), (size)))
#define XMALLOCS(size, num)     (xmalloc ((num) * (size)))
#define XCALLOC(type, num)	((type *) xcalloc ((num), sizeof(type)))
#define XMALLOC(type, num)	((type *) xmalloc ((num) * sizeof(type)))
#define XREALLOC(type, p, num)	((type *) xrealloc (((void_p)p), (num) * sizeof(type)))
#define XFREE(stale)   	  	       	  	   	 \
do {							 \
   if (stale) { free ((void_p) stale);  stale = 0; }	 \
} while(0)
#define XSTRDUP xstrdup

BEGIN_C_DECLS

/* These entry points to the API are guaranteed to be functions */

extern void_p xcalloc_p	(size_t num, size_t size);
extern void_p xmalloc_p	(size_t num);
extern void_p xrealloc_p (void_p p, size_t num);
extern void xfree_p (void_p stale);
extern char *xstrdup (const char *string);

END_C_DECLS

/* Let dmalloc provide mainstream `x' family mallocation API, or else
   provide our own which points to the `f_' family above.  Note that
   when using dmalloc, the functions wrapped by my `f_' API will be
   (correctly) redefined to use the dmalloc library. */

#ifndef WITH_DMALLOC
#  define xcalloc  xcalloc_p
#  define xmalloc  xmalloc_p
#  define xrealloc xrealloc_p
#  define xfree    xfree_p
#endif

#include <zt_replace.h>
#endif /*_ZT_INTERNAL_H_*/
