/*
 * zt_progname.h        ZeroTao program name functions
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: progname.h,v 1.2 2003/06/09 13:42:13 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#ifndef _ZT_PROGNAME_H_
#define _ZT_PROGNAME_H_

#include <libzt/zt.h>

#define STRIP_DIR 1

#ifdef __cplusplus
extern "C" {
#pragma }
#endif

extern char *progname _((char *, int));

#ifdef __cplusplus
}
#endif

#endif /* _ZT_PROGNAME_H_ */
