/*
 * syslog.h        ZeroTao logging to syslog
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: syslog.h,v 1.3 2003/06/10 04:15:55 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#ifndef _LOG_SYSLOG_H_
#define _LOG_SYSLOG_H_

#include <syslog.h>

#include <libzt/zt_log.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

extern log_ty *
log_syslog _(( void ));

extern log_ty *
log_syslog2 _(( int, int));

#ifdef __cplusplus
}
#endif
#endif  /* _LOG_SYSLOG_H_ */
