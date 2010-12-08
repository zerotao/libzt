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
#include <stdarg.h>

#include <zt_internal.h>
#include <zt_ptr_array.h>

BEGIN_C_DECLS

/* New additions */
extern char *zt_cstr_sub(const char *s, ssize_t i, ssize_t j);
extern char *zt_cstr_dup(const char *s, ssize_t i, ssize_t j, ssize_t n);
extern char *zt_cstr_cat(const char *s1, ssize_t i1, ssize_t j1,
                         const char *s2, ssize_t i2, ssize_t j2);
extern char *zt_cstr_catv(const char *s, ...);
extern char *zt_cstr_reverse(const char *s, ssize_t i, ssize_t j);
extern char *zt_cstr_map(const char *s, ssize_t i, ssize_t j,
                         const char *from, const char *to);
extern size_t zt_cstr_pos(const char *s, ssize_t i);
extern size_t zt_cstr_len(const char *s, ssize_t i, ssize_t j);
extern int zt_cstr_cmp(const char *s1, ssize_t i1, ssize_t j1,
                       const char *s2, ssize_t i2, ssize_t j2);
extern size_t zt_cstr_chr(const char *s, ssize_t i, ssize_t j, int c);
extern size_t zt_cstr_rchr(const char *s, ssize_t i, ssize_t j, int c);
extern ssize_t zt_cstr_upto(const char *s, ssize_t i, ssize_t j, const char *set);
extern ssize_t zt_cstr_rupto(const char *s, ssize_t i, ssize_t j, const char *set);
extern ssize_t zt_cstr_find(const char *s, ssize_t i, ssize_t j, const char *str);
extern ssize_t zt_cstr_rfind(const char *s, ssize_t i, ssize_t j, const char *str);
extern ssize_t zt_cstr_any(const char *s, ssize_t i, ssize_t j, const char *set);
extern ssize_t zt_cstr_rany(const char *s, ssize_t i, ssize_t j, const char *set);
extern int zt_cstr_puts(int code, va_list * app,
                        int put(int c, void *cl), void *cl,
                        unsigned char flags[], int width, int precision);


extern char *zt_cstr_chomp(char *s);
extern char *zt_cstr_strip(char *s);

/* extensions of std C */
extern size_t zt_cstr_rspn( const char *s, const char *accept );
extern size_t zt_cstr_rcspn( const char *s, const char *reject );


/* Utility */
extern char* zt_cstr_basename( char *, size_t, const char*, const char* );
extern char* zt_cstr_dirname( char*, size_t, const char* );
extern char* zt_cstr_path_append( const char *, const char * );

extern size_t zt_binary_to_hex(void *data, size_t dlen, char *hex, size_t hlen);
extern size_t zt_hex_to_binary(char *hex, size_t hlen, void *data, size_t dlen);
extern size_t zt_cstr_copy(const char * from, ssize_t i, ssize_t j, char * to, size_t len);
extern zt_ptr_array *zt_cstr_split(const char *str, const char *delim);
extern zt_ptr_array *zt_cstr_cut(const char *str, const int delim, int keep_delim);
extern zt_ptr_array *zt_cstr_tok(const char *str, const int delim, int keep_delim);
extern int zt_cstr_split_free(zt_ptr_array *);
extern int zt_cstr_cut_free(zt_ptr_array *);

/* String formatting */

/* Integer to ascii, with buffer length. Returns offset to trailing 0, or 0 on failure. */
ssize_t zt_cstr_itoa(char *s, int value, size_t offset, size_t bufsiz);

/* Integer display length */
size_t zt_cstr_int_display_len(int value);


END_C_DECLS
#endif /*_ZT_STRINGS_H_*/
