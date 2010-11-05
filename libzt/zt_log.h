/*!
 * Filename: zt_log.h
 * Description: generic logging interface
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_LOG_H_
#define _ZT_LOG_H_

#include <stdarg.h>
#include <zt_internal.h>
#include <zt_progname.h>

#define ZT_LOG_RAW           0
#define ZT_LOG_WITH_DATE     1
#define ZT_LOG_WITH_LEVEL    2
#define ZT_LOG_WITH_SYSNAME  4
#define ZT_LOG_WITH_PROGNAME 8
#define ZT_LOG_WITH_PID      16
#define ZT_LOG_EMU_SYSLOG    (ZT_LOG_WITH_DATE | ZT_LOG_WITH_SYSNAME | ZT_LOG_WITH_PROGNAME | ZT_LOG_WITH_PID)

BEGIN_C_DECLS

enum zt_log_level {
    zt_log_emerg = 0,   /* Panic - total system failure (application should exit now) */
    zt_log_alert,                                          /* A condition that should be corrected immediately, such as a corrupted system database. */
    zt_log_crit,                                           /* Critical conditions, e.g., hard device errors. */
    zt_log_err,                                            /* Program Errors (eg. could not find file "foo") */
    zt_log_warning,                                        /* Program Warnings (eg. val X is too large - rounding) */
    zt_log_notice,                                         /* Not an error but user correctable (eg. could not find config file X, switching to defaults) */
    zt_log_info,                                           /* Informational Messages (eg. we have just passed chattanooga) */
    zt_log_debug,                                          /* Messages that contain information normally of use only when debugging a program. */
    zt_log_max
};
typedef enum zt_log_level zt_log_level;

typedef struct zt_log_ty zt_log_ty;

extern zt_log_level zt_log_set_level( zt_log_ty *log, zt_log_level level );
extern zt_log_level zt_log_get_level( zt_log_ty *log );

extern unsigned int zt_log_set_opts( zt_log_ty *log, unsigned int opts );
extern unsigned int zt_log_get_opts( zt_log_ty *log );

extern void zt_log_set_debug_info( zt_log_ty *, const char *, int, const char * );
extern void zt_log_get_debug_info( zt_log_ty *, char **, int *, char ** );

extern zt_log_ty *zt_log_logger( zt_log_ty * );
extern zt_log_ty *zt_log_debug_logger( zt_log_ty * );

extern void zt_log_lprintf( zt_log_ty *, zt_log_level, char *, ... ) FORMAT((printf, 3, 4));
extern void zt_log_lstrerror( zt_log_ty *, zt_log_level, int, char *, ... ) FORMAT((printf, 4, 5));
extern void zt_log_lvprintf( zt_log_ty *, zt_log_level, char *, va_list ) FORMAT((printf, 3, 0));

/* zt_log_strerror(level, errnum, fmt, ...) */
#define zt_log_strerror( level, errnum, ... ) \
    zt_log_lstrerror( 0, level, errnum, __VA_ARGS__ )

/* zt_log_printf(level, fmt, ...) */
#define zt_log_printf( level, ... ) \
    zt_log_lprintf( 0, level, __VA_ARGS__ )


#define zt_log_vprintf( level, fmt, ap ) \
    zt_log_lvprintf( NULL, level, fmt, ap )

extern void _zt_log_debug( char *, ... ) FORMAT((printf, 1, 2));
extern void _zt_log_vdebug( char *, va_list ) FORMAT((printf, 1, 0));

extern void zt_log_close( zt_log_ty *log );

#define ZT_LOG_DEBUG_INFO( log ) \
    zt_log_set_debug_info( log, __FILE__, __LINE__, __FUNCTION__ )

/* zt_log_dprintf(level, fmt, ...) */
#define zt_log_dprintf( level, ... ) \
    ZT_LOG_DEBUG_INFO( NULL ); zt_log_printf( level, __VA_ARGS__ )

#define zt_log_dvprintf( level, fmt, ap ) \
    zt_log_vprintf( level, fmt, ap)

/* normal debugging logging */
#ifdef DEBUG
/* LOG_XDEBUG(fmt, ...) */
#  define ZT_LOG_XDEBUG(... ) \
    ZT_LOG_DEBUG_INFO( zt_log_debug_logger(NULL) ); _zt_log_debug( __VA_ARGS__ )

#  define LOG_XDEBUGV( fmt, ap ) \
    ZT_LOG_DEBUG_INFO( zt_log_debug_logger(NULL) ); _zt_log_debug( fmt, ap )
#else /* ifdef DEBUG */
/* LOG_XDEBUG(fmt, ...) */
#  define ZT_LOG_XDEBUG(... )
#  define ZT_LOG_XDEBUGV( fmt, ap )
#endif /* ifdef DEBUG */

/* associated with assert */
#ifndef NDEBUG
/* LOG_NDEBUG(fnt, ...) */
#  define ZT_LOG_NDEBUG(... ) \
    ZT_LOG_DEBUG_INFO( zt_log_debug_logger(NULL) ); _zt_log_debug( __VA_ARGS__ )

#  define ZT_LOG_NDEBUGV( fmt, ap ) \
    ZT_LOG_DEBUG_INFO( zt_log_debug_logger(NULL) ); _zt_log_debug( fmt, ap )
#else /* ifndef NDEBUG */
/* LOG_NDEBUG(fmt, ...) */
#  define ZT_LOG_NDEBUG(... )
#  define ZT_LOG_NDEBUGV( fmt, ap )
#endif /* ifndef NDEBUG */

END_C_DECLS
#include <zt_log/log_interface.h>

#endif  /* _ZT_LOG_H_ */
