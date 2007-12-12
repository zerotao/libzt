#include <stdio.h>
#include <libzt/zt_unit.h>


void
test_fn_1(struct zt_unit_test *test, void *data) 
{
	ZT_UNIT_ASSERT(test, 1 != 2);
}

void
test_fn_2(struct zt_unit_test *test, void *data)
{
	char * abc = "123";
	
	ZT_UNIT_ASSERT_RAISES(test, abc, TRY_THROW(abc));
	ZT_UNIT_ASSERT_RAISES(test, abc, TRY_THROW(abc));
	
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
extern int register_event_suite(struct zt_unit *unit);
extern int register_except_suite(struct zt_unit *unit);
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

/* 
 * extern int register_coro_suite(struct zt_unit *unit);
 * extern int register_cothread_suite(struct zt_unit *unit);
 */


int
main(int argc, char *argv[])
{
	struct zt_unit			* unit;
	struct zt_unit_suite	* suite1;
	struct zt_unit_test		* test1;
	int						  result;
	
	unit = zt_unit_init();

	suite1 = zt_unit_register_suite(unit, "unit test suite", NULL, NULL, NULL);
	test1 = zt_unit_register_test(suite1, "unit test 1", test_fn_1);
	zt_unit_register_test(suite1, "unit test 2", test_fn_2);

	/* register actual tests */
	register_assert_suite(unit);
	register_array_suite(unit);
	register_table_suite(unit);
	//register_adt_suite(unit);
	register_bstream_suite(unit);
	//register_bytearray_suite(unit);
	register_cfg_suite(unit);
	//register_coro_suite(unit);
	//register_cothread_suite(unit);
	register_cstr_suite(unit);
	register_event_suite(unit);
	register_except_suite(unit);
	register_format_suite(unit);
	register_gc_suite(unit);
	register_int_suite(unit);
	register_list_suite(unit);
	register_llist_suite(unit);
	register_log_suite(unit);
	register_macros_suite(unit);
	//register_msg_queue_suite(unit);
	register_opts_suite(unit);
	register_progname_suite(unit);
	register_set_suite(unit);
	register_stack_suite(unit);
	register_time_suite(unit);
	register_tree_suite(unit);
	
	zt_unit_main(unit, argc, argv);
	
}
