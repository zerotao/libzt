/****h* libZT/Logging/zt_log.h
 *  NAME
 *    zt_log.h
 *  DESCRIPTION
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *********/
#ifndef _ZT_LOG_H_
#define _ZT_LOG_H_

#include <stdarg.h>
#include <libzt/zt.h>
#include <libzt/zt_progname.h>

/****d* Logging/ZT_LOG_RAW
 *  NAME
 *    ZT_LOG_RAW
 *  SOURCE
 */
#define ZT_LOG_RAW           0
/***** ZT_LOG_RAW */

/****d* Logging/ZT_LOG_WITH_DATE
 *  NAME
 *    ZT_LOG_WITH_DATE
 *  SOURCE
 */
#define ZT_LOG_WITH_DATE     1
/***** ZT_LOG_WITH_DATE */

/****d* Logging/ZT_LOG_WITH_LEVEL
 *  NAME
 *    ZT_LOG_WITH_LEVEL
 *  SOURCE
 */
#define ZT_LOG_WITH_LEVEL    2
/***** ZT_LOG_WITH_LEVEL */

/****d* Logging/ZT_LOG_WITH_SYSNAME
 *  NAME
 *    ZT_LOG_WITH_SYSNAME
 *  SOURCE
 */
#define ZT_LOG_WITH_SYSNAME  4
/***** ZT_LOG_WITH_SYSNAME */

/****d* Logging/ZT_LOG_WITH_PROGNAME
 *  NAME
 *    ZT_LOG_WITH_PROGNAME
 *  SOURCE
 */
#define ZT_LOG_WITH_PROGNAME 8
/***** ZT_LOG_WITH_PROGNAME */

/****d* Logging/ZT_LOG_WITH_PID
 *  NAME
 *    ZT_LOG_WITH_PID
 *  SOURCE
 */
#define ZT_LOG_WITH_PID      16
/***** ZT_LOG_WITH_PID */

/****d* Logging/ZT_LOG_EMU_SYSLOG
 *  NAME
 *    ZT_LOG_EMU_SYSLOG
 *  SOURCE
 */
#define ZT_LOG_EMU_SYSLOG    (ZT_LOG_WITH_DATE | ZT_LOG_WITH_SYSNAME | ZT_LOG_WITH_PROGNAME | ZT_LOG_WITH_PID)
/***** ZT_LOG_EMU_SYSLOG */
BEGIN_C_DECLS

enum zt_log_level {
  zt_log_emerg = 0,
  zt_log_alert,
  zt_log_crit,
  zt_log_err,
  zt_log_warning,
  zt_log_notice,
  zt_log_info,
  zt_log_debug,
  zt_log_max
};
typedef enum zt_log_level zt_log_level;

typedef struct zt_log_ty zt_log_ty;

extern zt_log_level zt_log_set_level ( zt_log_ty *log, zt_log_level level );
extern zt_log_level zt_log_get_level ( zt_log_ty *log );

extern unsigned int zt_log_set_opts ( zt_log_ty *log, unsigned int opts );
extern unsigned int zt_log_get_opts ( zt_log_ty *log );

extern void zt_log_set_debug_info ( zt_log_ty *, const char *, int, const char * );
extern void zt_log_get_debug_info ( zt_log_ty *, char **, int *, char ** );

extern zt_log_ty *zt_log_logger ( zt_log_ty * );
extern zt_log_ty *zt_log_debug_logger ( zt_log_ty * );

extern void zt_log_lprintf ( zt_log_ty *, zt_log_level, char *, ... )FORMAT((printf, 3, 4));
extern void zt_log_lstrerror ( zt_log_ty *, zt_log_level, int, char *, ... )FORMAT((printf, 4, 5));
extern void zt_log_lvprintf ( zt_log_ty *, zt_log_level, char *, va_list )FORMAT((printf, 3, 0));

/* zt_log_strerror(level, errnum, fmt, ...) */
#define zt_log_strerror( level, errnum, ... )	\
	zt_log_lstrerror( 0, level, errnum, __VA_ARGS__ )

/* zt_log_printf(level, fmt, ...) */
#define zt_log_printf( level, ... ) \
	zt_log_lprintf( 0, level, __VA_ARGS__ )


#define zt_log_vprintf( level, fmt, ap ) \
	zt_log_lvprintf ( NULL, level, fmt, ap )

extern void _zt_log_debug ( char *, ... )FORMAT((printf, 1, 2));
extern void _zt_log_vdebug ( char *, va_list )FORMAT((printf, 1, 0));

extern void zt_log_close ( zt_log_ty *log );

#define ZT_LOG_DEBUG_INFO( log ) \
	zt_log_set_debug_info ( log, __FILE__, __LINE__, __FUNCTION__ )

/* zt_log_dprintf(level, fmt, ...) */
#define zt_log_dprintf( level, ... )\
	ZT_LOG_DEBUG_INFO ( NULL ); zt_log_printf( level, __VA_ARGS__ )

#define zt_log_dvprintf( level, fmt, ap ) \
	zt_log_vprintf( level, fmt, ap)

/* normal debugging logging */
#ifdef DEBUG
/* LOG_XDEBUG(fmt, ...) */
#  define ZT_LOG_XDEBUG( ... ) \
	ZT_LOG_DEBUG_INFO ( zt_log_debug_logger(NULL) ); _zt_log_debug ( __VA_ARGS__ )

#  define LOG_XDEBUGV( fmt, ap ) \
	ZT_LOG_DEBUG_INFO ( zt_log_debug_logger(NULL) ); _zt_log_debug ( fmt , ap )
#else
/* LOG_XDEBUG(fmt, ...) */
#  define ZT_LOG_XDEBUG( ... ) 
#  define ZT_LOG_XDEBUGV( fmt, ap ) 
#endif

/* associated with assert */
#ifndef NDEBUG
/* LOG_NDEBUG(fnt, ...) */
#  define ZT_LOG_NDEBUG( ... ) \
	ZT_LOG_DEBUG_INFO ( zt_log_debug_logger(NULL) ); _zt_log_debug ( __VA_ARGS__ )

#  define ZT_LOG_NDEBUGV( fmt, ap ) \
	ZT_LOG_DEBUG_INFO ( zt_log_debug_logger(NULL) ); _zt_log_debug ( fmt , ap )
#else
/* LOG_NDEBUG(fmt, ...) */
#  define ZT_LOG_NDEBUG( ... )
#  define ZT_LOG_NDEBUGV( fmt, ap ) 
#endif

END_C_DECLS
#include <libzt/zt_log/log_interface.h>

#endif  /* _ZT_LOG_H_ */
