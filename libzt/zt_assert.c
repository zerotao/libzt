/*
 * zt_assert.c        ZeroTao Assertions
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
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
#include "zt_assert.h"
#include "zt_log.h"

void
_assert_fail (s, file, line, func)
     char *s;
     char *file;
     unsigned int line;
     char *func;
{
  if(func)
    log_printf(log_err, "Assertion \"%s\" failed at: %s[%d:%s]\n", s, file, line, func);
  else
    log_printf(log_err, "Assertion \"%s\" failed at: %s[%d]\n", s, file, line);

  abort();
}
