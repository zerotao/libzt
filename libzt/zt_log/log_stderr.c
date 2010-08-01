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

static void destructor(zt_log_ty *log)
{
    XFREE(log);
    return;
}

static void print(zt_log_ty *log, zt_log_level level, char *fmt, va_list ap)
{
    char *nfmt = zt_log_gen_fmt( log, fmt, level, log->opts);

    vfprintf(stderr, nfmt, ap);
    XFREE(nfmt);
}

/* component data */
static zt_log_vtbl_ty vtbl = {
    sizeof(zt_log_ty),
    destructor,
    print,
};

zt_log_ty *
zt_log_stderr(unsigned int opts)
{
    return zt_log_new(&vtbl, opts);
}

