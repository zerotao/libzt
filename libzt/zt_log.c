/*
 * zt_log.c        Zerotao functions for logging
 *
 * Copyright (C) 2000, 2002, 2004, 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: log.c,v 1.1 2002/11/10 23:36:59 jshiffer Exp $
 */

/*
 * Description: 
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "zt_log.h"
#include "zt_log/log_private.h"

log_ty *
log_logger ( log )
     log_ty *log;
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

log_ty *
log_debug_logger ( log )
     log_ty *log;
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

log_level
log_set_level ( log, level )
     log_ty *log;
     log_level level;
{
  log_level olevel;
  olevel = log->level;
  log->level = level;
  return olevel;
}

log_level
log_get_level ( log )
     log_ty *log;
{
  return log->level;
}

unsigned int 
log_set_opts ( log, opts )
     log_ty *log;
     unsigned int opts;
{
  unsigned int oopts;
  
  oopts = log->opts;
  log->opts = opts;
  return oopts;
}

unsigned int
log_get_opts( log )
     log_ty *log;
{

  return log->opts;
}

void
log_set_debug_info ( log, file, line, func )
     log_ty *log;
     const char *file;
     int line;
     const char *func;
{
	if(!log)
		log = log_logger(NULL);

	log->file = (char *)file;
	log->line = line;
	log->function = (char *)func;
}

void
log_get_debug_info ( log, file, line, func )
     log_ty *log;
     char **file;
     int *line;
     char **func;
{
	if(!log)
		log = log_logger(NULL);
	
	*file = log->file;
	*line = log->line;
	*func = log->function;
}

void
log_lprintf ( log, level, fmt )
     log_ty *log;
     log_level level;
     char *fmt;
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

void
log_lvprintf ( log, level, fmt, ap )
     log_ty *log;
     log_level level;
     char *fmt;
     va_list ap;
{
  if(!log)
    log = log_logger(NULL);

  if(level > log->level)
    return;
  if(log->vtbl->print)
    log->vtbl->print(log, level, fmt, ap);
  log_set_debug_info(log, NULL, -1, NULL);
}

void
_log_debug ( fmt )
     char *fmt;
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

void
_log_vdebug ( fmt, ap )
     char *fmt;
     va_list ap;
{
  log_level level = log_debug;
  log_ty *log = log_debug_logger(NULL);
  
  if(level > log->level)
    return;
  if(log->vtbl->print)
    log->vtbl->print(log, level, fmt, ap);
  log_set_debug_info(log, NULL, -1, NULL);
}


void
log_close( log )
     log_ty *log;
{
  if(log->vtbl->destructor)
    log->vtbl->destructor(log);
}
