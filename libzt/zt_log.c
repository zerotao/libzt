/****h* libZT/Logging/zt_log.c
 *  NAME
 *    zt_log.c
 *  DESCRIPTION
 *    functions for logging
 *  COPYRIGHT
 *   Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *********/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "zt_log.h"
#include "zt_log/log_private.h"

/****f* zt_log.c/log_logger
 *  NAME
 *    log_logger
 *  SYNOPSIS
 *    log_logger (log_ty *log)
 *  SOURCE
 */

log_ty *log_logger (log_ty *log)
{
  static log_ty *log_log_ptr = NULL;
  static int forced = 0;

  if(!log && !log_log_ptr){
    log_log_ptr = log_stderr( LOG_WITH_LEVEL );
    forced = 1;
  }

  if(log) {
    if(forced)
      log_close(log_log_ptr), forced = 0;
    log_log_ptr = log;
  }

  return log_log_ptr;
}
/************ log_logger */

/****f* zt_log.c/log_debug_logger
 *  NAME
 *    log_debug_logger
 *  SYNOPSIS
 *    log_debug_logger (log_ty *log)
 *  SOURCE
 */
log_ty *log_debug_logger (log_ty *log)
{
  static log_ty *log_debug_ptr = NULL;
  static int forced = 0;

  if(((!log) && (!log_debug_ptr))){
    log_debug_ptr = log_stderr( LOG_WITH_LEVEL );
    forced = 1;
  }

  if(log) {
    if(forced)
      log_close(log_debug_ptr), forced = 0;
    log_debug_ptr = log;
  }

  return log_debug_ptr;
}
/************ log_debug_logger */

/****f* zt_log.c/log_set_level
 *  NAME
 *    log_set_level
 *  SYNOPSIS
 *    log_set_level (log_ty *log, log_level level)
 *  SOURCE
 */
log_level log_set_level (log_ty *log, log_level level)
{
  log_level olevel;
  olevel = log->level;
  log->level = level;
  return olevel;
}
/************ log_set_level */

/****f* zt_log.c/log_get_level
 *  NAME
 *    log_get_level
 *  SYNOPSIS
 *    log_get_level (log_ty *log)
 *  SOURCE
 */
log_level log_get_level (log_ty *log)
{
  return log->level;
}\
/************ log_get_level */

/****f* zt_log.c/log_set_opts
 *  NAME
 *    log_set_opts
 *  SYNOPSIS
 *    log_set_opts (log_ty *log, unsigned int opts)
 *  SOURCE
*/
unsigned int log_set_opts (log_ty *log, unsigned int opts)
{
  unsigned int oopts;
  
  oopts = log->opts;
  log->opts = opts;
  return oopts;
}
/************ log_set_opts */

/****f* zt_log.c/log_get_opts
 *  NAME
 *    log_get_opts
 *  SYNOPSIS
 *    log_get_opts(log_ty *log)
 *  SOURCE
 */
unsigned int log_get_opts(log_ty *log)
{
  return log->opts;
}
/************ log_get_opts */

/****f* zt_log.c/log_set_debug_info
 *  NAME
 *    log_set_debug_info
 *  SYNOPSIS
 *    log_set_debug_info (log_ty *log, const char *file, int line, const char *func)
 *  SOURCE
 */
void log_set_debug_info (log_ty *log, const char *file, int line, const char *func)
{
	if(!log)
		log = log_logger(NULL);

	log->file = (char *)file;
	log->line = line;
	log->function = (char *)func;
}
/************ log_set_debug_info */

/****f* zt_log.c/log_get_debug_info
 *  NAME
 *    log_get_debug_info
 *  SYNOPSIS
 *    log_get_debug_info (log_ty *log, char **file, int *line, char **func)
 *  SOURCE
*/
void log_get_debug_info (log_ty *log, char **file, int *line, char **func)
{
	if(!log)
		log = log_logger(NULL);
	
	*file = log->file;
	*line = log->line;
	*func = log->function;
}
/************ log_get_debug_info */

/****f* zt_log.c/log_lprintf
 *  NAME
 *    log_lprintf
 *  SYNOPSIS
 *    log_lprintf (log_ty *log, log_level level, char *fmt, ...)
 *  SOURCE
 */
void log_lprintf (log_ty *log, log_level level, char *fmt, ...)
{
  va_list ap;

  if(!log)
    log = log_logger(NULL);
  
  if(level > log->level)
    return;
  va_start(ap, fmt);
  log_lvprintf(log, level, fmt, ap);
  va_end(ap);
}
/************ log_lprintf */

/****f* zt_log.c/log_lvprintf
 *  NAME
 *    log_lvprintf
 *  SYNOPSIS
 *    log_lvprintf (log_ty *log, log_level level, char *fmt, va_list ap)
 *  SOURCE
*/

void log_lvprintf (log_ty *log, log_level level, char *fmt, va_list ap)
{
  if(!log)
    log = log_logger(NULL);

  if(level > log->level)
    return;
  if(log->vtbl->print)
    log->vtbl->print(log, level, fmt, ap);
  log_set_debug_info(log, NULL, -1, NULL);
}
/************ log_lvprintf */

/****f* zt_log.c/log_lstrerror
 *  NAME
 *    log_lstrerror
 *  SYNOPSIS
 *    log_lstrerror (log_ty *log, log_level level, int errnum, char *fmt, ...)
 *  SOURCE
 */
void log_lstrerror (log_ty *log, log_level level, int errnum, char *fmt, ...)
{
	va_list		  ap;
	int			  llen;
	char		* nfmt;

	if(!log)
		log = log_logger(NULL);

	if(level > log->level)
		return;
	
	llen = strlen(fmt);


	nfmt = (char *)alloca(llen + 256);
	memcpy(nfmt, fmt, llen);
	nfmt[llen] = ':';
	nfmt[llen+1] = ' ';
	
	strerror_r(errnum, nfmt+(llen+2), 255-2);
	
	va_start(ap, fmt);
	log_lvprintf(log, level, nfmt, ap);
	va_end(ap);
	
	//free(nfmt);
}
/************ log_lstrerror */

/****f* zt_log.c/_log_debug
 *  NAME
 *    _log_debug
 *  SYNOPSIS
 *    _log_debug (char *fmt, ...)
 *  SOURCE
 */
void _log_debug (char *fmt, ...)
{
  va_list ap;
  log_level level = log_debug;
  log_ty *log = log_debug_logger(NULL);
  
  if(level > log->level)
    return;
  va_start(ap, fmt);
  log_lvprintf(log, level, fmt, ap);
  va_end(ap);
}
/************ _log_debug */

/****f* zt_log.c/_log_vdebug
 *  NAME
 *    _log_vdebug
 *  SYNOPSIS
 *    _log_vdebug (char *fmt, va_list ap)
 *  SOURCE
 */
void _log_vdebug (char *fmt, va_list ap)
{
  log_level level = log_debug;
  log_ty *log = log_debug_logger(NULL);
  
  if(level > log->level)
    return;
  if(log->vtbl->print)
    log->vtbl->print(log, level, fmt, ap);
  log_set_debug_info(log, NULL, -1, NULL);
}
/************ _log_vdebug */

/****f* zt_log.c/log_close
 *  NAME
 *    log_close
 *  SYNOPSIS
 *    log_close(log_ty *log)
 *  SOURCE
 */
void log_close(log_ty *log)
{
  if(log->vtbl->destructor)
    log->vtbl->destructor(log);
}
/************ log_close */
