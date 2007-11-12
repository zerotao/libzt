#include <libzt/zt.h>
#include <libzt/zt_msgq.h>
#include <libzt/zt_unit.h>


static void
basic_tests(struct zt_unit_test *test, void *data)
{
	msg_queue msgq = msg_queue_init();

	
	return 0;
}

int
register_msg_queue_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "msg queue tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
