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

typedef struct byte_array byte_array;
struct byte_array {
	unsigned char	* data;
	size_t		  size;
};

extern byte_array *byte_array_init(unsigned char *data, int len);

extern byte_array *byte_array_sub(byte_array *ba, int i, int j);
extern char *byte_array_dup(const byte_array *s, int i, int j, int n);
extern char *byte_array_cat(const byte_array *s1, int i1, int j1,
			    const byte_array *s2, int i2, int j2);
extern char *byte_array_catv(const byte_array *s, ...);
extern char *byte_array_reverse(const byte_array *s, int i, int j);
extern char *byte_array_map(const byte_array *s, int i, int j,
			    const byte_array *from, const byte_array *to);
extern int byte_array_pos(const byte_array *s, int i);
extern int byte_array_len(const byte_array *s, int i, int j);
extern int byte_array_cmp(const byte_array *s1, int i1, int j1,
			  const byte_array *s2, int i2, int j2);
extern int byte_array_chr(const byte_array *s, int i, int j, int c);
extern int byte_array_rchr(const byte_array *s, int i, int j, int c);

extern int byte_array_upto(const byte_array *s, int i, int j, const byte_array *set);
extern int byte_array_rupto(const byte_array *s, int i, int j, const byte_array *set);
extern int byte_array_find(const byte_array *s, int i, int j, const byte_array *str);
extern int byte_array_rfind(const byte_array *s, int i, int j, const byte_array *str);
extern int byte_array_any(const byte_array *s, int i, const byte_array *set);
extern int byte_array_many(const byte_array *s, int i, int j, const byte_array *set);
extern int byte_array_rmany(const byte_array *s, int i, int j, const byte_array *set);
extern int byte_array_match(const byte_array *s, int i, int j, const byte_array *str);
extern int byte_array_rmatch(const byte_array *s, int i, int j, const byte_array *str);
extern int byte_array_format(int code, va_list	*app,
			     int put(int c, void *cl), void *cl,
			     unsigned char flags[], int width, int precision);




END_C_DECLS
#endif /* _ZT_BYTEARRAY_H_ */
