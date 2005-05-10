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
extern char *cstr_sub(const char *s, int i, int j);
extern char *cstr_dup(const char *s, int i, int j, int n);
extern char *cstr_cat(const char *s1, int i1, int j1,
		     const char *s2, int i2, int j2);
extern char *cstr_catv(const char *s, ...);
extern char *cstr_reverse(const char *s, int i, int j);
extern char *cstr_map(const char *s, int i, int j,
		     const char *from, const char *to);
extern int cstr_pos(const char *s, int i);
extern int cstr_len(const char *s, int i, int j);
extern int cstr_cmp(const char *s1, int i1, int j1,
		   const char *s2, int i2, int j2);
extern int cstr_chr(const char *s, int i, int j, int c);
extern int cstr_rchr(const char *s, int i, int j, int c);
extern int cstr_upto(const char *s, int i, int j, const char *set);
extern int cstr_rupto(const char *s, int i, int j, const char *set);
extern int cstr_find(const char *s, int i, int j, const char *str);
extern int cstr_rfind(const char *s, int i, int j, const char *str);
extern int cstr_any(const char *s, int i, const char *set);
extern int cstr_many(const char *s, int i, int j, const char *set);
extern int cstr_rmany(const char *s, int i, int j, const char *set);
extern int cstr_match(const char *s, int i, int j, const char *str);
extern int cstr_rmatch(const char *s, int i, int j, const char *str);
extern int cstr_format(int code, va_list	*app,
		      int put(int c, void *cl), void *cl,
		      unsigned char flags[], int width, int precision);


extern char *cstr_chomp(char *s);
extern char *cstr_strip(char *s);


/* extensions of std C */
extern size_t cstr_rspn ( const char *s, const char *accept );
extern size_t cstr_rcspn ( const char *s, const char *reject );


/* Utility */
extern char* cstr_basename ( char *, int, const char*, const char* );
extern char* cstr_dirname ( char*, int, const char* );



END_C_DECLS
#endif /*_ZT_STRINGS_H_*/
