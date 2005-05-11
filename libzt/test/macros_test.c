/*
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: macros_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#include <libzt/zt.h>
#include "test.h"

int
main ( argc, argv )
     int argc;
     char *argv[];
{
  /* test ABS */
  TEST("ABS[1]: ", (ABS(-1) == 1));

  TEST("CLAMP[1]: ", (CLAMP(10, 2, 5) == 5));
  TEST("CLAMP[2]: ", (CLAMP(1, 2, 5) == 2));

  TEST("MAX[1]: ", (MAX(5, 10) == 10));
  TEST("MAX[2]: ", (MAX(10, 5) == 10));

  TEST("MIN[1]: ", (MIN(5, 10) == 5));
  TEST("MIN[2]: ", (MIN(10, 5) == 5));
  
  {
    char size[10];
    TEST("sizeof_array[1]: ", (sizeof_array(size) == 10));
    TEST("endof_array[1]: ", (endof_array(size) == &size[10]));
  }

  return 0;
}
