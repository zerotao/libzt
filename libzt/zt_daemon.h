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

#include <zt_internal.h>
#include <sys/types.h>

BEGIN_C_DECLS

extern int zt_daemonize( char *root, mode_t umask, int options);

END_C_DECLS
#endif /*_ZT_DAEMON_H_*/
