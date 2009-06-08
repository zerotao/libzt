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

/****d* Logging/LOG_RAW
 *  NAME
 *    LOG_RAW
 *  SOURCE
 */
#define LOG_RAW           0
/***** LOG_RAW */

/****d* Logging/LOG_WITH_DATE
 *  NAME
 *    LOG_WITH_DATE
 *  SOURCE
 */
#define LOG_WITH_DATE     1
/***** LOG_WITH_DATE */

/****d* Logging/LOG_WITH_LEVEL
 *  NAME
 *    LOG_WITH_LEVEL
 *  SOURCE
 */
#define LOG_WITH_LEVEL    2
/***** LOG_WITH_LEVEL */

/****d* Logging/LOG_WITH_SYSNAME
 *  NAME
 *    LOG_WITH_SYSNAME
 *  SOURCE
 */
#define LOG_WITH_SYSNAME  4
/***** LOG_WITH_SYSNAME */

/****d* Logging/LOG_WITH_PROGNAME
 *  NAME
 *    LOG_WITH_PROGNAME
 *  SOURCE
 */
#define LOG_WITH_PROGNAME 8
/***** LOG_WITH_PROGNAME */

/****d* Logging/LOG_WITH_PID
 *  NAME
 *    LOG_WITH_PID
 *  SOURCE
 */
#define LOG_WITH_PID      16
/***** LOG_WITH_PID */

/****d* Logging/LOG_EMU_SYSLOG
 *  NAME
 *    LOG_EMU_SYSLOG
 *  SOURCE
 */
#define LOG_EMU_SYSLOG    (LOG_WITH_DATE | LOG_WITH_SYSNAME | LOG_WITH_PROGNAME | LOG_WITH_PID)
/***** LOG_EMU_SYSLOG */
BEGIN_C_DECLS

enum log_level {
  log_emerg = 0,
  log_alert,
  log_crit,
  log_err,
  log_warning,
  log_notice,
  log_info,
  log_debug,
  log_max
};
typedef enum log_level log_level;

typedef struct log_ty log_ty;

extern log_level log_set_level ( log_ty *log, log_level level );
extern log_level log_get_level ( log_ty *log );

extern unsigned int log_set_opts ( log_ty *log, unsigned int opts );
extern unsigned int log_get_opts ( log_ty *log );

extern void log_set_debug_info ( log_ty *, const char *, int, const char * );
extern void log_get_debug_info ( log_ty *, char **, int *, char ** );

extern log_ty *log_logger ( log_ty * );
extern log_ty *log_debug_logger ( log_ty * );

extern void log_lprintf ( log_ty *, log_level, char *, ... )FORMAT((printf, 3, 4));
extern void log_lstrerror ( log_ty *, log_level, int, char *, ... )FORMAT((printf, 4, 5));
extern void log_lvprintf ( log_ty *, log_level, char *, va_list )FORMAT((printf, 3, 0));

/* log_strerror(level, errnum, fmt, ...) */
#define log_strerror( level, errnum, ... )	\
	log_lstrerror( 0, level, errnum, __VA_ARGS__ )

/* log_printf(level, fmt, ...) */
#define log_printf( level, ... ) \
	log_lprintf( 0, level, __VA_ARGS__ )


#define log_vprintf( level, fmt, ap ) \
	log_lvprintf ( NULL, level, fmt, ap )

extern void _log_debug ( char *, ... )FORMAT((printf, 1, 2));
extern void _log_vdebug ( char *, va_list )FORMAT((printf, 1, 0));

extern void log_close ( log_ty *log );

#define LOG_DEBUG_INFO( log ) \
	log_set_debug_info ( log, __FILE__, __LINE__, __FUNCTION__ )

/* log_dprintf(level, fmt, ...) */
#define log_dprintf( level, ... )\
	LOG_DEBUG_INFO ( NULL ); log_printf( level, __VA_ARGS__ )

#define log_dvprintf( level, fmt, ap ) \
	log_vprintf( level, fmt, ap)

/* normal debugging logging */
#ifdef DEBUG
/* LOG_XDEBUG(fmt, ...) */
#  define LOG_XDEBUG( ... ) \
	LOG_DEBUG_INFO ( log_debug_logger(NULL) ); _log_debug ( __VA_ARGS__ )

#  define LOG_XDEBUGV( fmt, ap ) \
	LOG_DEBUG_INFO ( log_debug_logger(NULL) ); _log_debug ( fmt , ap )
#else
/* LOG_XDEBUG(fmt, ...) */
#  define LOG_XDEBUG( ... ) 
#  define LOG_XDEBUGV( fmt, ap ) 
#endif

/* associated with assert */
#ifndef NDEBUG
/* LOG_NDEBUG(fnt, ...) */
#  define LOG_NDEBUG( ... ) \
	LOG_DEBUG_INFO ( log_debug_logger(NULL) ); _log_debug ( __VA_ARGS__ )

#  define LOG_NDEBUGV( fmt, ap ) \
	LOG_DEBUG_INFO ( log_debug_logger(NULL) ); _log_debug ( fmt , ap )
#else
/* LOG_NDEBUG(fmt, ...) */
#  define LOG_NDEBUG( ... )
#  define LOG_NDEBUGV( fmt, ap ) 
#endif

END_C_DECLS
#include <libzt/zt_log/log_interface.h>

#endif  /* _ZT_LOG_H_ */
