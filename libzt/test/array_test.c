#include <libzt/adt/zt_array.h>
#include "test.h"

int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int test = 0;

int
main(int argc, char *argv[]) 
{
	int	  i, n, *v;
	zt_array array = zt_array_new(10, sizeof(int));
	zt_array array2;
	

	TEST("zt_array_new[0]", array != NULL);
	
	n = sizeof_array(values);

	TEST("zt_array_length[0]", zt_array_length(array) == sizeof_array(values));
	
	TEST("zt_array_size[0]", zt_array_size(array) == sizeof(int));

	
	for(i=0; i < n; i++) {
		zt_array_put(array, i, &values[i]);
	}


	test = 0;
	for(i=0; i < n; i++) {
		v = zt_array_get(array, i);

		TEST_N("zt_array_get & zt_array_put", test, *v == i);
	}

	
	test = 0;
	zt_array_resize(array, zt_array_length(array) * 2);

	TEST("zt_array_resize[0]", zt_array_length(array) == n * 2);

	array2 = zt_array_copy(array, zt_array_length(array));
	
	test = 0;
	TEST_N("zt_array_copy", test, zt_array_length(array) == zt_array_length(array2));
	TEST_N("zt_array_copy", test, zt_array_size(array) == zt_array_size(array2));
	{
		int	  tt = 0;
		
		for(i=0; i < n * 2; i++) {
			v = zt_array_get(array2, i);
			if(*v == i) {
				tt++;
			}
		}

		TEST_N("zt_array_copy", test, tt == n);
	}
	test = 0;
	
	zt_array_free(&array);
	zt_array_free(&array2);
	
	{
		char	* tmp = "This is a test";
		
		array = zt_array_with_cstr(tmp);
	
		TEST_N("zt_array_cstr", test, strcmp(tmp, zt_array_data(array)) == 0);
		
		zt_array_free(&array);
	}
	
		
	return 0;
}
