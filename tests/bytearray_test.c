#include <libzt/zt_bytearray.h>
#include <libzt/zt_unit.h>

char	* data = "This is a test one two three";

static void
basic_tests(struct zt_unit_test *test, void *data)
{

	zt_byteArray	* ba1 = 0;
	zt_byteArray	* ba2 = 0;
	zt_byteArray	* ba3 = 0;
	char		  buf[1024];	
	
	ba1 = zt_byteArray_newWithCstring_size(data, 14);
	ba2 = zt_byteArray_newWithCstring(&data[14]);

	zt_byteArray_append(ba1, ba2);
	
	ba3 = zt_byteArray_newCopy(ba1);

	zt_byteArray_toCstring(buf, ba1, 0);

	ZT_UNIT_ASSERT(test, strcmp(buf, data) == 0);
	
	zt_byteArray_free(&ba1);
	zt_byteArray_free(&ba2);
	zt_byteArray_free(&ba3);
	return 0;
}

int
register_bytearray_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "bytearray", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
