/****h* libZT/Format
 * DESCRIPTION
 *
 * COPYRIGHT
 *   Copyright (C) 2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/
#ifndef _ZT_FORMAT_H_
#define _ZT_FORMAT_H_

#include <stdarg.h>

#include <libzt/zt.h>
#include <libzt/zt_exceptions.h>

BEGIN_C_DECLS
typedef int(*zt_fmt_put_f)(int c, void *cl);
typedef int (*zt_fmt_ty)(int code, va_list app,
                         zt_fmt_put_f put, void *cl,
                         unsigned char flags[256], int width,
                         int precision);


extern int zt_fmt_format(zt_fmt_put_f put, void *cl,
                         const char *fmt, ...);
extern int zt_fmt_vformat(zt_fmt_put_f put, void *cl,
                          const char *fmt, va_list ap);

extern int zt_fmt_sprintf(char *buf, int size,
                          const char *fmt, ...);
extern int zt_fmt_vsprintf(char *buf, int size,
                           const char *fmt, va_list ap);

extern int zt_fmt_printf(const char *fmt, ...);
extern int zt_fmt_fprintf(FILE *stream, const char *fmt, ...);

extern char *zt_fmt_strprintf(const char *fmt, ...);
extern char *zt_fmt_vstrprintf(const char *fmt, va_list ap);


extern zt_fmt_ty zt_fmt_register(int code, zt_fmt_ty newcvt);

extern int zt_fmt_putd(const char *str, int len,
                       zt_fmt_put_f put, void *cl,
                       unsigned char flags[256],
                       int width, int precision);

extern int zt_fmt_puts(const char *str, int len,
                       zt_fmt_put_f put, void *cl,
                       unsigned char flags[256],
                       int width, int precision);

END_C_DECLS
#endif /* _ZT_FORMAT_H_ */
