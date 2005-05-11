#include <string.h>

#include "../zt.h"
#include "../zt_assert.h"
#include "zt_array.h"

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

/* exported functions */
array_ty
array_new(int len, int size) 
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

array_ty
array_with(unsigned char *data, int len, int size, int copy)
{
	array_ty	  array;

	array = XMALLOC(struct array_ty, 1);

	if(copy) {
		arrayrep_init(array, len, size,
			      XCALLOC(unsigned char, len*size));
		memcpy(array->data, data, len * size);
	} else {
		arrayrep_init(array, len, size, data);
	}
	
	return array;
}

array_ty
array_with_cstr(char *str) 
{
	return array_with(str, strlen(str), sizeof(char), 1);
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

	if (array->length > 0 && copy->length > 0) {
		memcpy(copy->data, array->data,
		       MIN(array->length, copy->length) * copy->size);
	}

	return copy;
}

int
array_length(array_ty array)
{
	assert(array);
	return array->length;
}


int
array_size(array_ty array)
{
	assert(array);
	return array->size;
}

unsigned char *
array_data(array_ty array)
{
	assert(array);
	return array->data;
}


void *
array_get(array_ty array, int offt)
{
	assert(array);
	assert(offt >= 0 && offt < array->length);
	
	return array->data + offt * array->size;
}



void *
array_put(array_ty array, int offt, void *elem)
{
	assert(array);
	assert(offt >= 0 && offt < array->length);
	assert(	elem);
	memcpy(array->data + offt * array->size, elem, array->size);

	return elem;
}
