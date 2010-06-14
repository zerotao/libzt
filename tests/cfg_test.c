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

#include <string.h>

#include <libzt/zt_cfg.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    zt_cfg_ty *cfg = NULL;
    long       i = 0;
    char      *s = NULL;
    double     f = 0.0, f2 = 0.00, f3 = 0.00;
    int        b = 0;

    cfg = zt_cfg_ini("cfg_test.ini", 0);

    ZT_UNIT_ASSERT(test, (cfg != NULL));
    if (!cfg) {
        printf("Could not open file cfg_test.ini\n");
        exit(1);
    }

    zt_cfg_get(cfg, "main", "int_var", &i, zt_cfg_int);
    zt_cfg_get(cfg, "main", "bool_var", &b, zt_cfg_bool);
    zt_cfg_get(cfg, "main", "float_var", &f, zt_cfg_float);
    zt_cfg_get(cfg, "main", "string_var", &s, zt_cfg_string);

    ZT_UNIT_ASSERT(test, (b == 1));
    ZT_UNIT_ASSERT(test, (i == 1));
    ZT_UNIT_ASSERT(test, (f == 99.999));
    ZT_UNIT_ASSERT(test, (!strcmp(s, "This is a string")));

    zt_cfg_get_int(cfg, "main", "int_var", &i);
    zt_cfg_get_bool(cfg, "main", "bool_var", &b);
    zt_cfg_get_float(cfg, "main", "float_var", &f);
    zt_cfg_get_string(cfg, "main", "string_var", &s);

    ZT_UNIT_ASSERT(test, (b == 1));
    ZT_UNIT_ASSERT(test, (i == 1));
    ZT_UNIT_ASSERT(test, (f == 99.999));
    ZT_UNIT_ASSERT(test, (!strcmp(s, "This is a string")));

    f = f + 100.00;

    zt_cfg_set(cfg, "main2", "float_var", &f, zt_cfg_float);
    zt_cfg_get(cfg, "main", "float_var", &f, zt_cfg_float);
    zt_cfg_get(cfg, "main2", "float_var", &f2, zt_cfg_float);
    zt_cfg_get(cfg, "main3", "float_var", &f3, zt_cfg_float);

    ZT_UNIT_ASSERT(test, (f == 199.999));
    ZT_UNIT_ASSERT(test, (f2 == 199.999));
    ZT_UNIT_ASSERT(test, (f3 == 99.999));

    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "int_var", &i, zt_cfg_bool) == -1));
    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "int_var", &i, zt_cfg_float) == -1));
    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "int_var", &i, zt_cfg_string) == -1));

    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "float_var", &f, zt_cfg_bool) == -1));
    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "float_var", &f, zt_cfg_int) == -1));
    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "float_var", &f, zt_cfg_string) == -1));

    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "string_var", &i, zt_cfg_bool) == -1));
    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "string_var", &i, zt_cfg_float) == -1));
    ZT_UNIT_ASSERT(test, (zt_cfg_get(cfg, "main", "string_var", &i, zt_cfg_int) == -1));

    zt_cfg_close(cfg);
}

int
register_cfg_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "cfg tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return(0);
}
