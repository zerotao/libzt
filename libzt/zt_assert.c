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
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "zt.h"
#include "zt_log.h"
#include "zt_cstr.h"
#include "zt_assert.h"

void
_zt_assert_fail(char *s, char *file, unsigned int line,
                const char *func)
{
    char bname[PATH_MAX];

    (void)zt_cstr_basename(bname, PATH_MAX, file, NULL);

    if (func) {
        zt_log_printf(zt_log_err, "Assertion \"%s\" failed: %s[%d:%s]", s, bname, line, func);
    } else{
        zt_log_printf(zt_log_err, "Assertion \"%s\" failed: %s[%d]", s, bname, line);
    }
}
