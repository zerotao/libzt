/*!
 *
 */

/*
 * Copyright (C) 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 */
#ifndef _ZT_FORMAT_H_
#define _ZT_FORMAT_H_

#include <stdarg.h>

#include <libzt/zt.h>
#include <libzt/zt_except.h>

BEGIN_C_DECLS
typedef int (*fmt_ty)(int code, va_list *app,
		      int put(int c, void *cl), void *cl,
		      unsigned char flags[256], int width,
		      int precision);

extern char *fmt_flags;

/* FIXME: consolidate all of the exceptions where it makes since */
extern const char *fmt_overflow;


extern int fmt_format(int put(int c, void *cl), void *cl, const char *fmt, ...);
extern int fmt_vformat(int put(int c, void *cl), void *cl, const char *fmt, va_list ap);

extern int fmt_sprintf(char *buf, int size, const char *fmt, ...);
extern int fmt_vsprintf(char *buf, int size, const char *fmt, va_list ap);

extern int fmt_printf(const char *fmt, ...);
extern int fmt_fprintf(FILE *stream, const char *fmt, ...);

extern char *fmt_strprintf(const char *fmt, ...);
extern char *fmt_vstrprintf(const char *fmt, va_list ap);


extern fmt_ty fmt_register(int code, fmt_ty newcvt);
extern int fmt_putd(const char *str, int len,
		     int put(int c, void *cl), void *cl,
		     unsigned char flags[256], int width, int precision);
extern int fmt_puts(const char *str, int len,
		     int put(int c, void *cl), void *cl,
		     unsigned char flags[256], int width, int precision);


END_C_DECLS
#endif /* _ZT_FORMAT_H_ */
