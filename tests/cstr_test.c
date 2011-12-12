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
#define ZT_WITH_UNIT
#include <zt.h>
#include <errno.h>

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    char   test_string1[] = "  Ok \n";
    char * chomp_test = strdup(test_string1);
    char * strip_test = strdup(test_string1);
    char * ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char * AHPLA = "ZYXWVUTSRQPONMLKJIHGFEDCBA";
    char * alpha = "abcdefghijklmnopqrstuvwxyz";
    char * path = "/home/jshiffer/config.foo";
    char * iface_str = "Interface";
    char * spain = "The rain in Spain";
    char * free_me;
    char * free_me2;
    char * hex = "34aa973cd4c4daa4f61FFB2BDBAD27316534016FXXX";
    char   digest[20];
    char   hex2[41];
    char *split_test = "/a/b/c/d/";
    zt_ptr_array *split_array;
    zt_ptr_array *cut_array;
#if !defined(_WIN32)
    char   bname[PATH_MAX+1];
    char   dname[PATH_MAX+1];
#endif


    zt_cstr_chomp(chomp_test);
    ZT_UNIT_ASSERT(test, (strlen(chomp_test) == 5));

    zt_cstr_strip(strip_test);
    ZT_UNIT_ASSERT(test, (strlen(strip_test) == 3));

    ZT_UNIT_ASSERT(test, (zt_cstr_rspn(test_string1, "\n ") == 2));
    ZT_UNIT_ASSERT(test, (zt_cstr_rspn(test_string1, "Ok\n ") == strlen(test_string1)));

    ZT_UNIT_ASSERT(test, (zt_cstr_rcspn(test_string1, "QRZ\nno") == strlen(test_string1) - 1));
    ZT_UNIT_ASSERT(test, (zt_cstr_rcspn(test_string1, "Q") == strlen(test_string1)));

    split_array = zt_cstr_split(split_test, "/");
    ZT_UNIT_ASSERT_NOT_EQUAL(test, split_array, NULL);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(split_array, 0), "a"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(split_array, 1), "b"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(split_array, 2), "c"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(split_array, 3), "d"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, zt_ptr_array_get_idx(split_array, 4), NULL);
    ZT_UNIT_ASSERT_EQUAL(test, zt_cstr_split_free(split_array), 0);

    cut_array = zt_cstr_cut(split_test, '/', 1);
    ZT_UNIT_ASSERT_NOT_EQUAL(test, cut_array, NULL);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 0), "/a/b/c/d/"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 1), "/b/c/d/"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 2), "/c/d/"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 3), "/d/"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, zt_ptr_array_get_idx(cut_array, 4), NULL);
    ZT_UNIT_ASSERT_EQUAL(test, zt_cstr_cut_free(cut_array), 0);

    cut_array = zt_cstr_cut(split_test, '/', 0);
    ZT_UNIT_ASSERT_NOT_EQUAL(test, cut_array, NULL);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 0), "a/b/c/d/"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 1), "b/c/d/"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 2), "c/d/"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 3), "d/"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, zt_ptr_array_get_idx(cut_array, 4), NULL);
    ZT_UNIT_ASSERT_EQUAL(test, zt_cstr_cut_free(cut_array), 0);

    cut_array = zt_cstr_tok(split_test, '/', 0);
    ZT_UNIT_ASSERT_NOT_EQUAL(test, cut_array, NULL);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 0), "/a/b/c/d"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 1), "/a/b/c"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 2), "/a/b"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, strcmp((const char *)zt_ptr_array_get_idx(cut_array, 3), "/a"), 0);
    ZT_UNIT_ASSERT_EQUAL(test, zt_ptr_array_get_idx(cut_array, 4), NULL);
    ZT_UNIT_ASSERT_EQUAL(test, zt_cstr_cut_free(cut_array), 0);

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

    ZT_UNIT_ASSERT(test, strcmp(free_me = zt_cstr_path_append("/foo/bar", "baz/"), "/foo/bar/baz/") == 0); zt_free(free_me);

    ZT_UNIT_ASSERT(test, strcmp(free_me = zt_cstr_path_append("/foo/bar", "/baz/"), "/foo/bar/baz/") == 0); zt_free(free_me);

    zt_cstr_dirname(dname, PATH_MAX, "foo");
    ZT_UNIT_ASSERT(test, (!strcmp(dname, ".")));

#else /* _WIN32 */
      /* reverse the mapping */

#endif /* _WIN32 */

    free(strip_test);
    free(chomp_test);

    ZT_UNIT_ASSERT(test, zt_cstr_abspath(PATH_SEPERATOR"tmp") == true);
    ZT_UNIT_ASSERT(test, zt_cstr_abspath("."PATH_SEPERATOR"tmp") == true);
    ZT_UNIT_ASSERT(test, zt_cstr_abspath("tmp") == false);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(iface_str, 5, 8), "face") == 0)); zt_free(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(iface_str, 5, -1), "face") == 0)); zt_free(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(iface_str, -4, 8), "face") == 0)); zt_free(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(iface_str, -4, -1), "face") == 0)); zt_free(free_me);
    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_sub(iface_str, -4, -2), "fac") == 0)); zt_free(free_me);


    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_catv(iface_str, -4, -1, " plant", 0, -1, NULL), "face plant") == 0)); zt_free(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_cat(iface_str, -4, -1, " plant", 0, -1), "face plant") == 0)); zt_free(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_map(ALPHA, 0, -1, ALPHA, alpha), alpha) == 0)); zt_free(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_map(ALPHA, 0, -1, NULL, NULL), ALPHA) == 0)); zt_free(free_me);

    ZT_UNIT_ASSERT(test, zt_cstr_map(NULL, 0, -1, NULL, NULL) == NULL);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_reverse(ALPHA, 0, -1), AHPLA) == 0)); zt_free(free_me);

    ZT_UNIT_ASSERT(test, (strcmp(free_me = zt_cstr_dup(ALPHA, 0, -1, 1), ALPHA) == 0)); zt_free(free_me);

    free_me = zt_cstr_dup(ALPHA, 0, -1, 2);
    free_me2 = zt_cstr_cat(ALPHA, 0, -1, ALPHA, 0, -1);
    ZT_UNIT_ASSERT(test, strcmp(free_me, free_me2) == 0);
    zt_free(free_me);
    zt_free(free_me2);

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

    ZT_UNIT_ASSERT(test, strcmp(&iface_str[zt_cstr_pos(iface_str, -4)], "face") == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_len(iface_str, -4, -1) == 4);

    ZT_UNIT_ASSERT(test, zt_cstr_cmp(iface_str, -4, -1, iface_str, 5, 8) == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_cmp(iface_str, 0, -1, iface_str, 0, -1) == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_cmp(iface_str, 0, -1, iface_str, 0, 4) == 1);
    ZT_UNIT_ASSERT(test, zt_cstr_cmp(iface_str, 0, 4, iface_str, 0, -1) == -1);

    /* */
    ZT_UNIT_ASSERT(test, zt_cstr_chr(iface_str, 0, -1, 'i') == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_rchr(iface_str, 0, -1, 'i') == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_chr(iface_str, 0, -1, 'e') == 3);
    ZT_UNIT_ASSERT(test, zt_cstr_rchr(iface_str, 0, -1, 'e') == 8);

    ZT_UNIT_ASSERT(test, zt_cstr_chr(iface_str, 0, -1, 'r') == 4);
    ZT_UNIT_ASSERT(test, zt_cstr_rchr(iface_str, 0, -1, 'r') == 4);

    ZT_UNIT_ASSERT(test, zt_cstr_upto(iface_str, 0, -1, "xyz") == -1);
    ZT_UNIT_ASSERT(test, zt_cstr_upto(iface_str, 0, -1, "AIb") == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_upto(iface_str, 0, -1, "far") == 4);

    ZT_UNIT_ASSERT(test, zt_cstr_rupto(iface_str, 0, -1, "xyz") == -1);
    ZT_UNIT_ASSERT(test, zt_cstr_rupto(iface_str, 0, -1, "AIb") == 0);
    ZT_UNIT_ASSERT(test, zt_cstr_rupto(iface_str, 0, -1, "aeb") == 8);
    ZT_UNIT_ASSERT(test, zt_cstr_rupto(iface_str, 0, -1, "ntr") == 4);


    ZT_UNIT_ASSERT(test, zt_hex_to_binary(hex, 40, NULL, 0) == 20);
    ZT_UNIT_ASSERT(test, zt_hex_to_binary(hex, 40, digest, 23) == 20);

    ZT_UNIT_ASSERT(test, zt_hex_to_binary(hex, 43, digest, 23) == 0);
    ZT_UNIT_ASSERT(test, errno == EINVAL);

    ZT_UNIT_ASSERT(test, zt_binary_to_hex(digest, 20, hex2, 41) == 40);
    free_me = zt_cstr_map(hex, 0, -1, ALPHA, alpha);
    ZT_UNIT_ASSERT(test, memcmp(hex2, free_me, 40) == 0);
    zt_free(free_me);

    ZT_UNIT_ASSERT(test, zt_hex_to_binary("AX", 2, digest, 1) == 0);
    ZT_UNIT_ASSERT(test, errno == EINVAL);
} /* basic_tests */


static void
intlen_test(struct zt_unit_test *test, void *data UNUSED)
{
    int       volatile i;

    for (i = 0; i < 10; i++) {
        ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(i) == 1);
    }

    for (i = 10; i < 100; i++) {
        ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(i) == 2);
    }

    for (i = 100; i < 1000; i++) {
        ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(i) == 3);
    }

    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(1000) == 4);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(9999) == 4);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(10000) == 5);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(99999) == 5);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(100000) == 6);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(999999) == 6);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(1000000) == 7);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(9999999) == 7);

    for (i = -1; i > -10; i--) {
        ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(i) == 2);
    }

    for (i = -10; i > -100; i--) {
        ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(i) == 3);
    }

    for (i = -100; i > -1000; i--) {
        ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(i) == 4);
    }

    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(-1000) == 5);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(-9999) == 5);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(-10000) == 6);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(-99999) == 6);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(-100000) == 7);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(-999999) == 7);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(-1000000) == 8);
    ZT_UNIT_ASSERT(test, zt_cstr_int_display_len(-9999999) == 8);
} /* intlen_test */

static void
itoa_test(struct zt_unit_test *test, void *data UNUSED)
{
    int                   i;
    char                  buffer[100];

    memset(buffer, 0, 100);

    /* Illegal input validation. */
    ZT_UNIT_ASSERT(test, zt_cstr_itoa(NULL, 0, 0, 100) == -1);
    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, 0, 0, 0) == -1);
    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, 10, 5, 6) == -1);

    /* Valid inputs */
    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, 0, 0, 100) == 1);
    ZT_UNIT_ASSERT(test, strcmp("0", buffer) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, 3, 0, 100) == 1);
    ZT_UNIT_ASSERT(test, strcmp("3", buffer) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, 10, 0, 100) == 2);
    ZT_UNIT_ASSERT(test, strcmp("10", buffer) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, 42, 0, 100) == 2);
    ZT_UNIT_ASSERT(test, strcmp("42", buffer) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, 987654321, 0, 100) == 9);
    ZT_UNIT_ASSERT(test, strcmp("987654321", buffer) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, -3, 0, 100) == 2);
    ZT_UNIT_ASSERT(test, strcmp("-3", buffer) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, -10, 0, 100) == 3);
    ZT_UNIT_ASSERT(test, strcmp("-10", buffer) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, -42, 0, 100) == 3);
    ZT_UNIT_ASSERT(test, strcmp("-42", buffer) == 0);

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, -987654321, 0, 100) == 10);
    ZT_UNIT_ASSERT(test, strcmp("-987654321", buffer) == 0);

    /* offset */
    for (i = 0; i < 20; i++) {
        buffer[i] = 'a';
    }

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, 301, 3, 100) == 6);
    ZT_UNIT_ASSERT(test, strcmp("aaa301", buffer) == 0);

    for (i = 0; i < 20; i++) {
        buffer[i] = 'a';
    }

    ZT_UNIT_ASSERT(test, zt_cstr_itoa(buffer, -5920, 4, 100) == 9);
    ZT_UNIT_ASSERT(test, strcmp("aaaa-5920", buffer) == 0);
} /* itoa_test */

int
register_cstr_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "string utils tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    zt_unit_register_test(suite, "intlen", intlen_test);
    zt_unit_register_test(suite, "itoa", itoa_test);
    return 0;
}
