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
#include "zt_assert.h"
#include "zt_log.h"
#include "zt_except.h"
#include "zt_cstr.h"

char *assertion_failed = "assertion_failed";

void
_assert_fail (char *s, char *file, unsigned int line,
			  const char *func, char fatal)
{
	char	bname[PATH_MAX];

	zt_cstr_basename(bname, PATH_MAX, file, NULL);
	
	if(func)
		log_printf(log_err, "Assertion \"%s\" failed at: %s[%d:%s]", s, bname, line, func);
	else
		log_printf(log_err, "Assertion \"%s\" failed at: %s[%d]", s, bname, line);

	if(fatal)
		TRY_THROW(assertion_failed);
}
