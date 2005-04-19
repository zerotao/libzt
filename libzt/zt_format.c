
#include <limits.h>
#include <ctype.h>
#include <float.h>

#include "zt_format.h"
#include "zt_assert.h"

struct buf {
	char	* buf;
	char	* bp;
	int	  size;
};

#define pad(n,c, tlen)					\
	do {						\
		int	  nn = (n);			\
		while (nn-- > 0) {			\
			tlen += put((c), cl);		\
		}					\
	} while (0)


static int cvt_c(int code, va_list *app,
		  int put(int c, void *cl), void *cl,
		  unsigned char flags[], int width, int precision);
static int cvt_d(int code, va_list *app,
		  int put(int c, void *cl), void *cl,
		  unsigned char flags[], int width, int precision);
static int cvt_f(int code, va_list *app,
		  int put(int c, void *cl), void *cl,
		  unsigned char flags[], int width, int precision);
static int cvt_o(int code, va_list *app,
		  int put(int c, void *cl), void *cl,
		  unsigned char flags[], int width, int precision);
static int cvt_p(int code, va_list *app,
		  int put(int c, void *cl), void *cl,
		  unsigned char flags[], int width, int precision);
static int cvt_s(int code, va_list *app,
		  int put(int c, void *cl), void *cl,
		  unsigned char flags[], int width, int precision);
static int cvt_u(int code, va_list *app,
		  int put(int c, void *cl), void *cl,
		  unsigned char flags[], int width, int precision);
static int cvt_x(int code, va_list *app,
		  int put(int c, void *cl), void *cl,
		  unsigned char flags[], int width, int precision);

static int outc(int c, void *cl);
static int insert(int c, void *cl);
static int append(int c, void *cl);


char *fmt_flags = "+- 0";
const char *fmt_overflow = "format overflow";
static fmt_ty cvt[256] = {
	0,     0, 0,     0,     0,     0,     0,     0,/*   0 -   7 */
	0,     0, 0,     0,     0,     0,     0,     0,/*   8 -  15 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  16 -  23 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  24 -  31 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  32 -  39 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  40 -  47 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  48 -  55 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  56 -  63 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  64 -  71 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  72 -  79 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  80 -  87 */
	0,     0, 0,     0,     0,     0,     0,     0,/*  88 -  95 */
	0,     0, 0, cvt_c, cvt_d, cvt_f, cvt_f, cvt_f,/*  96 - 103 */
	0,     0, 0,     0,     0,     0,     0, cvt_o,/* 104 - 111 */
	cvt_p, 0, 0, cvt_s,     0, cvt_u,     0,     0,/* 112 - 119 */
	cvt_x, 0, 0,     0,     0,     0,     0,     0,/* 120 - 127 */
};

/* exported functions */
int
fmt_format(int put(int c, void *cl), void *cl,
	   const char *fmt, ...)
{
	va_list	  ap;
	int	  tlen = 0;

	va_start(ap, fmt);
	tlen = fmt_vformat(put, cl, fmt, ap);
	va_end(ap);
	
	return tlen;
}

int
fmt_printf(const char *fmt, ...)
{
	int	  tlen = 0;
	va_list	  ap;

	va_start(ap, fmt);
	tlen = fmt_vformat(outc, stdout, fmt, ap);
	va_end(ap);
	
	return tlen;
}

int
fmt_fprintf(FILE *stream, const char *fmt, ...)
{
	int	  tlen = 0;
	va_list	  ap;

	va_start(ap, fmt);
	tlen = fmt_vformat(outc, stream, fmt, ap);
	va_end(ap);

	return tlen;
}


int
fmt_sprintf(char *buf, int size, const char *fmt, ...)
{
	int	  tlen = 0;
	va_list	  ap;

	va_start(ap, fmt);
	tlen = fmt_vsprintf(buf, size, fmt, ap);
	va_end(ap);

	return tlen;
}

int
fmt_vsprintf(char *buf, int size, const char *fmt, va_list ap)
{
	struct buf	  cl;

	assert(buf);
	assert(size > 0);
	assert(fmt);

	cl.buf = cl.bp = buf;
	cl.size = size;

	fmt_vformat(insert, &cl, fmt, ap);
	insert(0, &cl);
	return cl.bp - cl.buf - 1;
}

char *
fmt_strprintf(const char *fmt, ...)
{
	char	* str;
	va_list	  ap;

	assert(fmt);
	va_start(ap, fmt);

	str = fmt_vstrprintf(fmt, ap);
	
	va_end(ap);
	return str;
}

char *
fmt_vstrprintf(const char *fmt, va_list ap)
{
	struct buf	  cl;

	assert(fmt);
	cl.size = 256;
	cl.buf = cl.bp = XMALLOC(unsigned char, cl.size);
	fmt_vformat(append, &cl, fmt, ap);
	append(0, &cl);
	
	return XREALLOC(unsigned char, cl.buf, cl.bp - cl.buf);
}


int
fmt_vformat(int put(int c, void *cl), void *cl,
	    const char *fmt, va_list ap)
{
	int	  tlen = 0;
	
	assert(put);
	assert(fmt);

	while (*fmt) {
		/* both % and ~ are control chars */
		if ((*fmt != '%' && *fmt != '~') ||
		    (++fmt && (*fmt == '%' || *fmt == '~'))) {
			tlen += put((unsigned char)*fmt++, cl);
		} else {
			unsigned char	  c, flags[256];
			int		  width = INT_MIN,
				          precision = INT_MIN;
			memset(flags, '\0', sizeof(flags));

			if(fmt_flags) {
				unsigned char	  c = *fmt;
				for ( ; c && strchr(fmt_flags, c); c = *++fmt) {
					assert(flags[c] < 255);
					flags[c]++;
				}
			}

			if(*fmt == '*' || isdigit(*fmt)) {
				int	  n;
				
				if (*fmt == '*') {
					n = va_arg(ap, int);
					assert(n != INT_MIN);
					fmt++;
				} else {
					for (n = 0; isdigit(*fmt); fmt++) {
						int	  d = *fmt - '0';
						assert(n <= (INT_MAX - d)/10);
						n = 10 * n + d;
					}
				}
				width = n;
			}

			if(*fmt == '.' &&
			   (*++fmt == '*' || isdigit(*fmt))) {
				int	  n;
				if (*fmt == '*') {
					n = va_arg(ap, int);
					assert(n != INT_MIN);
					fmt++;
				} else {
					for (n = 0; isdigit(*fmt); fmt++) {
						int	  d = *fmt - '0';
						assert(n <= (INT_MAX - d)/10);
						n = 10 * n + d;
					}
				}
				precision = n;
			}
			
			c = *fmt++;
			assert(cvt[c]);
			tlen += (*cvt[c])(c, &ap, put, cl, flags, width, precision);
		}
		
	}
	return tlen;
}

#define NORMALIZE_WIDTH(width, flags)				\
	if (width == INT_MIN) {					\
		width = 0;					\
	} else if (width < 0) {					\
		flags['-'] = 1;					\
		width = -width;					\
	}
	
#define NORMALIZE_WIDTH_AND_FLAGS(width, flags, precision)	\
	NORMALIZE_WIDTH(width, flags)				\
	if (precision >= 0) {					\
		flags['0'] = 0;					\
	}

#define EMIT_STR(str, len, cl, tlen)					\
	do{								\
		int	  i;						\
		for (i=0; i < len; i++) {				\
			tlen += put((unsigned char) *str++, cl);	\
		}							\
	} while (0)

int
fmt_puts(const char *str, int len,
	 int put(int c, void *cl), void *cl,
	 unsigned char flags[256], int width, int precision)
{
	int	  tlen = 0;
	
	assert(str);
	assert(len >= 0);
	assert(flags);

	NORMALIZE_WIDTH_AND_FLAGS(width, flags, precision);

	if (precision >= 0 &&
	    precision < len) {
		len = precision;
	}

	if (!flags['-']) {
		pad(width - len, ' ', tlen);
	}
	

	EMIT_STR(str, len, cl, tlen);

	return tlen;
}

int
fmt_putd(const char *str, int len,
	 int put(int c, void *cl), void *cl,
	 unsigned char flags[256], int width, int precision) 
{
	int	  sign;
	int	  tlen = 0;
	int	  n;
	
	assert(str);
	assert(len >= 0);
	assert(flags);

	NORMALIZE_WIDTH_AND_FLAGS(width, flags, precision);

	if (len > 0 &&
	    (*str == '-' ||
	     *str == '+')) {
		sign = *str++;
		len--;
	} else if (flags[' ']) {
		sign = ' ';
	} else {
		sign = 0;
	}

	/* justify */
	if (precision < 0) {
		precision = 1;
	}
		
	if (len < precision) {
		n = precision;
	} else if (precision == 0 &&
		   len == 1 &&
		   str[0] == '0') {
		n = 0;
	} else {
		n = len;
	}

	if (sign) {
		n++;
	}

	if (flags['-']) {
		if(sign) {
			tlen += put(sign, cl);
		}
	} else if (flags['0']) {
		if(sign) {
			tlen += put(sign, cl);
		}
		pad(width - n, '0', tlen);
	} else {
		if(sign) {
			tlen += put(sign, cl);
		}
		pad(width - n, ' ', tlen);
	}
		

	pad(precision - len, '0', tlen);
	
	EMIT_STR(str, len, cl, tlen);

	if(flags['-']) {
		pad(width - n, ' ', tlen);
	}

	return tlen;
}


fmt_ty
fmt_register(int code, fmt_ty newcvt)
{
	fmt_ty	  old;

	assert(0 < code && code < (int)sizeof_array(cvt));
	old = cvt[code];
	
	cvt[code] = newcvt;
	return old;
}





/* local funcs */

static int
outc(int c, void *cl)	
{
	FILE *f = cl;
	return putc(c, f);
}

static int
cvt_c(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision)
{
	int	  tlen = 0;
	
	NORMALIZE_WIDTH(width, flags);

	if(!flags['-']) {
		pad(width - 1, ' ', tlen);
	}
	tlen += put((unsigned char)va_arg(*app, int), cl);
	if(flags['-']) {
		pad(width - 1, ' ', tlen);
	}
	return tlen;
}


static int
cvt_d(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision)
{
	int		  val = va_arg(*app, int);
	unsigned int	  m;
	char		  buf[43];
	char		* p = buf + sizeof(buf);

	if(val == INT_MAX) {
		m = INT_MAX + 1U;
	} else if (val < 0) {
		m = -val;
	} else {
		m = val;
	}

	do {
		*--p = m % 10 + '0';
	} while ((m /= 10) > 0);
	
	if (val < 0) {
		*--p = '-';
	}
	return fmt_putd(p, (buf + sizeof(buf)) - p, put, cl,
			flags, width, precision);
}


static int
cvt_f(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision)
{
	char	  buf[DBL_MAX_10_EXP+1+1+99+1];
	static	char	  fmt[] = "%.dd?";
	
	if (precision < 0) {
		precision = 6;
	}

	if (code == 'g' && precision == 0) {
		precision = 1;
	}

	assert(precision <= 99);
	fmt[4] = code;
	fmt[3] = precision % 10 + '0';
	fmt[2] = (precision / 10) % 10 + '0';

	sprintf(buf, fmt, va_arg(*app, double));
	
	return fmt_putd(buf, strlen(buf), put, cl,
			flags, width, precision);
}

static int
cvt_o(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision)
{
	unsigned int	  m = va_arg(*app, unsigned int);
	char		  buf[43];
	char		* p = buf + sizeof(buf);

	do {
		*--p = (m & 0x7) + '0';
	} while ((m >>= 3) != 0);
	return fmt_putd(p, (buf + sizeof(buf)) - p,
			put, cl, flags, width, precision);
}

static int
cvt_p(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision)
{
	unsigned long	  m = (unsigned long)va_arg(*app, void *);
	char		  buf[43];
	char		* p = buf + sizeof(buf);

	precision = INT_MIN;

	do {
		*--p = "0123456789abcdef"[m & 0xf];
	} while((m >>= 4) != 0);

	return fmt_putd(p, (buf + sizeof(buf)) - p,
			put, cl, flags, width, precision);
}


static int
cvt_s(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision) 
{
	char	* str = va_arg(*app, char *);
	
	assert(str);

	return fmt_puts(str, strlen(str), put, cl, flags, width, precision);
}

static int
cvt_u(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision)
{
	unsigned int	  m = va_arg(*app, unsigned int);
	char		  buf[43];
	char		* p = buf + sizeof(buf);

	do {
		*--p = m % 10 + '0';
	} while((m /= 10) > 0);

	return fmt_putd(p, (buf + sizeof(buf)) - p,
			put, cl, flags, width, precision);
}

static int
cvt_x(int code, va_list *app,
      int put(int c, void *cl), void *cl,
      unsigned char flags[], int width, int precision)
{
	unsigned int	  m = va_arg(*app, unsigned int);
	char		  buf[43];
	char		* p = buf + sizeof(buf);

	do {
		*--p = "0123456789abcdef"[m & 0xf];
	} while((m >>= 4) != 0);

	return fmt_putd(p, (buf + sizeof(buf)) - p,
			put, cl, flags, width, precision);
}

static int
insert(int c, void *cl)
{
	struct buf	* p = cl;

	if (p->bp >= p->buf + p->size) {
		TRY_THROW(fmt_overflow);
	}

	*p->bp++ = c;

	return c;
}

static int
append(int c, void *cl)
{
	struct buf	* p = (struct buf *)cl;
	
	if(p->bp >= p->buf + p->size) {
		XREALLOC(unsigned char, p->buf, 2*p->size);
		p->bp = p->buf + p->size;
		p->size *= 2;
	}

	*p->bp++ = c;
	return c;
}
