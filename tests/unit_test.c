#include <stdio.h>
#include <libzt/zt_unit.h>


void
test_fn_1(void *data) 
{
	ZT_UNIT_ASSERT(1 == 2);
}

void
test_fn_2(void *data)
{
	char * abc = "123";
	
	ZT_UNIT_ASSERT_RAISES(abc, TRY_THROW(abc));
	// ZT_UNIT_ASSERT_RAISES(abc, {});
	
	return;
}


int
main(int argc, char *argv[])
{
	struct zt_unit			* unit;
	struct zt_unit_suite	* suite1;
	struct zt_unit_test		* test1;
	int						  result;
	
	unit = zt_unit_init();

	suite1 = zt_unit_register_suite(unit, "test suite 1", NULL, NULL, NULL);
	test1 = zt_unit_register_test(suite1, "test 1", test_fn_1);
	zt_unit_register_test(suite1, "test 2", test_fn_2);

	zt_unit_main(unit, argc, argv);
	
	//result = zt_unit_run(unit);
	//printf("result: %d\n", result);

	//result = zt_unit_run_suite(unit, suite1);
	//printf("result: %d\n", result);

	//result = zt_unit_run_test(unit, suite1, test1);
	//printf("result: %d\n", result == TRUE ? 1 : 0);	
}
