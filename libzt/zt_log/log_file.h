/*
 * file.h        Zerotao logging to a file
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: file.h,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */
#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include <libzt/zt.h>
#include <libzt/zt_log.h>

#define ZT_LOG_FILE_OVERWRITE 0
#define ZT_LOG_FILE_APPEND    1

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

extern zt_log_ty *zt_log_file(char *name, int fopts, int lopts);

#ifdef __cplusplus
}
#endif
#endif  /* _LOG_FILE_H_ */
