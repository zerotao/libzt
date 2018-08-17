/*
 * zt_assert.c        ZeroTao Assertions
 *
 * Copyright (C) 2000-2006, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: assert.c,v 1.1 2002/11/10 23:36:42 jshiffer Exp $
 *
 */

/*
 * Description:
 */

#ifdef HAVE_CONFIG_H
# include <libzt/zt_config.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "zt_internal.h"
#include "zt_log.h"
#include "zt_cstr.h"
#include "zt_assert.h"

void
_zt_log_assertion(const char *s, const char *file, unsigned int line,
                  const char *func)
{
    char bname[PATH_MAX];

    if (!s) {
        s = "no assertion given";
    }

    (void)zt_cstr_basename(bname, PATH_MAX, file, NULL);

    if (func) {
        zt_log_printf(zt_log_err, "Assertion Failed: \"%s\": %s[%d:%s]", s, bname, line, func);
    } else {
        zt_log_printf(zt_log_err, "Assertion Failed: \"%s\": %s[%d]", s, bname, line);
    }
}

int
_zt_log_abort(const char *s, const char *file, unsigned int line,
              const char *func)
{
    char bname[PATH_MAX];

    if (!s) {
        s = "no message given";
    }

    (void)zt_cstr_basename(bname, PATH_MAX, file, NULL);

    if (func) {
        zt_log_printf(zt_log_crit, "Exiting: \"%s\": %s[%d:%s]", s, bname, line, func);
    } else {
        zt_log_printf(zt_log_crit, "Exiting: \"%s\": %s[%d]", s, bname, line);
    }

    return 0;
}
