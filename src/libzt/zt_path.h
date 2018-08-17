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

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#include <zt.h>

#ifdef WIN32
  typedef unsigned int mode_t;
#endif

BEGIN_C_DECLS

typedef enum {
    zt_mkdir_create_parent = 1,
}zt_mkdir_flags;

int zt_mkdir(const char * path, mode_t mode, zt_mkdir_flags flags);
int zt_path_exists(const char * path);
char *zt_find_basedir(const char * path, const char * trigger);
END_C_DECLS

#endif /* _ZT_PATH_ */
