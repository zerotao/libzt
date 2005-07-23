/*
 * cfg_test.c        config file parser test
 *
 * Copyright (C) 2000-2002, 2004, 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: cfg_test.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#include "test.h"
#include <libzt/zt_cfg.h>

int
main ( argc, argv )
     int argc;
     char *argv[];
{
  cfg_ty *cfg = NULL;
  int i = 0;
  char *s = NULL;
  double f = 0.0, f2 = 0.00, f3 = 0.00;
  short int b = 0;
  int n = 0;
  
  cfg = cfg_ini("cfg_test.ini", 0);
  
  TEST_N("cfg", n, (cfg != NULL));
  if(!cfg) {
	  printf("Could not open file cfg_test.ini\n");
	  exit(1);
  }
  


  cfg_get(cfg, "main", "bool_var", &b, cfg_bool);
  cfg_get(cfg, "main", "int_var", &i, cfg_int);
  cfg_get(cfg, "main", "float_var", &f, cfg_float);
  cfg_get(cfg, "main", "string_var", &s, cfg_string);

  TEST_N("cfg", n, (b == 1));
  TEST_N("cfg", n, (i == 1));
  TEST_N("cfg", n, (f == 99.999));
  TEST_N("cfg", n, (!strcmp(s, "This is a string")));

  f = f + 100.00;
  
  cfg_set(cfg, "main2", "float_var", &f, cfg_float);
  cfg_get(cfg, "main", "float_var", &f, cfg_float);
  cfg_get(cfg, "main2", "float_var", &f2, cfg_float);
  cfg_get(cfg, "main3", "float_var", &f3, cfg_float);
  TEST_N("cfg", n, (f == 199.999));
  TEST_N("cfg", n, (f2 == 199.999));
  TEST_N("cfg", n, (f3 == 99.999));      
  
  cfg_close(cfg);
  return 0;
}
