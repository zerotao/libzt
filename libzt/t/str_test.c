/*
 * Copyright (C) 2000-2002, 2004, 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: cstr_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description: tests for the strings interfaces
 */


#include <string.h>
#include <stdio.h>
#include <libzt/zt_cstr.h>

#include "test.h"

int
main ( argc, argv )
     int argc;
     char *argv[];
{
  char	  test_string1[] = "  Ok \n";
  char	* chomp_test = strdup(test_string1);
  char 	* strip_test = strdup(test_string1);
  char 	* ALPHA =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char	* AHPLA =  "ZYXWVUTSRQPONMLKJIHGFEDCBA";  
  char 	* alpha =  "abcdefghijklmnopqrstuvwxyz";
  char	* path = "/home/jshiffer/config.foo";
  char	* interface = "Interface";
  char  * spain = "The rain in Spain";
  char 	  bname[255];
  char 	  dname[255];
  char	* free_me;
  char	* free_me2;
  
  cstr_chomp(chomp_test);
  TEST("cstr_chomp[1]:", (strlen(chomp_test) == 5));

  cstr_strip(strip_test);
  TEST("cstr_strip[1]:", (strlen(strip_test) == 3));

  TEST("cstr_rspn[1]:", (cstr_rspn(test_string1, "\n ") == 2));

  TEST("cstr_rcspn[1]:", (cstr_rcspn(test_string1, "Q") == strlen(test_string1)));    

  cstr_basename(bname, 254, path, NULL);
  TEST("cstr_basename2[1]:", (!strcmp(bname, "config.foo")));
  cstr_basename(bname, 254, path, ".foo");
  TEST("cstr_basename2[2]:", (!strcmp(bname, "config")));

  cstr_dirname(dname, 254, path);
  TEST("cstr_dirname[1]:", (!strcmp(dname, "/home/jshiffer")));

  free(strip_test);
  free(chomp_test);
  
  
  TEST("cstr_sub[1]:", (strcmp(free_me = cstr_sub(interface, 6, 10), "face") == 0)); XFREE(free_me);
  TEST("cstr_sub[2]:", (strcmp(free_me = cstr_sub(interface, 6, 0), "face") == 0)); XFREE(free_me);
  TEST("cstr_sub[3]:", (strcmp(free_me = cstr_sub(interface, -4, 10), "face") == 0)); XFREE(free_me);
  TEST("cstr_sub[4]:", (strcmp(free_me = cstr_sub(interface, -4, 0), "face") == 0)); XFREE(free_me);
  TEST("cstr_sub[5]:", (strcmp(free_me = cstr_sub(interface, -4, -1), "fac") == 0)); XFREE(free_me);

  
  TEST("cstr_catv[1]:", (strcmp(free_me = cstr_catv(interface, -4, 0, " plant", 1, 0, NULL), "face plant") == 0)); XFREE(free_me);
  
  TEST("cstr_cat[1]:", (strcmp(free_me = cstr_cat(interface, -4, 0, " plant", 1, 0), "face plant") == 0)); XFREE(free_me);
  
  TEST("cstr_map[1]:", (strcmp(free_me = cstr_map(ALPHA, 1, 0, ALPHA, alpha), alpha) == 0)); XFREE(free_me);

  TEST("cstr_reverse[1]:", (strcmp(free_me = cstr_reverse(ALPHA, 1, 0), AHPLA) == 0)); XFREE(free_me);
  
  TEST("cstr_dup[1]:",(strcmp(free_me = cstr_dup(ALPHA, 1, 0, 1), ALPHA) == 0)); XFREE(free_me);

  free_me = cstr_dup(ALPHA, 1, 0, 2);
  free_me2 = cstr_cat(ALPHA, 1, 0, ALPHA, 1, 0);
  TEST("cstr_dup[2]:", strcmp(free_me, free_me2) == 0);
  XFREE(free_me);
  XFREE(free_me2);
  
  TEST("cstr_find[1]:", cstr_find(spain, 1, 0, "rain") == 5);
  TEST("cstr_find[2]:", cstr_find(spain, 1, 0, "in") == 7);
  
  TEST("cstr_rfind[1]:", cstr_rfind(spain, 1, 0, "rain") == 5);
  TEST("cstr_rfind[2]:", cstr_rfind(spain, 1, 0, "in") == 16);

  
  TEST("cstr_pos[1]:", strcmp(&interface[cstr_pos(interface, -4)-1], "face") == 0);

  TEST("cstr_len[1]:", cstr_len(interface, -4, 0) == 4);

  TEST("cstr_cmp[1]:", cstr_cmp(interface, -4, 0, interface, 6, 10) == 0);

  TEST("cstr_cmp[2]:", cstr_cmp(interface, -4, -1, interface, 6, 10) == -1);
  
  
  return 0;
}
