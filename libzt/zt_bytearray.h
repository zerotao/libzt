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

typedef struct string string;
struct string {
	size_t		  size;
	unsigned char	* data;
};

extern string *str_init(unsigned char *data, int len);

extern string *str_sub(string *ba, int i, int j);
extern char *str_dup(const string *s, int i, int j, int n);
extern char *str_cat(const string *s1, int i1, int j1,
			    const string *s2, int i2, int j2);
extern char *str_catv(const string *s, ...);
extern char *str_reverse(const string *s, int i, int j);
extern char *str_map(const string *s, int i, int j,
			    const string *from, const string *to);
extern int str_pos(const string *s, int i);
extern int str_len(const string *s, int i, int j);
extern int str_cmp(const string *s1, int i1, int j1,
			  const string *s2, int i2, int j2);
extern int str_chr(const string *s, int i, int j, int c);
extern int str_rchr(const string *s, int i, int j, int c);

extern int str_upto(const string *s, int i, int j, const string *set);
extern int str_rupto(const string *s, int i, int j, const string *set);
extern int str_find(const string *s, int i, int j, const string *str);
extern int str_rfind(const string *s, int i, int j, const string *str);
extern int str_any(const string *s, int i, const string *set);
extern int str_many(const string *s, int i, int j, const string *set);
extern int str_rmany(const string *s, int i, int j, const string *set);
extern int str_match(const string *s, int i, int j, const string *str);
extern int str_rmatch(const string *s, int i, int j, const string *str);
extern int str_format(int code, va_list	*app,
			     int put(int c, void *cl), void *cl,
			     unsigned char flags[], int width, int precision);




END_C_DECLS
#endif /* _ZT_BYTEARRAY_H_ */
