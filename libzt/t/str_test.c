/*
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: str_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description: tests for the strings interfaces
 */


#include <string.h>
#include <stdio.h>
#include <libzt/zt_strings.h>

#include "test.h"

int
main ( argc, argv )
     int argc;
     char *argv[];
{
  char test_string1[] = "  Ok \n";
  char *chomp_test = strdup(test_string1);
  char *strip_test = strdup(test_string1);
  char *path = "/home/jshiffer/config.foo";
  char bname[255];
  char dname[255];

  strchomp(chomp_test);
  TEST("chomp[1]: ", (strlen(chomp_test) == 5));

  strstrip(strip_test);
  TEST("strip[1]: ", (strlen(strip_test) == 3));

  TEST("rstrspn[1]: ", (rstrspn(test_string1, "\n ") == 2));

  TEST("rstrcspn[1]: ", (rstrcspn(test_string1, "Q") == strlen(test_string1)));    

  basename2(bname, 254, path, NULL);
  TEST("basename2[1]: ", (!strcmp(bname, "config.foo")));
  basename2(bname, 254, path, ".foo");
  TEST("basename2[2]: ", (!strcmp(bname, "config")));

  dirname(dname, 254, path);
  TEST("dirname[1]: ", (!strcmp(dname, "/home/jshiffer")));

  free(strip_test);
  free(chomp_test);
  
  return 0;
}
