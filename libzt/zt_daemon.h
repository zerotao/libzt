/****h* libZT/Daemon
 * DESCRIPTION
 *   daemon tools
 *
 * COPYRIGHT
 *   Copyright (C) 2001-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   This file is part of libzt.
 *
 ****/

#ifndef _ZT_DAEMON_H_
#define _ZT_DAEMON_H_

#include <libzt/zt.h>
#include <sys/types.h>

BEGIN_C_DECLS

/****f* Daemon/daemonize
 * NAME
 *   daemonize - do everything necessary in order to become a proper daemon
 *
 * DESCRIPTION
 *   daemonize a process following (for the most part) the rules
 *   defined by W. Richard Stevens. Advanced Programming in the UNIX
 *
 * INPUTS
 *   o root    - String with a path a directory to chroot to.
 *   o umask   - The umask that will be set
 *   o options -  Unused
 *
 * RETURNS
 *   0 on success
 *
 ****/
extern int daemonize( char *root, mode_t umask, int options);

END_C_DECLS
#endif /*_ZT_DAEMON_H_*/
