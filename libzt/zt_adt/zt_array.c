#include <string.h>

#include "../zt.h"
#include "../zt_assert.h"
#include "zt_array.h"

static void
arrayrep_init(array_ty array, int len, int size, void *ary);


static void
arrayrep_init(array_ty array, int len, int size, void *ary)
{
	assert(array);
	assert((ary && len > 0) ||
	       (len == 0 && ary == NULL));
	assert(size > 0);
	
	array->length = len;
	array->size = size;
	array->data = ary;
}


array_ty array_new(int len, int size) 
{
	array_ty array;
	
	array = XMALLOC(struct array_ty, 1);

	if(len > 0) {
		arrayrep_init(array, len, size,
			      XCALLOC(unsigned char, len*size));
	} else {
		arrayrep_init(array, len, size, NULL);
	}

	return array;	
}

void
array_free(array_ty *array)
{
	assert(array && *array);
	XFREE((*array)->data);
	XFREE(*array);
}

void
array_resize(array_ty array, int len)
{
	assert(array);
	assert(len >= 0);

	if (array->length == 0) {
		array->data = XCALLOC(unsigned char, len*array->size);
	} else if (len > 0) {
		array->data = XREALLOC(unsigned char, array->data, len*array->size);
	} else {
		XFREE(array->data);
	}

	array->length = len;
}

array_ty
array_copy(array_ty array, int len)
{
	array_ty	  copy;

	assert(array);
	assert(len >= 0);
	copy = array_new(len, array->size);

	if (array->length > 0 &&
	    copy->length > 0) {
		memcpy(copy->data, array->data, MIN(array->length, copy->length) * copy->size);
	}

	return copy;
}

