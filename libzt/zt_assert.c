/*
 * zt_assert.c        ZeroTao Assertions
 *
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
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
#include "zt_except.h"

char *assertion_failed = "assertion_failed";

void
_assert_fail (char *s, char *file, unsigned int line,
	      char *func, char fatal)
{
  if(func)
    log_printf(log_err, "Assertion \"%s\" failed at: %s[%d:%s]", s, file, line, func);
  else
    log_printf(log_err, "Assertion \"%s\" failed at: %s[%d]", s, file, line);

  if(fatal)
	  TRY_THROW(assertion_failed);
}
