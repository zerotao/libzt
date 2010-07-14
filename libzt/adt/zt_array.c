#include <string.h>

#include "../zt.h"
#include "../zt_assert.h"
#include "../zt_exceptions.h"
#include "zt_array.h"

static void
arrayrep_init(zt_array array, int len, int size, void *ary)
{
    zt_assert(array);
    zt_assert((ary && len > 0) ||
              (len == 0 && ary == NULL));
    zt_assert(size > 0);

    array->length = len;
    array->size = size;
    array->data = ary;
}

/* exported functions */
zt_array
zt_array_new(int len, int size)
{
    zt_array array;

    array = XMALLOC(struct zt_array, 1);

    if (len > 0) {
        arrayrep_init(array, len, size,
                      XCALLOC(char, len * size));
    } else {
        arrayrep_init(array, len, size, NULL);
    }

    return array;
}

void
zt_array_free(zt_array *array)
{
    zt_assert(array && *array);
    XFREE((*array)->data);
    XFREE(*array);
}

void
zt_array_set_data(zt_array array, char *data,
                  int len, int size, int copy)
{
    zt_assert(array);

    if (copy) {
        arrayrep_init(array, len, size,
                      XCALLOC(char, len * size));
        memcpy(array->data, data, len * size);
    } else {
        arrayrep_init(array, len, size, data);
    }
}

zt_array
zt_array_with(char *data, int len, int size, int copy)
{
    zt_array array;

    array = XMALLOC(struct zt_array, 1);

    if (array->data) {
        XFREE(array->data);
    }

    zt_array_set_data(array, data, len, size, copy);

    return array;
}

zt_array
zt_array_with_cstr(char *str)
{
    return zt_array_with((char *)str, strlen(str), sizeof(char), 1);
}

void
zt_array_resize(zt_array array, int len)
{
    zt_assert(array);
    zt_assert(len >= 0);

    if (array->length == 0) {
        array->data = XCALLOC(char, len * array->size);
    } else if (len > 0) {
        array->data = XREALLOC(char, array->data, len * array->size);
    } else {
        XFREE(array->data);
    }

    array->length = len;
}

zt_array
zt_array_copy(zt_array array, int len)
{
    zt_array copy;

    zt_assert(array);
    zt_assert(len >= 0);

    copy = zt_array_new(len, array->size);

    if (array->length > 0 && copy->length > 0) {
        memcpy(copy->data, array->data,
               MIN(array->length, copy->length) * copy->size);
    }

    return copy;
}

int
zt_array_length(zt_array array)
{
    zt_assert(array);
    return array->length;
}

int
zt_array_set_length(zt_array array, int len)
{
    size_t olen;

    zt_assert(array);

    olen = array->length;

    array->length = len;
    return olen;
}

int
zt_array_size(zt_array array)
{
    zt_assert(array);
    return array->size;
}

char *
zt_array_data(zt_array array)
{
    zt_assert(array);
    return array->data;
}



void *
zt_array_elem_copy(zt_array array, int offt, void *elem)
{
    zt_assert(array);
    zt_assert(offt >= 0 && offt < array->length);
    zt_assert(elem);

    memcpy(elem, array->data + offt * array->size, array->size);
    return elem;
}

void *
zt_array_get(zt_array array, int offt)
{
    zt_assert(array);
    zt_assert(offt >= 0 && offt < array->length);
    /*
     * zt_assert(elem);
     *
     * memcpy(elem, array->data + offt * array->size, array->size);
     * *elem = ;
     */
    return array->data + offt * array->size;     /* *elem; */
}

void *
zt_array_put(zt_array array, int offt, void *elem)
{
    zt_assert(array);
    zt_assert(offt >= 0 && offt < array->length);
    zt_assert(    elem);

    memcpy(array->data + offt * array->size, elem, array->size);

    return elem;
}
