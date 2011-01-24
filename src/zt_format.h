/*!
 * Filename: zt_format.c
 * Description: portable printf replacement
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_FORMAT_H_
#define _ZT_FORMAT_H_

#include <stdarg.h>
#include <stdio.h>

#include <zt_internal.h>

BEGIN_C_DECLS
typedef int (*zt_fmt_put_f)(int c, void *cl);
typedef size_t (*zt_fmt_ty)(int code, va_list app,
                            zt_fmt_put_f put, void *cl,
                            unsigned char flags[256],
                            ssize_t width,
                            ssize_t precision);


extern size_t zt_fmt_format(zt_fmt_put_f put, void *cl,
                            const char *fmt, ...);
extern size_t zt_fmt_vformat(zt_fmt_put_f put, void *cl,
                             const char *fmt, va_list ap);

extern size_t zt_fmt_sprintf(char *buf, size_t size,
                             const char *fmt, ...);
extern size_t zt_fmt_vsprintf(char *buf, size_t size,
                              const char *fmt, va_list ap);

extern size_t zt_fmt_printf(const char *fmt, ...);
extern size_t zt_fmt_fprintf(FILE *stream, const char *fmt, ...);

extern char *zt_fmt_strprintf(const char *fmt, ...);
extern char *zt_fmt_vstrprintf(const char *fmt, va_list ap);


extern zt_fmt_ty zt_fmt_register(int code, zt_fmt_ty newcvt);

extern size_t zt_fmt_putd(const char *str, size_t len,
                          zt_fmt_put_f put, void *cl,
                          unsigned char flags[256],
                          ssize_t width, ssize_t precision);

extern size_t zt_fmt_puts(const char *str, size_t len,
                          zt_fmt_put_f put, void *cl,
                          unsigned char flags[256],
                          ssize_t width, ssize_t precision);

END_C_DECLS
#endif /* _ZT_FORMAT_H_ */
