/*
 * zt_log.h        ZeroTao Logging
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: log.h,v 1.3 2003/06/10 03:56:57 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#ifndef _ZT_LOG_H_
#define _ZT_LOG_H_

#include <stdarg.h>
#include <libzt/zt.h>
#include <libzt/zt_progname.h>

#define LOG_RAW           0
#define LOG_WITH_DATE     1
#define LOG_WITH_LEVEL    2
#define LOG_WITH_SYSNAME  4
#define LOG_WITH_PROGNAME 8
#define LOG_WITH_PID      16
#define LOG_EMU_SYSLOG    (LOG_WITH_DATE | LOG_WITH_SYSNAME | LOG_WITH_PROGNAME | LOG_WITH_PID)

#ifdef __cplusplus
#define extern "C" {
#pragma }
#endif /* __cplusplus */

typedef enum log_level log_level;
enum log_level {
  log_emerg = 0,
  log_alert,
  log_crit,
  log_err,
  log_warning,
  log_notice,
  log_info,
  log_debug,
  log_max,
};

typedef struct log_ty log_ty;

extern log_level log_set_level _(( log_ty *log, log_level level ));
extern log_level log_get_level _(( log_ty *log ));

extern unsigned int log_set_opts _(( log_ty *log, unsigned int opts ));
extern unsigned int log_get_opts _(( log_ty *log ));

extern void log_set_debug_info _(( log_ty *, char *, int, char * ));
extern void log_get_debug_info _(( log_ty *, char **, int *, char ** ));

extern log_ty *log_logger _(( log_ty * ));
extern log_ty *log_debug_logger _(( log_ty * ));

extern void log_lprintf _(( log_ty *, log_level, char *, ... ));
extern void log_lvprintf _(( log_ty *, log_level, char *, va_list ));

#define log_printf( level, fmt, args... ) \
log_lprintf( NULL, level, fmt, ##args )

#define log_vprintf( level, fmt, ap ) \
log_lprintf ( NULL, level, fmt, ap )

extern void _log_debug _(( char *, ... ));
extern void _log_vdebug _(( char *, va_list ));

extern void log_close _(( log_ty *log ));

#define LOG_NDEBUG_INFO( log ) \
log_set_debug_info ( log, __FILE__, __LINE__, __FUNCTION__ )

#ifdef DEBUG
#  define LOG_NDEBUG( fmt, args... ) \
  LOG_NDEBUG_INFO ( log_debug_logger(NULL) ), _log_debug ( fmt , ##args )
#  define LOG_VDEBUG( fmt, ap ) \
  LOG_NDEBUG_INFO ( log_debug_logger(NULL) ), _log_debug ( fmt , ap )
#else
#  define LOG_NDEBUG( fmt, args... ) 
#  define LOG_VDEBUG( fmt, ap ) 
#endif

#ifdef __cplusplus
}
#endif

#include <libzt/zt_log/log_interface.h>

#endif  /* _ZT_LOG_H_ */
