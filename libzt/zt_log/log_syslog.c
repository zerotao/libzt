/*
 * syslog.c        ZeroTao logging to syslog
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: syslog.c,v 1.3 2003/06/10 04:15:55 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#include <syslog.h>

#include "log_private.h"

static void destructor (log_ty *log)
{
	closelog();
	XFREE(log);
	return;
}

static void print(log_ty *log, log_level level, char *fmt, va_list ap)
{
	int syslog_level = 0;
	switch(level){
		case log_emerg:
			syslog_level = LOG_EMERG;
			break;
		case log_alert:
			syslog_level = LOG_ALERT;
			break;
		case log_crit:
			syslog_level = LOG_CRIT;
			break;
		case log_err:
			syslog_level = LOG_ERR;
			break;
		case log_warning:
			syslog_level = LOG_WARNING;
			break;
		case log_notice:
			syslog_level = LOG_NOTICE;
			break;
		case log_info:
			syslog_level = LOG_INFO;
			break;
		case log_debug:
			syslog_level = LOG_DEBUG;
			break;
		default:
			syslog_level = LOG_ERR;
			break;
	}
	vsyslog(syslog_level, fmt, ap);
}

/* component data */
static log_vtbl_ty vtbl = {
	sizeof(log_ty),
	destructor,
	print,
};

log_ty *
log_syslog(void)
{
	return log_syslog2(LOG_WITH_PID, LOG_USER);
}

log_ty *
log_syslog2(int opt, int facility)
{
	char	 *name = zt_progname(0, 0);
	int	  sysopts = 0;

	if(opt & LOG_WITH_PID)
	{
		sysopts |= LOG_PID;
	}
	
	openlog(name ? name : "Set name with progname call", sysopts, facility);
	return log_new(&vtbl, 0);
}

