/* 
 * zt_daemon.h        daemon tools
 * 
 * Copyright (C) 2001-2004, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * This file is part of libzt.
 * 
 * $Id: daemon.h,v 1.3 2003/06/09 18:57:39 jshiffer Exp $
 */

#ifndef _ZT_DAEMON_H_
#define _ZT_DAEMON_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif


/** f
 * daemonize a process following (for the most part)
 * the rules defined by W. Richard Stevens. Advanced Programming in the UNIX
 * Enviroment.
 * @param: char * : string with a path a directory to chroot to.
 * @param: mode_t : The umask that will be set
 * @param: int : Unused
 * @return: 0 on success
 * @usage: 
 * 
 */

extern int daemonize( char *root, mode_t umask, int options);

#ifdef __cplusplus
}
#endif

#endif /*_ZT_DAEMON_H_*/
