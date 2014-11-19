#ifndef _ZT_ARRAY_H_
#define _ZT_ARRAY_H_

#include <stddef.h> /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct zt_array *zt_array_t;
struct zt_array {
    size_t    length;   /* number of elements */
    size_t    size; /* size of an element */
    char    * data;
};

extern zt_array_t zt_array_new(size_t len, size_t size);
extern void     zt_array_free(zt_array_t *array);
extern zt_array_t zt_array_with(char *buf, size_t len, size_t size, int copy);
extern zt_array_t zt_array_with_cstr(char *str);

extern void     zt_array_resize(zt_array_t array, size_t len);
extern zt_array_t zt_array_copy(zt_array_t array, size_t len);

extern size_t   zt_array_length(zt_array_t array);
extern size_t   zt_array_size(zt_array_t array);

extern char *   zt_array_data(zt_array_t array);

extern size_t   zt_array_set_length(zt_array_t array, size_t len);
extern void     zt_array_set_data(zt_array_t array, char *data, size_t len, size_t size, int copy);

void *          zt_array_elem_copy(zt_array_t array, size_t offt, void *elem);
extern void *   zt_array_get(zt_array_t array, size_t i); /* , void **elem); */
extern void *   zt_array_put(zt_array_t array, size_t i, void *elem);

#ifdef __cplusplus
}
#endif

#endif /* _ZT_ARRAY_H_ */
