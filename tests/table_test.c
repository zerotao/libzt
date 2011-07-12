/*
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 *
 */

/*
 * Description:
 */
#include <string.h>
#include <stdio.h>
#define ZT_WITH_UNIT
#include <zt.h>

#define STR_TEST_PRE "strings "

static int count = 0;

static int str_iterator(void *key, void *datum, void *param);
static int int_iterator(void *key, void *datum, void *param);
static int str_free(void *key, void *datum, void *param);

void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    char       buf[1024];
    ssize_t    i;
    ssize_t    nn = 0;
    ssize_t    numbers[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    zt_table * table_int = NULL;
    zt_table * table_str = NULL;

    table_int = zt_table_init("test int table", zt_table_hash_int, zt_table_compare_int, 10, 0, NULL);
    table_str = zt_table_init("test string table", zt_table_hash_string,
                              zt_table_compare_string, 10, 0, NULL);

    for ( i = 9; i >= 0; i--) {
        zt_table_set(table_int, (void *)i, (void *)numbers[i]);
    }

    zt_table_for_each(table_int, int_iterator, test);

    ZT_UNIT_ASSERT(test, count == 10);

    nn = (ssize_t)zt_table_get( table_int, (void *)5 );
    ZT_UNIT_ASSERT(test, nn == 4);

    /* string test */
    /* printf("Testing strings\n"); */

    count = 0;    /* GLOBAL */

    for ( i = 9; i >= 0; i--) {
        char * b2 = NULL;
        sprintf(buf, "%s%zd", STR_TEST_PRE, i);
        b2 = zt_strdup(buf);
        zt_table_set(table_str, b2, (void *)numbers[i]);
    }

    zt_table_for_each(table_str, str_iterator, test);
    ZT_UNIT_ASSERT(test, count == 10);

    zt_table_for_each(table_str, str_free, NULL);

    zt_table_destroy(table_int);
    zt_table_destroy(table_str);
} /* basic_tests */

static int
str_free(void *key, void *datum UNUSED, void *param UNUSED)
{
    zt_free(key);
    return 0;
}

static int
str_iterator(void *key, void *datum, void *param)
{
    ssize_t               d = (intptr_t)datum;
    ssize_t               k = atoi(((const char *)key + strlen(STR_TEST_PRE)));
    struct zt_unit_test * test = (struct zt_unit_test *)param;

    ZT_UNIT_ASSERT(test, (k + d == 9));

    count++;    /* GLOBAL */
    return 0;
}

static int
int_iterator(void *key, void *datum, void *param)
{
    ssize_t               d = (intptr_t)datum;
    ssize_t               k = (intptr_t)key;
    struct zt_unit_test * test = (struct zt_unit_test *)param;

    ZT_UNIT_ASSERT(test, (k + d == 9));

    count++;    /* GLOBAL */
    return 0;
}

int
register_table_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "table tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
