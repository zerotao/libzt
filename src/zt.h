#ifndef _ZT_H_
#define _ZT_H_

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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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

#include <zt_assert.h>
#include <zt_atexit.h>

#ifndef WIN32
# include <zt_daemon.h>
# include <zt_opts.h>
#endif

#include <zt_progname.h>
#include <zt_log.h>
#include <zt_time.h>
#include <zt_uuid.h>
#include <zt_cfg.h>
#include <zt_sha1.h>
#include <zt_mem.h>
#include <zt_format.h>
#include <zt_int.h>

#ifdef WITH_THREADS
#include <zt_threads.h>
#endif

#ifndef ZT_WITHOUT_ADT
# include <zt_array.h>
# include <zt_list.h>
# include <zt_queue.h>
# include <zt_table.h>
# include <zt_bstream.h>
# include <zt_llist.h>
# include <zt_set.h>
# include <zt_tree.h>
# include <zt_hash.h>
# include <zt_ptr_array.h>
# include <zt_stack.h>
#endif /* ZT_WITHOUT_ADT */

#include <zt_cstr.h> /* uses ptr_array.h */
#include <zt_ez_mempool.h> /* needs zt_table.h */
#include <zt_ipv4_tbl.h> /* needs zt_table.h */

#ifdef ZT_WITH_GC
#include <zt_gc.h>
#endif /* ZT_WITH_GC */

#ifdef ZT_WITH_UNIT
#include <zt_unit.h>
#endif /* ZT_WITH_UNIT */

#endif /* _ZT_H_ */
