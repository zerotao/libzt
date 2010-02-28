/*!
 * Filename: zt_progname.h
 * Description: program name functions
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *      Copyright (C) 2000-2010, Jason L. Shiffer.
 *      See file COPYING for details
 *
 * Notes:
 *
 */

#ifndef _ZT_PROGNAME_H_
#define _ZT_PROGNAME_H_

#include <libzt/zt.h>

#define STRIP_DIR 1

BEGIN_C_DECLS
extern char *zt_progname(char *path, int flags);

END_C_DECLS
#endif /* _ZT_PROGNAME_H_ */
