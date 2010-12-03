#ifndef __ZT_PTR_ARRAY_H__
#define __ZT_PTR_ARRAY_H__

#define ZT_PTR_ARRAY_DEFAULT_SIZE 32

typedef void (*zt_ptr_array_free_cb)(void *data);

typedef struct {
    int32_t              size;
    int32_t              count;
    void               **array;
    void                *args;
    zt_ptr_array_free_cb free_cb;
} zt_ptr_array;

#define zt_ptr_array_is_full(array) (array->count >= array->size)
#define zt_ptr_array_length(array) array->count

zt_ptr_array *zt_ptr_array_init(void *args, zt_ptr_array_free_cb free_cb);
int zt_ptr_array_free(zt_ptr_array *array, int free_members);
int zt_ptr_array_copy_data(zt_ptr_array *src, char **dstbuf);
int zt_ptr_array_resize(zt_ptr_array *array, int32_t expand);
int zt_ptr_array_move_idx_to_idx(zt_ptr_array *array, int32_t src_idx, int32_t dst_idx);
int zt_ptr_array_del(zt_ptr_array *array, void *data);
int zt_ptr_array_add(zt_ptr_array *array, void *data);
int zt_ptr_array_cat(zt_ptr_array *dst, zt_ptr_array *src);
void *zt_ptr_array_get_idx(zt_ptr_array *array, int32_t idx);

#endif
