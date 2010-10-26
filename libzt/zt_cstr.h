/*!
 * Filename: zt_cstr.c
 * Description: C String utilities
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */

#ifndef _ZT_STRINGS_H_
#define _ZT_STRINGS_H_

#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <libzt/zt_internal.h>
#include <libzt/adt/zt_ptr_array.h>

BEGIN_C_DECLS

/* New additions */
extern char *zt_cstr_sub(const char *s, int i, int j);
extern char *zt_cstr_dup(const char *s, int i, int j, int n);
extern char *zt_cstr_cat(const char *s1, int i1, int j1,
                         const char *s2, int i2, int j2);
extern char *zt_cstr_catv(const char *s, ...);
extern char *zt_cstr_reverse(const char *s, int i, int j);
extern char *zt_cstr_map(const char *s, int i, int j,
                         const char *from, const char *to);
extern int zt_cstr_pos(const char *s, int i);
extern int zt_cstr_len(const char *s, int i, int j);
extern int zt_cstr_cmp(const char *s1, int i1, int j1,
                       const char *s2, int i2, int j2);
extern int zt_cstr_chr(const char *s, int i, int j, int c);
extern int zt_cstr_rchr(const char *s, int i, int j, int c);
extern int zt_cstr_upto(const char *s, int i, int j, const char *set);
extern int zt_cstr_rupto(const char *s, int i, int j, const char *set);
extern int zt_cstr_find(const char *s, int i, int j, const char *str);
extern int zt_cstr_rfind(const char *s, int i, int j, const char *str);
extern int zt_cstr_any(const char *s, int i, int j, const char *set);
extern int zt_cstr_rany(const char *s, int i, int j, const char *set);
extern int zt_cstr_puts(int code, va_list * app,
                        int put(int c, void *cl), void *cl,
                        unsigned char flags[], int width, int precision);


extern char *zt_cstr_chomp(char *s);
extern char *zt_cstr_strip(char *s);

/* extensions of std C */
extern size_t zt_cstr_rspn( const char *s, const char *accept );
extern size_t zt_cstr_rcspn( const char *s, const char *reject );


/* Utility */
extern char* zt_cstr_basename( char *, int, const char*, const char* );
extern char* zt_cstr_dirname( char*, int, const char* );
extern char* zt_cstr_path_append( const char *, const char * );

extern size_t zt_binary_to_hex(void *data, size_t dlen, char *hex, size_t hlen);
extern size_t zt_hex_to_binary(char *hex, size_t hlen, void *data, size_t dlen);
extern int zt_cstr_copy(const char * from, int i, int j, char * to, int len);
extern zt_ptr_array *zt_cstr_split(const char *str, const char *delim);
extern zt_ptr_array *zt_cstr_cut(const char *str, const int delim, int keep_delim);
extern zt_ptr_array *zt_cstr_tok(const char *str, const int delim, int keep_delim);
extern inline int zt_cstr_split_free(zt_ptr_array *);
extern inline int zt_cstr_cut_free(zt_ptr_array *);

/* String formatting */

/* Integer display length */
size_t zt_cstr_int_display_len(int value);


END_C_DECLS
#endif /*_ZT_STRINGS_H_*/
