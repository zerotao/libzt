#include <string.h>

#define ZT_WITH_UNIT
#include <zt.h>

ssize_t values0_19[] = {
    0,  1,  2,  3,  4,  5,
    6,  7,  8,  9,  10,
    11, 12, 13, 14, 15,
    16, 17, 18, 19
};

char * valuesA_Z[] = {
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
};

#define MAX_LEN (int)sizeof_array(values0_19)
#define MAX_STR_LEN (int)sizeof_array(valuesA_Z)

ssize_t values0_19_fill[MAX_LEN + 1];
char *  valuesA_Z_fill[MAX_STR_LEN + 1];

static int union_test_int(void *data, void *param)
{
    struct zt_unit_test * test = (struct zt_unit_test *)param;
    ssize_t               result = (intptr_t)data;

    ZT_UNIT_ASSERT(test, values0_19_fill[result] == 0);

    values0_19_fill[result] = result;

    return 0;
}

static int intersection_test_int(void *data, void *param)
{
    struct zt_unit_test * test = (struct zt_unit_test *)param;
    ssize_t               result = (intptr_t)data;

    ZT_UNIT_ASSERT(test, (values0_19_fill[result] == 0) &&
                         result >= MAX_LEN / 2);

    values0_19_fill[result] = result;

    return 0;
}

static int difference_test_int(void *data, void *param)
{
    struct zt_unit_test * test = (struct zt_unit_test *)param;
    ssize_t               result = (intptr_t)data;

    ZT_UNIT_ASSERT(test, (values0_19_fill[result] == 0) &&
                         result < MAX_LEN / 2);

    values0_19_fill[result] = result;

    return 0;
}

static int union_test_str(void *data, void *param)
{
    struct zt_unit_test * test = (struct zt_unit_test *)param;
    int                   result = *(char *)data;

    result -= 97;
    ZT_UNIT_ASSERT(test, valuesA_Z_fill[result] == 0);
    valuesA_Z_fill[result] = (char *)data;

    return 0;
}

static int intersection_test_str(void *data, void *param)
{
    struct zt_unit_test * test = (struct zt_unit_test *)param;
    int                   result = *(char *)data;

    result -= 97;

    ZT_UNIT_ASSERT(test,
                   (valuesA_Z_fill[result] == 0) &&
                   result >= MAX_STR_LEN / 2);

    valuesA_Z_fill[result] = (char *)data;

    return 0;
}

static int difference_test_str(void *data, void *param)
{
    struct zt_unit_test * test = (struct zt_unit_test *)param;
    int                   result = *(char *)data;

    result -= 97;
    ZT_UNIT_ASSERT(test,
                   (valuesA_Z_fill[result] == NULL) &&
                   result < MAX_STR_LEN / 2);

    valuesA_Z_fill[result] = (char *)data;

    return 0;
}
static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    int      i;
    zt_set * result;
    zt_set * set1;
    zt_set * set2;


    set1 = zt_set_init(zt_table_compare_int, NULL, NULL);
    set2 = zt_set_init(zt_table_compare_int, NULL, NULL);

    for (i = 0; i < MAX_LEN; i++) {
        zt_set_insert(set1, (void *)values0_19[i], sizeof(values0_19[i]));
        if (i >= MAX_LEN / 2) {
            zt_set_insert(set2, (void *)values0_19[i], sizeof(values0_19[i]));
        }
    }


    memset(values0_19_fill, 0, sizeof(values0_19_fill));
    result = zt_set_init(zt_table_compare_int, NULL, NULL);
    zt_set_union(result, set1, set2);
    zt_set_for_each(result, union_test_int, test);
    zt_set_destroy(result);

    memset(values0_19_fill, 0, sizeof(values0_19_fill));
    result = zt_set_init(zt_table_compare_int, NULL, NULL);
    zt_set_intersection(result, set1, set2);
    zt_set_for_each(result, intersection_test_int, test);
    zt_set_destroy(result);

    memset(values0_19_fill, 0, sizeof(values0_19_fill));
    result = zt_set_init(zt_table_compare_int, NULL, NULL);
    zt_set_difference(result, set1, set2);
    zt_set_for_each(result, difference_test_int, test);
    zt_set_destroy(result);

    ZT_UNIT_ASSERT(test, zt_set_is_member(set1, (void *)values0_19[0], sizeof(values0_19[0])));
    ZT_UNIT_ASSERT(test, zt_set_is_subset(set2, set1));
    ZT_UNIT_ASSERT(test, zt_set_is_subset(set1, set2) == 0);

    ZT_UNIT_ASSERT(test, zt_set_is_equal(set1, set1));
    ZT_UNIT_ASSERT(test, zt_set_is_equal(set2, set2));
    ZT_UNIT_ASSERT(test, zt_set_is_equal(set1, set2) == 0);
    ZT_UNIT_ASSERT(test, zt_set_is_equal(set2, set1) == 0);

    zt_set_destroy(set1);
    zt_set_destroy(set2);

    set1 = zt_set_init(zt_table_compare_string, NULL, NULL);
    set2 = zt_set_init(zt_table_compare_string, NULL, NULL);

    for (i = 0; i < MAX_STR_LEN; i++) {
        zt_set_insert(set1, (void *)valuesA_Z[i], strlen(valuesA_Z[i]));
        if (i >= MAX_STR_LEN / 2) {
            zt_set_insert(set2, (void *)valuesA_Z[i], strlen(valuesA_Z[i]));
        }
    }

    ZT_UNIT_ASSERT(test, zt_set_is_subset(set2, set1));
    ZT_UNIT_ASSERT(test, zt_set_is_subset(set1, set2) == 0);

    ZT_UNIT_ASSERT(test, zt_set_is_equal(set1, set1));
    ZT_UNIT_ASSERT(test, zt_set_is_equal(set2, set2));
    ZT_UNIT_ASSERT(test, zt_set_is_equal(set1, set2) == 0);
    ZT_UNIT_ASSERT(test, zt_set_is_equal(set2, set1) == 0);

    memset(valuesA_Z_fill, 0, sizeof(valuesA_Z_fill));
    result = zt_set_init(zt_table_compare_string, NULL, NULL);
    zt_set_union(result, set1, set2);
    zt_set_for_each(result, union_test_str, test);
    zt_set_destroy(result);

    memset(valuesA_Z_fill, 0, sizeof(valuesA_Z_fill));
    result = zt_set_init(zt_table_compare_string, NULL, NULL);
    zt_set_intersection(result, set1, set2);
    zt_set_for_each(result, intersection_test_str, test);
    zt_set_destroy(result);

    memset(valuesA_Z_fill, 0, sizeof(valuesA_Z_fill));
    result = zt_set_init(zt_table_compare_string, NULL, NULL);
    zt_set_difference(result, set1, set2);
    zt_set_for_each(result, difference_test_str, test);
    zt_set_destroy(result);

    zt_set_destroy(set1);
    zt_set_destroy(set2);

} /* basic_tests */


int
register_set_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "set tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
