#include <libzt/adt/zt_array.h>
#include "test.h"

int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int test = 0;

int
main(int argc, char *argv[]) 
{
	int	  i, n, *v;
	array_ty array = array_new(10, sizeof(int));
	array_ty array2;
	

	TEST("array_new[0]", array != NULL);
	
	n = sizeof_array(values);

	TEST("array_length[0]", array_length(array) == sizeof_array(values));
	
	TEST("array_size[0]", array_size(array) == sizeof(int));

	
	for(i=0; i < n; i++) {
		array_put(array, i, &values[i]);
	}


	test = 0;
	for(i=0; i < n; i++) {
		v = array_get(array, i);

		TEST_N("array_get & array_put", test, *v == i);
	}

	
	test = 0;
	array_resize(array, array_length(array) * 2);

	TEST("array_resize[0]", array_length(array) == n * 2);

	array2 = array_copy(array, array_length(array));
	
	test = 0;
	TEST_N("array_copy", test, array_length(array) == array_length(array2));
	TEST_N("array_copy", test, array_size(array) == array_size(array2));
	{
		int	  tt = 0;
		
		for(i=0; i < n * 2; i++) {
			v = array_get(array2, i);
			if(*v == i) {
				tt++;
			}
		}

		TEST_N("array_copy", test, tt == n);
	}
	

	
	return 0;
}
