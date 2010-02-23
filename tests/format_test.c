#include <string.h>
#include <libzt/zt_assert.h>
#include <libzt/zt_format.h>
#include <libzt/zt_cstr.h>
#include <libzt/zt_unit.h>

static int
cvt_S(int code, va_list app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision) 
{
	char	* str = va_arg(app, char *);
	
	zt_assert(str);

	return zt_fmt_puts(str, strlen(str), put, cl, flags, width, precision);
}


static void
basic_tests(struct zt_unit_test *test, void *data)
{
	char 	  buf[256];
	char	* str;
	
	zt_fmt_sprintf(buf, 256, "%s: %d\n", "this is a test", 34);
	ZT_UNIT_ASSERT(test,
	     zt_cstr_cmp(buf, 1, 0,
		      "this is a test: 34\n", 1, 0) == 0);
	
	zt_fmt_sprintf(buf, 256, "\t%s: 0x~x~~ %% %~ ~%\n", "this is a test", 34);
	ZT_UNIT_ASSERT(test,
	     zt_cstr_cmp(buf, 1, 0,
		     "\tthis is a test: 0x22~ % ~ %\n", 1, 0) == 0);

	
	ZT_UNIT_ASSERT(test, zt_fmt_register('S', cvt_S) == 0);
	
	zt_fmt_sprintf(buf, 256, "%S:\n", "this is a test");
	ZT_UNIT_ASSERT(test,
	     zt_cstr_cmp(buf, 1, 0,
		     "this is a test:\n", 1, 0) == 0);


	str = zt_fmt_strprintf("%s: %d\n", "this is a test", 34);
	ZT_UNIT_ASSERT(test,
	     zt_cstr_cmp(str, 1, 0,
		     "this is a test: 34\n", 1, 0) == 0);
	XFREE(str);
	
	str = zt_fmt_strprintf("\t%s: 0x~x~~ %% %~ ~%\n", "this is a test", 34);
	ZT_UNIT_ASSERT(test,
	     zt_cstr_cmp(str, 1, 0,
		     "\tthis is a test: 0x22~ % ~ %\n", 1, 0) == 0);
	XFREE(str);
}

int
register_format_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "format", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}

