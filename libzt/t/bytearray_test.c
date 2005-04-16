#include <libzt/zt_bytearray.h>

#include "test.h"

char	* data = "This is a test one two three";

int main(int argc, char *argv[]) 
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


	TEST("basic operations (newWithCstring, append, toCstring)", strcmp(buf, data) == 0);
	
	zt_byteArray_free(&ba1);
	zt_byteArray_free(&ba2);
	zt_byteArray_free(&ba3);
	return 0;
}
