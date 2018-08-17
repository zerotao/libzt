/*
 * interface.c        logging interfaces
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: interface.h,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

#ifndef _LOG_INTERFACE_H_
#define _LOG_INTERFACE_H_

#include <libzt/zt_log.h>
#include <libzt/zt_config.h>

#include <libzt/zt_log/log_stderr.h>
#include <libzt/zt_log/log_file.h>
#ifdef HAVE_SYSLOG
#include <libzt/zt_log/log_syslog.h>
#endif

#endif  /* _LOG_INTERFACE_H_ */
