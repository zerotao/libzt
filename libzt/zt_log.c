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
 *    zt_log_logger
 *  SYNOPSIS
 *    zt_log_logger zt_log_ty *log)
 *  SOURCE
 */

zt_log_ty *
zt_log_logger(zt_log_ty *log)
{
    static zt_log_ty *log_log_ptr = NULL;
    static int forced = 0;

    if(!log && !log_log_ptr){
        log_log_ptr = zt_log_stderr( ZT_LOG_WITH_LEVEL );
        forced = 1;
    }

    if(log) {
        if(forced)
            zt_log_close(log_log_ptr), forced = 0;
        log_log_ptr = log;
    }

    return log_log_ptr;
}
/************ zt_log_logger */

/****f* zt_log.c/log_debug_logger
 *  NAME
 *    zt_log_debug_logger
 *  SYNOPSIS
 *    zt_log_debug_logger zt_log_ty *log)
 *  SOURCE
 */
zt_log_ty *
zt_log_debug_logger(zt_log_ty *log)
{
    static zt_log_ty *log_debug_ptr = NULL;
    static int forced = 0;

    if(((!log) && (!log_debug_ptr))){
        log_debug_ptr = zt_log_stderr( ZT_LOG_WITH_LEVEL );
        forced = 1;
    }

    if(log) {
        if(forced)
            zt_log_close(log_debug_ptr), forced = 0;
        log_debug_ptr = log;
    }

    return log_debug_ptr;
}
/************ zt_log_debug_logger */

/****f* zt_log.c/log_set_level
 *  NAME
 *    zt_log_set_level
 *  SYNOPSIS
 *    zt_log_set_level zt_log_ty *log, zt_log_level level)
 *  SOURCE
 */
zt_log_level
zt_log_set_level(zt_log_ty *log, zt_log_level level)
{
    zt_log_level olevel;
    olevel = log->level;
    log->level = level;
    return olevel;
}
/************ zt_log_set_level */

/****f* zt_log.c/log_get_level
 *  NAME
 *    zt_log_get_level
 *  SYNOPSIS
 *    zt_log_get_level zt_log_ty *log)
 *  SOURCE
 */
zt_log_level
zt_log_get_level(zt_log_ty *log)
{
    return log->level;
}\
/************ zt_log_get_level */

/****f* zt_log.c/log_set_opts
 *  NAME
 *    zt_log_set_opts
 *  SYNOPSIS
 *    zt_log_set_opts zt_log_ty *log, unsigned int opts)
 *  SOURCE
 */
unsigned int
zt_log_set_opts(zt_log_ty *log, unsigned int opts)
{
    unsigned int oopts;
  
    oopts = log->opts;
    log->opts = opts;
    return oopts;
}
/************ zt_log_set_opts */

/****f* zt_log.c/log_get_opts
 *  NAME
 *    zt_log_get_opts
 *  SYNOPSIS
 *    zt_log_get_optszt_log_ty *log)
 *  SOURCE
 */
unsigned int
zt_log_get_opts(zt_log_ty *log)
{
    return log->opts;
}
/************ zt_log_get_opts */

/****f* zt_log.c/log_set_debug_info
 *  NAME
 *    zt_log_set_debug_info
 *  SYNOPSIS
 *    zt_log_set_debug_info zt_log_ty *log, const char *file, int line, const char *func)
 *  SOURCE
 */
void
zt_log_set_debug_info(zt_log_ty *log, const char *file, int line, const char *func)
{
	if(!log)
		log = zt_log_logger(NULL);

	log->file = (char *)file;
	log->line = line;
	log->function = (char *)func;
}
/************ zt_log_set_debug_info */

/****f* zt_log.c/log_get_debug_info
 *  NAME
 *    zt_log_get_debug_info
 *  SYNOPSIS
 *    zt_log_get_debug_info zt_log_ty *log, char **file, int *line, char **func)
 *  SOURCE
 */
void
zt_log_get_debug_info(zt_log_ty *log, char **file, int *line, char **func)
{
	if(!log)
		log = zt_log_logger(NULL);
	
	*file = log->file;
	*line = log->line;
	*func = log->function;
}
/************ zt_log_get_debug_info */

/****f* zt_log.c/log_lprintf
 *  NAME
 *    zt_log_lprintf
 *  SYNOPSIS
 *    zt_log_lprintf zt_log_ty *log, zt_log_level level, char *fmt, ...)
 *  SOURCE
 */
void
zt_log_lprintf(zt_log_ty *log, zt_log_level level, char *fmt, ...)
{
    va_list ap;

    if(!log)
        log = zt_log_logger(NULL);
  
    if(level > log->level)
        return;
    va_start(ap, fmt);
    zt_log_lvprintf(log, level, fmt, ap);
    va_end(ap);
}
/************ zt_log_lprintf */

/****f* zt_log.c/log_lvprintf
 *  NAME
 *    zt_log_lvprintf
 *  SYNOPSIS
 *    zt_log_lvprintf zt_log_ty *log, zt_log_level level, char *fmt, va_list ap)
 *  SOURCE
 */

void
zt_log_lvprintf(zt_log_ty *log, zt_log_level level, char *fmt, va_list ap)
{
    if(!log)
        log = zt_log_logger(NULL);

    if(level > log->level)
        return;
    if(log->vtbl->print)
        log->vtbl->print(log, level, fmt, ap);
    zt_log_set_debug_info(log, NULL, -1, NULL);
}
/************ zt_log_lvprintf */

/****f* zt_log.c/log_lstrerror
 *  NAME
 *    zt_log_lstrerror
 *  SYNOPSIS
 *    zt_log_lstrerror zt_log_ty *log, zt_log_level level, int errnum, char *fmt, ...)
 *  SOURCE
 */
void
zt_log_lstrerror(zt_log_ty *log, zt_log_level level, int errnum, char *fmt, ...)
{
	va_list		  ap;
	int			  llen;
	char		* nfmt;

	if(!log)
		log = zt_log_logger(NULL);

	if(level > log->level)
		return;
	
	llen = strlen(fmt);


	nfmt = (char *)alloca(llen + 256);
	memcpy(nfmt, fmt, llen);
	nfmt[llen] = ':';
	nfmt[llen+1] = ' ';
	
	strerror_r(errnum, nfmt+(llen+2), 255-2);
	
	va_start(ap, fmt);
	zt_log_lvprintf(log, level, nfmt, ap);
	va_end(ap);
	
	/* free(nfmt); */
}
/************ zt_log_lstrerror */

/****f* zt_log.c/_log_debug
 *  NAME
 *    _log_debug
 *  SYNOPSIS
 *    _log_debug (char *fmt, ...)
 *  SOURCE
 */
void
_zt_log_debug (char *fmt, ...)
{
    va_list ap;
    zt_log_level level = zt_log_debug;
    zt_log_ty *log = zt_log_debug_logger(NULL);
  
    if(level > log->level)
        return;
    va_start(ap, fmt);
    zt_log_lvprintf(log, level, fmt, ap);
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
void
_zt_log_vdebug (char *fmt, va_list ap)
{
    zt_log_level level = zt_log_debug;
    zt_log_ty *log = zt_log_debug_logger(NULL);
  
    if(level > log->level)
        return;
    if(log->vtbl->print)
        log->vtbl->print(log, level, fmt, ap);
    zt_log_set_debug_info(log, NULL, -1, NULL);
}
/************ _log_vdebug */

/****f* zt_log.c/log_close
 *  NAME
 *    zt_log_close
 *  SYNOPSIS
 *    zt_log_closezt_log_ty *log)
 *  SOURCE
 */
void
zt_log_close(zt_log_ty *log)
{
    if(log->vtbl->destructor)
        log->vtbl->destructor(log);
}
/************ zt_log_close */
