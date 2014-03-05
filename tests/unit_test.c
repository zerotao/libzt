#include <stdio.h>
#define ZT_WITH_UNIT
#include <zt.h>


void
test_fn_1(struct zt_unit_test *test, void *data UNUSED)
{
    ZT_UNIT_ASSERT(test, 1 != 2);
}

void
test_fn_2(struct zt_unit_test *test UNUSED, void *data UNUSED)
{
    /* char * abc = "123"; */

    /* ZT_UNIT_ASSERT_RAISES(test, abc, TRY_THROW(abc)); */
    /* ZT_UNIT_ASSERT_RAISES(test, abc, TRY_THROW(abc)); */

    return;
}

extern int register_assert_suite(struct zt_unit *unit);
extern int register_array_suite(struct zt_unit *unit);
extern int register_table_suite(struct zt_unit *unit);
extern int register_adt_suite(struct zt_unit *unit);
extern int register_bstream_suite(struct zt_unit *unit);
extern int register_bytearray_suite(struct zt_unit *unit);
extern int register_cfg_suite(struct zt_unit *unit);
extern int register_cstr_suite(struct zt_unit *unit);
extern int register_format_suite(struct zt_unit *unit);
extern int register_gc_suite(struct zt_unit *unit);
extern int register_int_suite(struct zt_unit *unit);
extern int register_list_suite(struct zt_unit *unit);
extern int register_llist_suite(struct zt_unit *unit);
extern int register_log_suite(struct zt_unit *unit);
extern int register_macros_suite(struct zt_unit *unit);
extern int register_msg_queue_suite(struct zt_unit *unit);
extern int register_opts_suite(struct zt_unit *unit);
extern int register_progname_suite(struct zt_unit *unit);
extern int register_set_suite(struct zt_unit *unit);
extern int register_stack_suite(struct zt_unit *unit);
extern int register_time_suite(struct zt_unit *unit);
extern int register_tree_suite(struct zt_unit *unit);
extern int register_sha1_suite(struct zt_unit *unit);
extern int register_uuid_suite(struct zt_unit *unit);
extern int register_ipv4_tbl_suite(struct zt_unit *unit);
extern int register_include_suite(struct zt_unit *unit);
extern int register_daemon_suite(struct zt_unit *unit);
extern int register_path_suite(struct zt_unit *unit);
extern int register_base_suite(struct zt_unit *unit);
extern int register_buf_suite(struct zt_unit *unit);
extern int register_random_suite(struct zt_unit *unit);


int
main(int argc, char *argv[])
{
    struct zt_unit       * unit;
    struct zt_unit_suite * suite1;
    int                    result = 0;

    unit = zt_unit_init();

    suite1 = zt_unit_register_suite(unit, "unit test suite", NULL, NULL, NULL);
    zt_unit_register_test(suite1, "unit test 1", test_fn_1);
    zt_unit_register_test(suite1, "unit test 2", test_fn_2);

    /* register actual tests */
    register_cfg_suite(unit);
    register_assert_suite(unit);
    register_array_suite(unit);
    register_table_suite(unit);
    register_bstream_suite(unit);
    register_cstr_suite(unit);
    register_format_suite(unit);
    register_gc_suite(unit);
    register_int_suite(unit);
    register_list_suite(unit);
    register_llist_suite(unit);
    register_log_suite(unit);
    register_macros_suite(unit);
    register_opts_suite(unit);
    register_progname_suite(unit);
    register_set_suite(unit);
    register_stack_suite(unit);
    register_time_suite(unit);
    register_tree_suite(unit);
    register_sha1_suite(unit);
    register_uuid_suite(unit);
    register_include_suite(unit);
    /* register_ipv4_tbl_suite(unit); */
    register_daemon_suite(unit);
    register_path_suite(unit);
    register_base_suite(unit);
    register_buf_suite(unit);
    register_random_suite(unit);

    /*
     * register_adt_suite(unit);
     * register_bytearray_suite(unit);
     * register_msg_queue_suite(unit);
     */

    result = zt_unit_main(unit, argc, argv);

    zt_unit_release(&unit);
    return result;
} /* main */
