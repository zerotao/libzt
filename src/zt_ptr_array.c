#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "zt_ptr_array.h"

zt_ptr_array *
zt_ptr_array_init(void *args, zt_ptr_array_free_cb free_cb) {
    zt_ptr_array *array;

    if (!(array = calloc(sizeof(zt_ptr_array), 1))) {
        return NULL;
    }

    array->size    = ZT_PTR_ARRAY_DEFAULT_SIZE;
    array->args    = args;
    array->free_cb = free_cb;

    if (!(array->array = malloc(sizeof(char *) * array->size))) {
        free(array);
        return NULL;
    }

    return array;
}


int
zt_ptr_array_copy_data(zt_ptr_array *src, char **dstbuf) {
    int32_t i;

    if (src == NULL || dstbuf == NULL) {
        return -1;
    }

    for (i = 0; i < src->count; i++) {
        dstbuf[i] = src->array[i];
    }

    return 0;
}

int
zt_ptr_array_resize(zt_ptr_array *array, int32_t expand) {
    int32_t size;
    char  **ndata;

    if (array == NULL) {
        return -1;
    }

    if (expand == 0) {
        size = ZT_PTR_ARRAY_DEFAULT_SIZE;
    } else {
        size = expand;
    }

    if ((array->size + size) < array->size) {
        /* error on overflow */
        return -1;
    }

    ndata = malloc(sizeof(char *) * (array->size + size));

    if (ndata == NULL) {
        return -1;
    }

    if (zt_ptr_array_copy_data(array, ndata) < 0) {
        return -1;
    }

    free(array->array);

    array->array = (void **)ndata;
    array->size += size;

    return 0;
}

int
zt_ptr_array_move_idx_to_idx(zt_ptr_array *array, int32_t src_idx, int32_t dst_idx) {
    if (src_idx > array->count) {
        return -1;
    }

    if (dst_idx > array->count) {
        return -1;
    }

    array->array[dst_idx] = array->array[src_idx];

    return 0;
}

int
zt_ptr_array_del(zt_ptr_array *array, void *data) {
    int32_t i;

    if (array == NULL || data == NULL) {
        return -1;
    }

    for (i = 0; i < array->count; i++) {
        if (array->array[i] == data) {
            zt_ptr_array_move_idx_to_idx(array, array->count - 1, i);
            array->count--;

            if (array->free_cb) {
                array->free_cb(data);
            }
        }
    }

    return -1;
}

int
zt_ptr_array_cat(zt_ptr_array *dst, zt_ptr_array *src) {
    int32_t i;

    if (dst == NULL || src == NULL) {
        return -1;
    }

    for (i = 0; i < src->count; i++) {
        if (zt_ptr_array_add(dst, zt_ptr_array_get_idx(src, i))) {
            return -1;
        }
    }

    return 0;
}

int
zt_ptr_array_add(zt_ptr_array *array, void *data) {
    if (data == NULL || array == NULL) {
        return 0;
    }

    if (zt_ptr_array_is_full(array)) {
        if (zt_ptr_array_resize(array, 0) < 0) {
            return -1;
        }
    }

    array->array[array->count] = data;
    array->count++;

    return 0;
}

int
zt_ptr_array_free(zt_ptr_array *array, int free_members) {
    int32_t i;

    if (array == NULL) {
        return -1;
    }

    if (free_members > 0) {
        for (i = 0; i < array->count; i++) {
            void *memb;

            if ((memb = array->array[i]) == NULL) {
                continue;
            }

            array->free_cb ? array->free_cb(memb) : free(memb);
        }
    }

    if (array->array) {
        free(array->array);
    }

    free(array);

    return 0;
}

void *
zt_ptr_array_get_idx(zt_ptr_array *array, int32_t idx) {
    if (idx == -1) {
        /* return the last element */
        return array->array[array->count - 1];
    }

    if (idx >= array->count) {
        return NULL;
    }

    return array->array[idx];
}
