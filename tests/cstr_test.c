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
#include <limits.h>
#include <libzt/zt_cstr.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    char   test_string1[] = "  Ok \n";
    char * chomp_test = strdup(test_string1);
    char * strip_test = strdup(test_string1);
    char * ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char * AHPLA = "ZYXWVUTSRQPONMLKJIHGFEDCBA";
    char * alpha = "abcdefghijklmnopqrstuvwxyz";
    char * path = "/home/jshiffer/config.foo";
    char * interface = "Interface";
    char * spain = "The rain in Spain";
    char   bname[PATH_MAX+1];
    char   dname[PATH_MAX+1];
    char * free_me;
    char * free_me2;
    char * hex = "34aa973cd4c4daa4f61eEB2BDBAD27316534016FXXX";
    char   digest[20];
    char   hex2[41];


    zt_cstr_chomp(chomp_test);
    ZT_UNIT_ASSERT(test, (strlen(chomp_test) == 5));

    zt_cstr_strip(strip_test);
    ZT_UNIT_ASSERT(test, (strlen(strip_test) == 3));

    ZT_UNIT_ASSERT(test, (zt_cstr_rspn(test_string1, "\n ") == 2));
    ZT_UNIT_ASSERT(test, (zt_cstr_rspn(test_string1, "Ok\n ") == strlen(test_string1)));

    ZT_UNIT_ASSERT(test, (zt_cstr_rcspn(test_string1, "QRZ\nno") == strlen(test_string1) - 1));
    ZT_UNIT_ASSERT(test, (zt_cstr_rcspn(test_string1, "Q") == strlen(test_string1)));

#if !defined(_WIN32)
    zt_cstr_basename(bname, PATH_MAX, path, NULL);
    ZT_UNIT_ASSERT(test, (!strcmp(bname, "config.foo")));
    zt_cstr_basename(bname, PATH_MAX, path, ".foo");
    ZT_UNIT_ASSERT(test, (!strcmp(bname, "config")));

    zt_cstr_basename(bname, PATH_MAX, "c:\\Windows\\System32\\", NULL);
    ZT_UNIT_ASSERT(test, (!strcmp(bname, "c:\\Windows\\System32\\")));

    zt_cstr_dirname(dname, PATH_MAX, path);
    ZT_UNIT_ASSERT(test, (!strcmp(dname, "/home/jshiffer")));

    zt_cstr_dirname(bname, PATH_MAX, "/foo/bar/baz/");
    ZT_UNIT_ASSERT(test, (!strcmp(bname, "/foo/bar")));

    ZT_UNIT_ASSERT(test, strcmp(free_me = zt_cstr_path_append("/foo/bar", "baz/"), "/foo/bar/baz/") == 0); XFREE(free_me);

    ZT_UNIT_ASSERT(test, strcmp(free_me = zt_cstr_path_append("/foo/bar", "/baz/"), "/foo/bar/baz/") == 0); XFREE(free_me);

#else /* _WIN32 */
      /* reverse the mapping */

#endif /* _WIN32 */

    free(strip_test);
    free(chomp_test);


    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, 5, 8), "face") == 0)); XFREE(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, 5, -1), "face") == 0)); XFREE(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, -4, 8), "face") == 0)); XFREE(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, -4, -1), "face") == 0)); XFREE(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(interface, -4, -2), "fac") == 0)); XFREE(free_me);


    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_catv(interface, -4, -1, " plant", 0, -1, NULL), "face plant") == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_cat(interface, -4, -1, " plant", 0, -1), "face plant") == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_map(ALPHA, 0, -1, ALPHA, alpha), alpha) == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_map(ALPHA, 0, -1, NULL, NULL), ALPHA) == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, zt_cstr_map(NULL, 0, -1, NULL, NULL) == NULL);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_reverse(ALPHA, 0, -1), AHPLA) == 0)); XFREE(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_dup(ALPHA, 0, -1, 1), ALPHA) == 0)); XFREE(free_me);

    free_me = zt_cstr_dup(ALPHA, 0, -1, 2);
    free_me2 = zt_cstr_cat(ALPHA, 0, -1, ALPHA, 0, -1);
    ZT_UNIT_ASSERT(test, strcmp(free_me, free_me2) == 0);
    XFREE(free_me);
    XFREE(free_me2);

    ZT_UNIT_ASSERT(test, zt_cstr_find(spain, 0, -1, "") == -1);
    ZT_UNIT_ASSERT(test, zt_cstr_find(spain, 0, -1, "e") == 2);
    ZT_UNIT_ASSERT(test, zt_cstr_find(spain, 0, -1, "rain") == 4);
    ZT_UNIT_ASSERT(test, zt_cstr_find(spain, 0, -1, "in") == 6);
    ZT_UNIT_ASSERT(test, zt_cstr_find(spain, 0, -1, "Spain") == 12);
    ZT_UNIT_ASSERT(test, zt_cstr_find(spain, 0, -1, "zqb") == -1);

    ZT_UNIT_ASSERT(test, zt_cstr_rfind(spain, 0, -1, "") == -1);
    ZT_UNIT_ASSERT(test, zt_cstr_rfind(spain, 0, -1, "e") == 2);
    ZT_UNIT_ASSERT(test, zt_cstr_rfind(spain, 0, -1, "rain") == 4);
    ZT_UNIT_ASSERT(test, zt_cstr_rfind(spain, 0, -1, "in") == 15);
    ZT_UNIT_ASSERT(test, zt_cstr_rfind(spain, 0, -1, "The") == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_rfind(spain, 0, -1, "zqb") == -1);

    ZT_UNIT_ASSERT(test, zt_cstr_any(spain, 0, -1, "zqbS") == 12);
    ZT_UNIT_ASSERT(test, zt_cstr_any(spain, 0, -1, "zqb") == -1);

    ZT_UNIT_ASSERT(test, zt_cstr_rany(spain, 0, -1, "zqbS") == 12);
    ZT_UNIT_ASSERT(test, zt_cstr_rany(spain, 0, -1, "zqb") == -1);

    ZT_UNIT_ASSERT(test, strcmp(&interface[zt_cstr_pos(interface, -4)], "face") == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_len(interface, -4, -1) == 4);

    ZT_UNIT_ASSERT(test, zt_cstr_cmp(interface, -4, -1, interface, 5, 8) == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_cmp(interface, 0, -1, interface, 0, -1) == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_cmp(interface, 0, -1, interface, 0, 4) == 1);
    ZT_UNIT_ASSERT(test, zt_cstr_cmp(interface, 0, 4, interface, 0, -1) == -1);

    /* */
    ZT_UNIT_ASSERT(test, zt_cstr_chr(interface, 0, -1, 'i') == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_rchr(interface, 0, -1, 'i') == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_chr(interface, 0, -1, 'e') == 3);
    ZT_UNIT_ASSERT(test, zt_cstr_rchr(interface, 0, -1, 'e') == 8);

    ZT_UNIT_ASSERT(test, zt_cstr_chr(interface, 0, -1, 'r') == 4);
    ZT_UNIT_ASSERT(test, zt_cstr_rchr(interface, 0, -1, 'r') == 4);

    ZT_UNIT_ASSERT(test, zt_cstr_upto(interface, 0, -1, "xyz") == -1);
    ZT_UNIT_ASSERT(test, zt_cstr_upto(interface, 0, -1, "AIb") == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_upto(interface, 0, -1, "far") == 4);

    ZT_UNIT_ASSERT(test, zt_cstr_rupto(interface, 0, -1, "xyz") == -1);
    ZT_UNIT_ASSERT(test, zt_cstr_rupto(interface, 0, -1, "AIb") == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_rupto(interface, 0, -1, "aeb") == 8);
    ZT_UNIT_ASSERT(test, zt_cstr_rupto(interface, 0, -1, "ntr") == 4);


    ZT_UNIT_ASSERT(test, zt_hex_to_binary(hex, 40, NULL, 0) == 20);
    ZT_UNIT_ASSERT(test, zt_hex_to_binary(hex, 43, digest, 23) == 20);
    ZT_UNIT_ASSERT(test, zt_binary_to_hex(digest, 20, hex2, 41) == 40);
    free_me = zt_cstr_map(hex, 0, -1, ALPHA, alpha);
    ZT_UNIT_ASSERT(test, memcmp(hex2, free_me, 40) == 0);
    XFREE(free_me);

    ZT_UNIT_ASSERT(test, zt_hex_to_binary("AX", 2, digest, 1) == -1);
} /* basic_tests */

int
register_cstr_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "string utils tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
