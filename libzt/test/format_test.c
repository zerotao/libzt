#include <string.h>
#include <libzt/zt_assert.h>
#include <libzt/zt_format.h>
#include <libzt/zt_cstr.h>

#include "test.h"

static int
cvt_S(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision) 
{
	char	* str = va_arg(*app, char *);
	
	assert(str);

	return fmt_puts(str, strlen(str), put, cl, flags, width, precision);
}


int
main(int argc, char *argv[]) 
{
	char 	  buf[256];
	char	* str;
	
	fmt_sprintf(buf, 256, "%s: %d\n", "this is a test", 34);
	TEST("fmt_sprintf & fmt_vsprintf[1]:",
	     cstr_cmp(buf, 1, 0,
		      "this is a test: 34\n", 1, 0) == 0);
	
	fmt_sprintf(buf, 256, "\t%s: 0x~x~~ %% %~ ~%\n", "this is a test", 34);
	TEST("fmt_sprintf & fmt_vsprintf[2]:",
	     cstr_cmp(buf, 1, 0,
		     "\tthis is a test: 0x22~ % ~ %\n", 1, 0) == 0);

	
	TEST("fmt_register[1]:", fmt_register('S', cvt_S) == 0);
	
	fmt_sprintf(buf, 256, "%S:\n", "this is a test");
	TEST("fmt_register[2]:",
	     cstr_cmp(buf, 1, 0,
		     "this is a test:\n", 1, 0) == 0);


	str = fmt_strprintf("%s: %d\n", "this is a test", 34);
	TEST("fmt_strprintf[1]:",
	     cstr_cmp(str, 1, 0,
		     "this is a test: 34\n", 1, 0) == 0);
	XFREE(str);
	
	str = fmt_strprintf("\t%s: 0x~x~~ %% %~ ~%\n", "this is a test", 34);
	TEST("fmt_strprintf[2]:",
	     cstr_cmp(str, 1, 0,
		     "\tthis is a test: 0x22~ % ~ %\n", 1, 0) == 0);
	XFREE(str);
	
	return 0;
}
