/****h* libZT/ByteArray
 * DESCRIPTION
 *   mutable array of bytes (expands and contracts)
 *
 * COPYRIGHT
 *   Copyright (C) 2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/


#ifndef _ZT_BYTEARRAY_H_
#define _ZT_BYTEARRAY_H_

#include <libzt/zt.h>

BEGIN_C_DECLS

typedef struct bytevect bytevect;
struct bytevect {
	size_t		  size;
	size_t		  memsize;
	unsigned char	* data;
};

extern bytevect *bytevect_init(unsigned char *data, int len);

extern bytevect *bytevect_sub(bytevect *ba, int i, int j);
extern char *bytevect_dup(const bytevect *s, int i, int j, int n);
extern char *bytevect_cat(const bytevect *s1, int i1, int j1,
			    const bytevect *s2, int i2, int j2);
extern char *bytevect_catv(const bytevect *s, ...);
extern char *bytevect_reverse(const bytevect *s, int i, int j);
extern char *bytevect_map(const bytevect *s, int i, int j,
			    const bytevect *from, const bytevect *to);
extern int bytevect_pos(const bytevect *s, int i);
extern int bytevect_len(const bytevect *s, int i, int j);
extern int bytevect_cmp(const bytevect *s1, int i1, int j1,
			  const bytevect *s2, int i2, int j2);
extern int bytevect_chr(const bytevect *s, int i, int j, int c);
extern int bytevect_rchr(const bytevect *s, int i, int j, int c);

extern int bytevect_upto(const bytevect *s, int i, int j, const bytevect *set);
extern int bytevect_rupto(const bytevect *s, int i, int j, const bytevect *set);
extern int bytevect_find(const bytevect *s, int i, int j, const bytevect *bv);
extern int bytevect_rfind(const bytevect *s, int i, int j, const bytevect *bv);
extern int bytevect_any(const bytevect *s, int i, const bytevect *set);
extern int bytevect_many(const bytevect *s, int i, int j, const bytevect *set);
extern int bytevect_rmany(const bytevect *s, int i, int j, const bytevect *set);
extern int bytevect_match(const bytevect *s, int i, int j, const bytevect *bv);
extern int bytevect_rmatch(const bytevect *s, int i, int j, const bytevect *bv);
extern int bytevect_format(int code, va_list	*app,
			     int put(int c, void *cl), void *cl,
			     unsigned char flags[], int width, int precision);




END_C_DECLS
#endif /* _ZT_BYTEARRAY_H_ */
