/*!
 * Filename: zt_daemon.h
 * Description: deamon tools
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */

#ifndef _ZT_DAEMON_H_
#define _ZT_DAEMON_H_

#include <sys/types.h>
#include <zt.h>

BEGIN_C_DECLS

extern int zt_daemonize( char *root, mode_t umask, int options);
extern int zt_writepid(const char* pidF);

END_C_DECLS
#endif /*_ZT_DAEMON_H_*/
