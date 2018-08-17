#include <string.h>

#include "zt.h"
#include "zt_internal.h"

static void
arrayrep_init(zt_array_t array, size_t len, size_t size, void *ary)
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
zt_array_t
zt_array_new(size_t len, size_t size)
{
    zt_array_t array;

    array = zt_malloc(struct zt_array, 1);

    if (len > 0) {
        arrayrep_init(array, len, size,
                      zt_calloc(char, len * size));
    } else {
        arrayrep_init(array, len, size, NULL);
    }

    return array;
}

void
zt_array_free(zt_array_t *array)
{
    zt_assert(array && *array);
    zt_free((*array)->data);
    zt_free(*array);
}

void
zt_array_set_data(zt_array_t array, char *data,
                  size_t len, size_t size, int copy)
{
    zt_assert(array);

    if (copy) {
        arrayrep_init(array, len, size,
                      zt_calloc(char, len * size));
        memcpy(array->data, data, len * size);
    } else {
        arrayrep_init(array, len, size, data);
    }
}

zt_array_t
zt_array_with(char *data, size_t len, size_t size, int copy)
{
    zt_array_t array;

    array = zt_calloc(struct zt_array, 1);

    zt_array_set_data(array, data, len, size, copy);

    return array;
}

zt_array_t
zt_array_with_cstr(char *str)
{
    return zt_array_with((char *)str, strlen(str), sizeof(char), 1);
}

void
zt_array_resize(zt_array_t array, size_t len)
{
    zt_assert(array);
    zt_assert(len > 0);

    if (array->length == 0) {
        array->data = zt_calloc(char, len * array->size);
    } else if (len > 0) {
        array->data = zt_realloc(char, array->data, len * array->size);
        /* lear the added space */
        if(len > array->length) {
            size_t   rlen = len - array->length;
            memset(&array->data[array->length*array->size], 0, rlen * array->size);
        }
    } else {
        zt_free(array->data);
        array->data = NULL;
    }

    array->length = len;
}

zt_array_t
zt_array_copy(zt_array_t array, size_t len)
{
    zt_array_t copy;

    zt_assert(array);

    copy = zt_array_new(len, array->size);

    if (array->length > 0 && copy->length > 0) {
        memcpy(copy->data, array->data,
               MIN(array->length, copy->length) * copy->size);
    }

    return copy;
}

size_t
zt_array_length(zt_array_t array)
{
    zt_assert(array);
    return array->length;
}

size_t
zt_array_set_length(zt_array_t array, size_t len)
{
    size_t olen;

    zt_assert(array);

    olen = array->length;

    array->length = len;
    return olen;
}

size_t
zt_array_size(zt_array_t array)
{
    zt_assert(array);
    return array->size;
}

char *
zt_array_data(zt_array_t array)
{
    zt_assert(array);
    return array->data;
}



void *
zt_array_elem_copy(zt_array_t array, size_t offt, void *elem)
{
    zt_assert(array);
    zt_assert(offt < array->length);
    zt_assert(elem);

    memcpy(elem, array->data + offt * array->size, array->size);
    return elem;
}

void *
zt_array_get(zt_array_t array, size_t offt)
{
    zt_assert(array);
    zt_assert(offt < array->length);
    /*
     * zt_assert(elem);
     *
     * memcpy(elem, array->data + offt * array->size, array->size);
     * *elem = ;
     */
    return array->data + offt * array->size;     /* *elem; */
}

void *
zt_array_put(zt_array_t array, size_t offt, void *elem)
{
    zt_assert(array);
    zt_assert(offt < array->length);
    zt_assert(elem);

    memcpy(array->data + offt * array->size, elem, array->size);

    return elem;
}
