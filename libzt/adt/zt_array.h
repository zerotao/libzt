#ifndef _ZT_ARRAY_H_
#define _ZT_ARRAY_H_

#include <libzt/zt.h>
#include <libzt/zt_assert.h>
BEGIN_C_DECLS

typedef struct array_ty *array_ty;
struct array_ty {
	int		  length;
	int		  size;
	unsigned char	* data;
};

extern array_ty	array_new(int len, int size);
extern void	array_free(array_ty *array);
extern void	array_resize(array_ty array, int len);
extern array_ty	array_copy(array_ty array, int len);

#define array_length(array) \
	(assert(array), array->length)

#define array_size(array) \
	(assert(array), array->size)

#define array_get(array, offt)		\
	(assert(array), assert(offt >= 0 && offt < array->length),	\
	 (void *)(array->data + offt * array->size))

#define array_put(array, offt, elem)			\
	(assert(array),					\
	 assert(offt >= 0 && offt < array->length),	\
	 assert(elem),					\
	 memcpy(array->data + i * array->size,		\
		elem, array->size))


/* extern int	array_length(array_ty array); */
/* extern int	array_size(array_ty array); */
/* extern void *	array_get(array_ty array, int i); */
/* extern void *	array_put(array_ty array, int i, void *elem); */



END_C_DECLS

#endif /* _ZT_ARRAY_H_ */
