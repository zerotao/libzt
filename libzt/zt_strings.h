/****h* libZT/CString
 * DESCRIPTION
 *   string manipulation functions
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 ****/


#ifndef _ZT_STRINGS_H_
#define _ZT_STRINGS_H_

#include <unistd.h>
#include <stdarg.h>
#include <libzt/zt.h>

BEGIN_C_DECLS

/* New additions */
extern char *str_sub(const char *s, int i, int j);
extern char *str_dup(const char *s, int i, int j, int n);
extern char *str_cat(const char *s1, int i1, int j1,
		     const char *s2, int i2, int j2);
extern char *str_catv(const char *s, ...);
extern char *str_reverse(const char *s, int i, int j);
extern char *str_map(const char *s, int i, int j,
		     const char *from, const char *to);
extern int str_pos(const char *s, int i);
extern int str_len(const char *s, int i, int j);
extern int str_cmp(const char *s1, int i1, int j1,
		   const char *s2, int i2, int j2);
extern int str_chr(const char *s, int i, int j, int c);
extern int str_rchr(const char *s, int i, int j, int c);
extern int str_upto(const char *s, int i, int j, const char *set);
extern int str_rupto(const char *s, int i, int j, const char *set);
extern int str_find(const char *s, int i, int j, const char *str);
extern int str_rfind(const char *s, int i, int j, const char *str);
extern int str_any(const char *s, int i, const char *set);
extern int str_many(const char *s, int i, int j, const char *set);
extern int str_rmany(const char *s, int i, int j, const char *set);
extern int str_match(const char *s, int i, int j, const char *str);
extern int str_rmatch(const char *s, int i, int j, const char *str);
extern int str_format(int code, va_list	*app,
		      int put(int c, void *cl), void *cl,
		      unsigned char flags[], int width, int precision);


extern char *str_chomp(char *s);
extern char *str_strip(char *s);


/* extensions of std C */
extern size_t str_rspn ( const char *s, const char *accept );
extern size_t str_rcspn ( const char *s, const char *reject );


/* Utility */
extern char* str_basename ( char *, int, const char*, const char* );
extern char* str_dirname ( char*, int, const char* );



END_C_DECLS
#endif /*_ZT_STRINGS_H_*/
