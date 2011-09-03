/*!
 * Filename: zt_path.h
 * Description: path utils
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *      Copyright (C) 2010-2011, Jason L. Shiffer.
 *      See file COPYING for details
 */

#ifndef _ZT_PATH_
#define _ZT_PATH_

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif /* HAVE_SYS_STAT_H */

#include <zt.h>

BEGIN_C_DECLS

typedef enum {
    zt_mkdir_create_parent = 1,
}zt_mkdir_flags;

int zt_mkdir(char * path, mode_t mode, zt_mkdir_flags flags);


END_C_DECLS

#endif /* _ZT_PATH_ */
