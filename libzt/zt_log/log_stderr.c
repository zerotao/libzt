/*
 * stderr.c        ZeroTao logging to stderr
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: stderr.c,v 1.3 2003/06/10 04:00:48 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#include <stdio.h>

#include "../zt.h"
#include "log_private.h"

static void destructor ( log )
    log_ty *log;
{
	XFREE(log);
	return;
}

static void print ( log, level, fmt, ap)
     log_ty    *log;
     log_level level;
     char      *fmt;
     va_list   ap;
{
  char *nfmt = log_gen_fmt( log, fmt, level, log->opts);
  vfprintf(stderr, nfmt, ap);
  XFREE(nfmt);
}

/* component data */
static log_vtbl_ty vtbl = {
  sizeof(log_ty),
  destructor,
  print,
};

log_ty *
log_stderr( opts)
     unsigned int opts;
{
    return log_new(&vtbl, opts);
}

