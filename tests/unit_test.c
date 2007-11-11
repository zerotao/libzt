#include <stdio.h>
#include <libzt/zt_unit.h>


void
test_fn_1(struct zt_unit_test *test, void *data) 
{
	ZT_UNIT_ASSERT(test, 1 == 2);
}

void
test_fn_2(struct zt_unit_test *test, void *data)
{
	char * abc = "123";
	
	ZT_UNIT_ASSERT_RAISES(test, abc, TRY_THROW(abc));
	ZT_UNIT_ASSERT_RAISES(test, abc, TRY_THROW(abc));
	// ZT_UNIT_ASSERT_RAISES(abc, {});
	
	return;
}

extern int register_array_suite(struct zt_unit *unit);
extern int register_table_suite(struct zt_unit *unit);

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
	register_array_suite(unit);
	register_table_suite(unit);
	
	zt_unit_main(unit, argc, argv);
	
}
