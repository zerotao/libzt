/*
 * assert_test.c        test assertions
 *
 * Copyright (C) 2000-2002, 2004, 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: assert_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#include <libzt/zt_assert.h>
#include "test.h"

int
main ( argc, argv )
     int argc;
     char *argv[];
{
  assert(1==1);			/* will produce a warning: statement with no effect which is too true */
  assert(1==0);

  return 0;
}
