/*
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: progname_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#include <string.h>

#include <libzt/zt_progname.h>

#include "test.h"


int
main ( argc, argv )
     int argc;
     char *argv[];
{
  char *name = NULL;
  name = progname(argv[0], 0);
  TEST("progname[1]: ", (!strcmp(name, argv[0])));

  name = progname(NULL, 0);
  TEST("progname[2]: ", (!strcmp(name, argv[0])));

  name = progname("foo", 0);
  TEST("progname[3]: ", (!strcmp(name, "foo")));

  return 0;
}
