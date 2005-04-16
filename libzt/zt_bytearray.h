/**
 * byte array
 *
 * Copyright (C) 2005, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * mutable array of bytes (expands and contracts)
 */


#ifndef _ZT_BYTEARRAY_H_
#define _ZT_BYTEARRAY_H_

#include <libzt/zt.h>

BEGIN_C_DECLS

typedef struct zt_byteArray zt_byteArray;

struct zt_byteArray {
	char		  bytes_per_char;
	size_t		  size;
	unsigned char 	* offset;
	unsigned char	* data;
};

zt_byteArray *zt_byteArray_new(void);
zt_byteArray *zt_byteArray_newWithSize(size_t size);


zt_byteArray *zt_byteArray_newWithCstring(const char *data);
zt_byteArray *zt_byteArray_newWithCstring_size(const char *data, size_t size);
zt_byteArray *zt_byteArray_newCopy(zt_byteArray *this);


void zt_byteArray_free(zt_byteArray **this);
size_t zt_byteArray_length(zt_byteArray *this);
size_t zt_byteArray_memUse(zt_byteArray *this);

void zt_byteArray_append(zt_byteArray *this,
			 zt_byteArray *new);

void zt_byteArray_toCstring(unsigned char *buf,
			    zt_byteArray *this,
			    char rep);



END_C_DECLS
#endif /* _ZT_BYTEARRAY_H_ */
