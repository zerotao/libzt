/*
 * Copyright (C) 2000-2002, 2004, 2005, 2006, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
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
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    char   test_string1[] = "  Ok \n";
    char * chomp_test = strdup(test_string1);
    char * strip_test = strdup(test_string1);
    char * ALPHA =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char * AHPLA =  "ZYXWVUTSRQPONMLKJIHGFEDCBA";
    char * alpha =  "abcdefghijklmnopqrstuvwxyz";
    char * path = "/home/jshiffer/config.foo";
    char * interface = "Interface";
    char * spain = "The rain in Spain";
    char   bname[255];
    char   dname[255];
    char * free_me;
    char * free_me2;
    char * hex = "34aa973cd4c4daa4f61eeb2bdbad27316534016f";
    char   digest[20];
    char   hex2[41];


    zt_cstr_chomp(chomp_test);
    ZT_UNIT_ASSERT(test, (strlen(chomp_test) == 5));

    zt_cstr_strip(strip_test);
    ZT_UNIT_ASSERT(test, (strlen(strip_test) == 3));

    ZT_UNIT_ASSERT(test, (zt_cstr_rspn(test_string1, "\n ") == 2));

    ZT_UNIT_ASSERT(test, (zt_cstr_rcspn(test_string1, "Q") == strlen(test_string1)));

    zt_cstr_basename(bname, 254, path, NULL);
    ZT_UNIT_ASSERT(test, (!strcmp(bname, "config.foo")));
    zt_cstr_basename(bname, 254, path, ".foo");
    ZT_UNIT_ASSERT(test, (!strcmp(bname, "config")));

    zt_cstr_dirname(dname, 254, path);
    ZT_UNIT_ASSERT(test, (!strcmp(dname, "/home/jshiffer")));

    free(strip_test);
    free(chomp_test);


    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, 6, 10), "face") == 0)); XFREE(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, 6, 0), "face") == 0)); XFREE(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, -4, 10), "face") == 0)); XFREE(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, -4, 0), "face") == 0)); XFREE(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, -4, -1), "fac") == 0)); XFREE(free_me);


    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_catv(interface, -4, 0, " plant", 1, 0, NULL), "face plant") == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_cat(interface, -4, 0, " plant", 1, 0), "face plant") == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_map(ALPHA, 1, 0, ALPHA, alpha), alpha) == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_reverse(ALPHA, 1, 0), AHPLA) == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_dup(ALPHA, 1, 0, 1), ALPHA) == 0)); XFREE(free_me);

    free_me = zt_cstr_dup(ALPHA, 1, 0, 2);
    free_me2 = zt_cstr_cat(ALPHA, 1, 0, ALPHA, 1, 0);
    ZT_UNIT_ASSERT(test, strcmp(free_me, free_me2) == 0);
    XFREE(free_me);
    XFREE(free_me2);

    ZT_UNIT_ASSERT(test, zt_cstr_find(spain, 1, 0, "rain") == 5);
    ZT_UNIT_ASSERT(test, zt_cstr_find(spain, 1, 0, "in") == 7);

    ZT_UNIT_ASSERT(test, zt_cstr_rfind(spain, 1, 0, "rain") == 5);
    ZT_UNIT_ASSERT(test, zt_cstr_rfind(spain, 1, 0, "in") == 16);


    ZT_UNIT_ASSERT(test, strcmp(&interface[zt_cstr_pos(interface, -4) - 1], "face") == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_len(interface, -4, 0) == 4);

    ZT_UNIT_ASSERT(test, zt_cstr_cmp(interface, -4, 0, interface, 6, 10) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_cmp(interface, -4, -1, interface, 6, 10) == -1);

    ZT_UNIT_ASSERT(test, zt_hex_to_binary(hex, 40, digest, 20) == 20);
    ZT_UNIT_ASSERT(test, zt_hex_to_binary(hex, 40, NULL, 0) == 20);
    ZT_UNIT_ASSERT(test, zt_binary_to_hex(digest, 20, hex2, 41) == 40);
    ZT_UNIT_ASSERT(test, memcmp(hex2, hex, 40) == 0);
} /* basic_tests */

int
register_cstr_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "string utils tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return(0);
}
