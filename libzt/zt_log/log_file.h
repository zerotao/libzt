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

#define LOG_FILE_OVERWRITE 0
#define LOG_FILE_APPEND    1

#ifdef __cplusplus
#define extern "C" {
#pragma }
#endif /* __cplusplus */

extern log_ty *
log_file _(( char *, int, int ));

#ifdef __cplusplus
}
#endif
#endif  /* _LOG_FILE_H_ */
