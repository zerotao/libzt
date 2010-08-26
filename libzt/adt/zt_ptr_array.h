#ifndef __ZT_PTR_ARRAY_H__
#define __ZT_PTR_ARRAY_H__

#define ZT_PTR_ARRAY_DEFAULT_SIZE 32


typedef struct {
    uint32_t size;
    uint32_t count;
    void   **array;
} zt_ptr_array;

#define zt_ptr_array_is_full(array) (array->count >= array->size)
#define zt_ptr_array_length(array) array->count

zt_ptr_array *zt_ptr_array_init(void);
int zt_ptr_array_copy_data(zt_ptr_array *src, char **dstbuf);
int zt_ptr_array_resize(zt_ptr_array *array, uint32_t expand); 
int zt_ptr_array_move_idx_to_idx(zt_ptr_array *array, uint32_t src_idx, uint32_t dst_idx);
int zt_ptr_array_del(zt_ptr_array *array, void *data, int should_free);
int zt_ptr_array_add(zt_ptr_array *array, void *data);
void *zt_ptr_array_get_idx(zt_ptr_array *array, uint32_t idx); 

#endif
