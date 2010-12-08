#include <string.h>

#include <zt_array.h>
#include <zt_unit.h>

int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    size_t    n;
    size_t    i;
    int     * pv;

    zt_array_t array = zt_array_new(10, sizeof(int));
    zt_array_t array2;


    ZT_UNIT_ASSERT(test, array != NULL);

    n = sizeof_array(values);

    ZT_UNIT_ASSERT(test, zt_array_length(array) == sizeof_array(values));

    ZT_UNIT_ASSERT(test, zt_array_size(array) == sizeof(int));

    for (i = 0; i < n; i++) {
        zt_array_put(array, i, &values[i]);
    }

    for (i = 0; i < n; i++) {
        pv = zt_array_get(array, i);
        ZT_UNIT_ASSERT(test, *pv == (int)i);
    }

    {
        size_t    tt = 0;
        size_t    nn = zt_array_length(array);

        for (i = 0; i < nn; i++) {
            pv = zt_array_get(array, i);
            if (*pv == (int)i) {
                tt++;
            }
        }

        ZT_UNIT_ASSERT(test, nn == tt);
    }

    zt_array_resize(array, zt_array_length(array) * 2);
    ZT_UNIT_ASSERT(test, zt_array_length(array) == n * 2);

    array2 = zt_array_copy(array, zt_array_length(array));

    ZT_UNIT_ASSERT(test, zt_array_length(array) == zt_array_length(array2));
    ZT_UNIT_ASSERT(test, zt_array_size(array) == zt_array_size(array2));
    {
        size_t    tt = 0;

        for (i = 0; i < n; i++) {
            pv = zt_array_get(array2, i);
            if (*pv == (int)i) {
                tt++;
            }
        }

        ZT_UNIT_ASSERT(test, tt == n);
    }
    zt_array_free(&array);
    zt_array_free(&array2);

    {
        char * tmp = "This is a test";
        char * data = NULL;

        array = zt_array_with_cstr(tmp);
        data = zt_array_data(array);

        ZT_UNIT_ASSERT(test, zt_array_length(array) == strlen(tmp));

        ZT_UNIT_ASSERT(test, strncmp(tmp, data, zt_array_length(array)) == 0);
        zt_array_free(&array);
    }
} /* basic_tests */

int register_array_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "array tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}

