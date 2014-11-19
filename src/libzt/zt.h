#ifndef _ZT_H_
#define _ZT_H_

#include <libzt/zt_config.h>

#ifndef WIN32
# include <stdbool.h>
#else
# define _GNU_SOURCE
#include <libzt/zt_win32.h>
#endif /* WIN32 */

#include <stdio.h>
#ifndef PATH_MAX
# define PATH_MAX FILENAME_MAX
#endif

/* local defines */
#ifndef BEGIN_C_DECLS
#ifdef __cplusplus
# define BEGIN_C_DECLS      extern "C" {
# define END_C_DECLS      }
#else
# define BEGIN_C_DECLS
# define END_C_DECLS
#endif
#endif /* BEGIN_C_DECLS */

#define ZT_FAIL 0
#define ZT_PASS 1

extern char *memoryError;
#define zt_callocs(size, num)     (zt_calloc_p ((num), (size)))
#define zt_mallocs(size, num)     (zt_malloc_p ((num) * (size)))
#define zt_calloc(type, num)	((type *) zt_calloc_p ((num), sizeof(type)))
#define zt_malloc(type, num)	((type *) zt_malloc_p ((num) * sizeof(type)))
#define zt_realloc(type, p, num)	((type *) zt_realloc_p (((void_p)p), (num) * sizeof(type)))
#define zt_free(stale)   	  	       	  	   	 \
do {							 \
   if (stale) { free ((void_p) stale);  stale = 0; }	 \
} while(0)
#define zt_strdup zt_strdup_p

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
# define HAS_INLINE  1
#else
# define INLINE                                            /* no inline */
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(WIN32)
# define ZT_STR(x)    #x
# define ZT_CONC(x, y)    x##y
# define ZT_CONC3(x, y, z)   x##y##z
typedef void* void_p;
#elif defined(__STDC__) || defined(__STDC_HOSTED__)
# define ZT_STR(x)    # x
# define ZT_CONC(x, y)    x ## y
# define ZT_CONC3(x, y, z)   x ## y ## z
typedef void* void_p;
#else
# define ZT_STR(x)    "x"
# define ZT_CONC(x, y)    x /**/ y
# define ZT_CONC3(x, y, z)    x /**/ y /**/ z
typedef char* void_p;
#endif /* if defined(__STDC__) || defined(__STDC_HOSTED__) */

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
/* These entry points to the API are guaranteed to be functions */
#include <stdlib.h>

extern void * zt_calloc_p	(size_t num, size_t size);
extern void * zt_malloc_p	(size_t num);
extern void * zt_realloc_p (void * p, size_t num);
extern void zt_free_p (void * stale);
extern char *zt_strdup_p (const char *string);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include <libzt/zt_assert.h>
#include <libzt/zt_atexit.h>
#include <libzt/zt_buf.h>

#ifndef WIN32
# include <libzt/zt_daemon.h>
# include <libzt/zt_opts.h>
#endif

#include <libzt/zt_progname.h>
#include <libzt/zt_log.h>
#include <libzt/zt_time.h>
#include <libzt/zt_uuid.h>
#include <libzt/zt_cfg.h>
#include <libzt/zt_sha1.h>
#include <libzt/zt_mem.h>
#include <libzt/zt_format.h>
#include <libzt/zt_int.h>
#include <libzt/zt_base.h>

#ifdef WITH_THREADS
#include <libzt/zt_threads.h>
#endif

#ifndef ZT_WITHOUT_ADT
# include <libzt/zt_array.h>
# include <libzt/zt_list.h>
# include <libzt/zt_queue.h>
# include <libzt/zt_table.h>
# include <libzt/zt_bstream.h>
# include <libzt/zt_llist.h>
# include <libzt/zt_set.h>
# include <libzt/zt_tree.h>
# include <libzt/zt_hash.h>
# include <libzt/zt_ptr_array.h>
# include <libzt/zt_stack.h>
#endif /* ZT_WITHOUT_ADT */

#include <libzt/zt_cstr.h> /* uses ptr_array.h */
#include <libzt/zt_ez_mempool.h> /* needs zt_table.h */
#include <libzt/zt_ipv4_tbl.h> /* needs zt_table.h */

#include <libzt/zt_macros.h>
#include <libzt/zt_path.h>
#include <libzt/zt_random.h>

#ifdef ZT_WITH_GC
#include <libzt/zt_gc.h>
#endif /* ZT_WITH_GC */

#ifdef ZT_WITH_UNIT
#include <libzt/zt_unit.h>
#endif /* ZT_WITH_UNIT */

#endif /* _ZT_H_ */
