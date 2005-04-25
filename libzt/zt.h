/****h* libZT/Main
 * DESCRIPTION
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/

#ifndef _ZT_H_
#define _ZT_H_

#ifdef __cplusplus
# define BEGIN_C_DECLS	  extern "C" {
# define END_C_DECLS	  }
#else
# define BEGIN_C_DECLS
# define END_C_DECLS
#endif

#ifdef __GNUC__
#  ifndef const
#    define const	__const
#  endif
#  ifndef signed
#    define signed	__signed
#  endif
#  ifndef volatile
#    define volatile	__volatile
#  endif
#else
#  if defined(__STDC__) || defined(__STDC_HOSTED__)
#    undef  signed
#    define signed
#    undef  volatile
#    define volatile
#  endif
#endif

#if defined(__STDC__) || defined(__STDC_HOSTED__)
# define STR(x)		#x
# define CONC(x, y)	x##y
# define CONC3(x,y,z)   x##y##z
  typedef void* void_p;
#else
# define STR(x)		"x"
# define CONC(x, y)	x/**/y
# define CONC3(x, y)	x/**/y/**/z
  typedef char* void_p;
#endif

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS  0
#  define EXIT_FAILURE  1
#endif

#ifndef _
# if defined(__STDC__) || defined(__STDC_HOSTED__)
#  define _(x) x
# else
#  define _(x) ()
# endif
#endif

/* Convienience definitions */
#ifndef NULL
# define NULL ((void *) 0)
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

/* Unix Path */
#ifndef PATH_SEPERATOR
# define PATH_SEPERATOR '/'
#endif

/* Unix Shell */
#ifndef ENV_SEPERATOR
# define ENV_SEPERATOR ':'
#endif


/* Cover the cases where I am not using a GNU compiler */
#if __GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC__MINOR__ > 4)
# define NORETURN  __attribute__((noreturn))
# define CONST     __attribute__((const))
# define UNUSED    __attribute__((unused))
# define INLINE    inline
#else
# define NORETURN  
# define CONST     
# define UNUSED    
# define INLINE    
#endif

#ifndef __GNUC__
# define __FUNCTION__        ""
# define __PRETTY_FUNCTION__ ""
#endif

/* local defines */
#define ZT_FAIL 0
#define ZT_PASS 1

#include <libzt/zt_macros.h>
#include <libzt/zt_common.h>
#endif /*_ZT_H_*/
