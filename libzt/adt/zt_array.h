#ifndef _ZT_ARRAY_H_
#define _ZT_ARRAY_H_

#include <zt_internal.h>
#include <zt_assert.h>
BEGIN_C_DECLS

typedef struct zt_array *zt_array_t;
struct zt_array {
    int    length;   /* number of elements */
    int    size; /* size of an element */
    char * data;
};

extern zt_array_t zt_array_new(int len, int size);
extern void     zt_array_free(zt_array_t *array);
extern zt_array_t zt_array_with(char *buf, int len, int size, int copy);
extern zt_array_t zt_array_with_cstr(char *str);

extern void     zt_array_resize(zt_array_t array, int len);
extern zt_array_t zt_array_copy(zt_array_t array, int len);

extern int      zt_array_length(zt_array_t array);
extern int      zt_array_size(zt_array_t array);

extern char *   zt_array_data(zt_array_t array);

extern int      zt_array_set_length(zt_array_t array, int len);
extern void     zt_array_set_data(zt_array_t array, char *data, int len, int size, int copy);

extern void *   zt_array_get(zt_array_t array, int i); /* , void **elem); */
extern void *   zt_array_put(zt_array_t array, int i, void *elem);

END_C_DECLS

#endif /* _ZT_ARRAY_H_ */
