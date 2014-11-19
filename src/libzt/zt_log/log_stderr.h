/*
 * stderr.h        Zerotao logging to stderr
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: stderr.h,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

#ifndef _LOG_STDERR_H_
#define _LOG_STDERR_H_

#include <libzt/zt_log.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern zt_log_ty * zt_log_stderr(unsigned int opts);

#ifdef __cplusplus
}
#endif
#endif  /* _LOG_STDERR_H_ */
