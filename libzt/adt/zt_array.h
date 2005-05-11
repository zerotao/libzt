#ifndef _ZT_ARRAY_H_
#define _ZT_ARRAY_H_

#include <libzt/zt.h>
#include <libzt/zt_assert.h>
BEGIN_C_DECLS
	
typedef struct array_ty *array_ty;
struct array_ty {
	int		  length; /* number of elements */
	int		  size;	  /* size of an element */
	unsigned char	* data;
};

extern array_ty	array_new(int len, int size);
extern void	array_free(array_ty *array);
extern array_ty array_with(unsigned char *buf, int len, int size, int copy);
extern array_ty array_with_cstr(char *str);

extern void	array_resize(array_ty array, int len);
extern array_ty	array_copy(array_ty array, int len);

extern int	array_length(array_ty array);
extern int	array_size(array_ty array);
extern unsigned char *	array_data(array_ty array);

extern void *	array_get(array_ty array, int i);
extern void *	array_put(array_ty array, int i, void *elem);

END_C_DECLS

#endif /* _ZT_ARRAY_H_ */
