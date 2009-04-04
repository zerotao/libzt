#include <libzt/zt_int.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
        unsigned int    a = 22;
        unsigned int    b = 44;
        unsigned int    c = 0;

        
        c = uint_add(a, b);

        /* mixed sign addition */
        ZT_UNIT_ASSERT(test, int_add(-128, 0) == -128);
        ZT_UNIT_ASSERT(test, int_add(127, -128) == -1);
        /* printf("%d\n", c); */
}

int
register_int_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "int tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
