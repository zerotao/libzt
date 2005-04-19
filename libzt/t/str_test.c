/*
 * Copyright (C) 2000-2002, 2004, 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
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
  
  str_chomp(chomp_test);
  TEST("str_chomp[1]:", (strlen(chomp_test) == 5));

  str_strip(strip_test);
  TEST("str_strip[1]:", (strlen(strip_test) == 3));

  TEST("str_rspn[1]:", (str_rspn(test_string1, "\n ") == 2));

  TEST("str_rcspn[1]:", (str_rcspn(test_string1, "Q") == strlen(test_string1)));    

  str_basename(bname, 254, path, NULL);
  TEST("str_basename2[1]:", (!strcmp(bname, "config.foo")));
  str_basename(bname, 254, path, ".foo");
  TEST("str_basename2[2]:", (!strcmp(bname, "config")));

  str_dirname(dname, 254, path);
  TEST("str_dirname[1]:", (!strcmp(dname, "/home/jshiffer")));

  free(strip_test);
  free(chomp_test);
  
  
  TEST("str_sub[1]:", (strcmp(free_me = str_sub(interface, 6, 10), "face") == 0)); XFREE(free_me);
  TEST("str_sub[2]:", (strcmp(free_me = str_sub(interface, 6, 0), "face") == 0)); XFREE(free_me);
  TEST("str_sub[3]:", (strcmp(free_me = str_sub(interface, -4, 10), "face") == 0)); XFREE(free_me);
  TEST("str_sub[4]:", (strcmp(free_me = str_sub(interface, -4, 0), "face") == 0)); XFREE(free_me);
  TEST("str_sub[5]:", (strcmp(free_me = str_sub(interface, -4, -1), "fac") == 0)); XFREE(free_me);

  
  TEST("str_catv[1]:", (strcmp(free_me = str_catv(interface, -4, 0, " plant", 1, 0, NULL), "face plant") == 0)); XFREE(free_me);
  
  TEST("str_cat[1]:", (strcmp(free_me = str_cat(interface, -4, 0, " plant", 1, 0), "face plant") == 0)); XFREE(free_me);
  
  TEST("str_map[1]:", (strcmp(free_me = str_map(ALPHA, 1, 0, ALPHA, alpha), alpha) == 0)); XFREE(free_me);

  TEST("str_reverse[1]:", (strcmp(free_me = str_reverse(ALPHA, 1, 0), AHPLA) == 0)); XFREE(free_me);
  
  TEST("str_dup[1]:",(strcmp(free_me = str_dup(ALPHA, 1, 0, 1), ALPHA) == 0)); XFREE(free_me);

  free_me = str_dup(ALPHA, 1, 0, 2);
  free_me2 = str_cat(ALPHA, 1, 0, ALPHA, 1, 0);
  TEST("str_dup[2]:", strcmp(free_me, free_me2) == 0);
  XFREE(free_me);
  XFREE(free_me2);
  
  TEST("str_find[1]:", str_find(spain, 1, 0, "rain") == 5);
  TEST("str_find[2]:", str_find(spain, 1, 0, "in") == 7);
  
  TEST("str_rfind[1]:", str_rfind(spain, 1, 0, "rain") == 5);
  TEST("str_rfind[2]:", str_rfind(spain, 1, 0, "in") == 16);

  
  TEST("str_pos[1]:", strcmp(&interface[str_pos(interface, -4)-1], "face") == 0);

  TEST("str_len[1]:", str_len(interface, -4, 0) == 4);

  TEST("str_cmp[1]:", str_cmp(interface, -4, 0, interface, 6, 10) == 0);

  TEST("str_cmp[2]:", str_cmp(interface, -4, -1, interface, 6, 10) == -1);
  
  
  return 0;
}
