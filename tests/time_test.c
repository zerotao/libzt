
#include <libzt/zt.h>
#include <libzt/zt_time.h>
#include <libzt/zt_log.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    struct timeval tv1;
    struct timeval tv2;
    struct timeval tv_zero = { 0, 0 }; /* 0 second diff time */
    struct timeval tv4 = { 2, 0 };   /* 2 second diff time */
    struct timeval tgt;


    gettimeofday(&tv1, NULL);
    gettimeofday(&tv2, NULL);

    ZT_UNIT_ASSERT(test, zt_cmp_time(&tv1, &tv2) <= 0);

    zt_diff_time(&tgt, &tv1, &tv2);
    ZT_UNIT_ASSERT(test, zt_cmp_time(&tv4, &tgt) > 0);

    zt_sub_time(&tgt, &tv1, &tv1);
    ZT_UNIT_ASSERT(test, zt_cmp_time(&tgt, &tv_zero) == 0);

    zt_add_time(&tgt, &tv1, &tv1);
    ZT_UNIT_ASSERT(test, zt_cmp_time(&tgt, &tv1) > 0);

    /*
     *    {
     *    struct time_result tr;
     *
     *    void *test_fn(void * f) {
     *    sleep(1);
     *    return NULL;
     *    }
     *
     *    zt_time(1, &tr, test_fn, NULL);
     *    zt_time_print_result(&tr, "test_fn", 1);
     *    }
     */
}

int
register_time_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "time tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return (0);
}
