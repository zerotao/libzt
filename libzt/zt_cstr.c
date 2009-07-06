/*! \file zt_strings.c
 * \brief string manipulation functions
 *
 *
 */

/*
 * Copyright (C) 2000-2006, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 */


#include <config.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "zt_cstr.h"
#include "zt_assert.h"
#include "zt_format.h"
#include "zt_log.h"

/*! 
 * generate an index based on an index statement i and length len
 */
#define IDX(i, len) ((i) <= 0 ? (i) + (len) : (i) - 1)

/*!
 * update s, i and j 
 */
#define CONVERT(s, i, j) do {			\
		int	  len;			\
		zt_assert(s);			\
		len = strlen(s);		\
		i = IDX(i, len);		\
		j = IDX(j, len);		\
		if (i > j) {			\
			int t = i;		\
			i = j;			\
			j = t;			\
		}				\
		zt_assert(i >= 0 && j <= len);	\
	} while(0)

/*!
 * return a newly allocated substring
 */
char *zt_cstr_sub(const char *s, int i, int j)
{
	char	* new;
	char	* p;

	CONVERT(s, i, j);
	
	new = XMALLOC(char, j - i + 1);

	p = new;
	
	while(i < j) {
		*p++ = s[i++];
	}
	*p = '\0';
	return new;
}

/*!
 * return a newley allocated string containing n copies of s[i:j] plus NUL
 */
char *zt_cstr_dup(const char *s, int i, int j, int n)
{
	int	  k;
	char	* new, *p;

	zt_assert(n >= 0);
	CONVERT(s, i, j);

	p = new = XMALLOC(char, n*(j - i) + 1);

	if (j - i > 0) {
		while (n-- > 0) {
			for (k = i; k < j; k++) {
				*p++ = s[k];
			}
		}
	}

	*p = '\0';

	return new;
}

/*!
 * return a newly allocated string containing s1[i1:j1]s2[i2:j2]
 */
char *zt_cstr_cat(const char *s1, int i1, int j1,
		  const char *s2, int i2, int j2)
{
	char	* new,
		* p;

	CONVERT(s1, i1, j1);
	CONVERT(s2, i2, j2);

	p = new = XMALLOC(char, j1 - i1 + j2 - i2 + 1);
	
	while(i1 < j1) {
		*p++ = s1[i1++];
	}

	while(i2 < j2) {
		*p++ = s2[i2++];
	}

	*p = '\0';
	return new;
}

/*!
 * return a newly allocated string containing s[i:j]s1[i1:j1]...sn[in:jn] 
 */
char *zt_cstr_catv(const char *s, ...)
{
	char		* new,
		        * p;
	const char	* save = s;
	int		  i,
		          j,
		          len = 0;
	va_list	          ap;

	va_start(ap, s);

	while (s) {
		i = va_arg(ap, int);
		j = va_arg(ap, int);
		CONVERT(s, i, j);
		len += j - i;
		s = va_arg(ap, const char *);
	}

	va_end(ap);

	p = new = XMALLOC(char, len + 1);
	s = save;
	va_start(ap, s);

	while (s) {
		i = va_arg(ap, int);
		j = va_arg(ap, int);
		CONVERT(s, i, j);
		while (i < j) {
			*p++ = s[i++];
		}
		s = va_arg(ap, const char *);
	}
	
	*p = '\0';
	return new;
}

/*!
 * returns a newly allocated string containing the reverse of s[i:j]
 */
char *zt_cstr_reverse(const char *s, int i, int j)
{
	char	* new,
		* p;
	
	CONVERT(s, i, j);

	p = new = XMALLOC(char, j - i + 1);

	while (j > i) {
		*p++ = s[--j];
	}

	*p = '\0';
	return new;
}

/*!
 * returns a newly allocated string containing convert(s[i:j], from, to) where the chars in from are
 * converted to the corrisponding chars in to.
 * returns NULL on failure
 */
char *zt_cstr_map(const char *s, int i, int j,
		  const char *from, const char *to)
{
	static char map[256] = { 0 };

	if (from && to) {
		unsigned int	  c;
		for (c = 0; c < sizeof(map); c++) {
			map[c] = c;
		}
		while(*from && *to) {
			map[(unsigned char)*from++] = *to++;
		}
		zt_assert(*from == 0 && *to == 0);
	} else {
		zt_assert(from == NULL && to == NULL && s);
		zt_assert(map['a']);
	}

	if (s) {
		char	* new,
			* p;
		CONVERT(s, i, j);
		p = new = XMALLOC(char, j - i + 1);
		while (i < j) {
			*p++ = map[(unsigned char)s[i++]];
		}
		*p = '\0';
		return new;
	}
	
	return NULL;
}

/*!
 * returns the index into s that i corrisponds to.
 */
int zt_cstr_pos(const char *s, int i)
{
	int	  len;
	
	zt_assert(s);
	len = strlen(s);
	i = IDX(i, len);
	zt_assert(i >= 0 && i <= len);
	return i + 1;
}

/*!
 * returns the length of the slice represented by [i:j]
 */
int zt_cstr_len(const char *s, int i, int j)
{
	CONVERT(s, i, j);
	return j - i;
}

/*!
 * return -1, 0, 1 if s1[i1:j1] is lexically less then, equal to or greater
 * then s2[i2:j2]
 */
int zt_cstr_cmp(const char *s1, int i1, int j1,
		const char *s2, int i2, int j2)
{
	CONVERT(s1, i1, j1);
	CONVERT(s2, i2, j2);

	s1 += i1;
	s2 += i2;

	if (j1 - i1 < j2 - i2) {
		int cond = strncmp(s1, s2, j1 - i1);
		return cond == 0 ? -1 : cond;
	} else if (j1 - i1 > j2 - i2) {
		int cond = strncmp(s1, s2, j2 - i2);
		return cond == 0 ? +1 : cond;
	}

	return strncmp(s1, s2, j1 - i1);
}

/*!
 * locates the first occurrence of c in the string referenced by s[i:j]
 */
int zt_cstr_chr(const char *s, int i, int j, int c)
{
	CONVERT(s, i, j);
	for ( ; i < j; i++) {
		if (s[i] == c) {
			return i + 1;
		}
	}
	
	return 0;
}
/*!
 * locates the last occurrence of c in the string referenced by s[i:j]
 */
int zt_cstr_rchr(const char *s, int i, int j, int c)
{
	CONVERT(s, i, j);
	while (j > i) {
		if (s[--j] == c) {
			return j + 1;
		}
	}
	
	return 0;
}

/*!
 * locates the first occurrence of any char in set in the string referenced by s[i:j]
 */
int zt_cstr_upto(const char *s, int i, int j, const char *set)
{
	zt_assert(set);
	CONVERT(s, i, j);
	for( ; i < j; i++) {
		if (strchr(set, s[i])) {
			return i + 1;
		}
	}
	
	return 0;
}

/*!
 * locates the last occurrence of any char in set in the string referenced by s[i:j]
 */
int zt_cstr_rupto(const char *s, int i, int j, const char *set)
{
	zt_assert(set);

	CONVERT(s, i, j);
	while (j > i) {
		if (strchr(set, s[--j])) {
			return j + 1;
		}
	}
	
	return 0;
}

/*!
 * locates the first occurrence of the string str in the string referenced by s[i:j]
 */
int zt_cstr_find(const char *s, int i, int j, const char *str)
{
	int	  len;

	CONVERT(s, i, j);
	zt_assert(str);

	len = strlen(str);

	if (len == 0) {
		return i + 1;
	} else if (len == 1) {
		for ( ; i < j; i++) {
			if (s[i] == *str) {
				return i + 1;
			}
		}
	}
	
	for ( ; i + len <= j; i++) {
		if (strncmp(&s[i], str, len) == 0) {
			return i + 1;
		}
	}
	
	return 0;
}

/*!
 * locates the first occurrence of the string str in the string referenced by s[i:j]
 */
int zt_cstr_rfind(const char *s, int i, int j, const char *str)
{
	int	  len;

	CONVERT(s, i, j);
	zt_assert(str);
	len = strlen(str);

	if (len == 0) {
		return j + 1;
	} else if (len == 1) {
		while (j > i) {
			if (s[--j] == *str) {
				return j + 1;
			}
		}
	}

	for ( ; j - len >= i; j--) {
		if (strncmp(&s[j-len], str, len) == 0) {
			return j - len + 1;
		}
	}
	
	return 0;
}

/*!
 * locates the first occurrence of any char in set in the string referenced by s[i]
 */
int zt_cstr_any(const char *s, int i, const char *set)
{
	int	  len;

	zt_assert(s);
	zt_assert(set);

	len = strlen(s);
	i = IDX(i, len);

	zt_assert(i >= 0 && i <= len);
	if (i < len && strchr(set, s[i])) {
		return i + 2;
	}
	
	return 0;
}

/*!
 * locates the first occurrence of any string not in set in the string referenced by s[i:j]
 */
int zt_cstr_many(const char *s, int i, int j, const char *set)
{
	zt_assert(set);

	CONVERT(s, i, j);

	if (i < j && strchr(set, s[i])) {
		do {
			i++;
		} while(i < j && strchr(set, s[i]));
		return i + 1;
	}
	
	return 0;
}

/*!
 * locates the last occurrence of any string not in set in the string referenced by s[i:j]
 */
int zt_cstr_rmany(const char *s, int i, int j, const char *set)
{
	zt_assert(set);

	CONVERT(s, i, j);
	if (j > i && strchr(set, s[j-1])) {
		do {
			--j;
		} while(j >= i && strchr(set, s[j]));

		return j + 2;
	}
	
	return 0;
}

/*!
 * 
 */
int zt_cstr_match(const char *s, int i, int j, const char *str)
{
	int	  len;

	CONVERT(s, i, j);
	zt_assert(str);
	len = strlen(str);

	if (len == 0) {
		return i + 1;
	} else if (len == 1) {
		if (i < j && s[i] == *str) {
			return i + 2;
		}
	} else if (i + len <= j && (strncmp(&s[i], str, len) == 0)) {
		return i + len + 1;
	}
	
	return 0;
}

/*!
 *
 */
int zt_cstr_rmatch(const char *s, int i, int j, const char *str)
{
	int	  len;

	CONVERT(s, i, j);
	zt_assert(str);
	len = strlen(str);
	
	if (len == 0) {
		return j + 1;
	} else if (len == 1) {
		if (j > i && s[j-1] == *str) {
			return j;
		}
	} else if (j - len >= i &&
		   strncmp(&s[j-len], str, len) == 0) {
		return j - len + 1;
	}
		
	return 0;
}

/*!
 *
 */
int
zt_put_cstr(int code, va_list *app,
	   int put(int c, void *cl), void *cl,
	   unsigned char flags[], int width, int precision)
{
	char	* s;
	int	  i,
		  j;

	zt_assert(app && flags);
	
	s = va_arg(*app, char *);
	i = va_arg(*app, int);
	j = va_arg(*app, int);

	CONVERT(s, i, j);

	return fmt_puts(s + i, j - i, put, cl, flags, width, precision);
}

/* strip the \n from the end of a string */
char*
zt_cstr_chomp (char *str)
{
	zt_assert(str);
	{
		int i = 0;
		i = strlen(str);
		if(str[i-1] == '\n')
			str[i-1] = '\0';
	}
	return (str);  
}

/* Strip white space characters from the front and back of the string */
char*
zt_cstr_strip (char *str)
{
	zt_assert(str);
	{
		/* strip whitespace from the beginning of the string */
		int len = 0;
		int nl = 0;
		len = strspn(str, WHITESPACE);
		memmove(str, &str[len], (strlen(str) - len) + 1); /* +1 captures \0 */

		/* strip whitespace from the end of the string */
		if(index(str, '\n') != NULL)
			nl = 1;
		len = zt_cstr_rspn(str, WHITESPACE "\n");
		len = strlen(str) - len;
		if(len){
			if(nl)
				str[len++] = '\n';
			str[len] = '\0';
		}
	}
	return (str);
}

size_t
zt_cstr_rspn (const char *s,
	 const char *accept)
{
	zt_assert(s);
	zt_assert(accept);
	{
		int len = strlen(s);
		int i = 0;
		for (i = len-1; i > 0; i--){ /* -1 to skip \0 */
			if(strspn(&s[i], accept) == 0)
				return ((len - i) - 1);	/* -1 for the first non matching char */
		}
		return len;
	}
	/* never reached */
	return 0;
}

size_t
zt_cstr_rcspn (const char *s,
	  const char *reject)
{
	zt_assert(s);
	zt_assert(reject);
	{
		int len = strlen(s);
		int i = 0;
		for (i = len-1; i > 0; i--){ /* -1 to skip \0 */
			if(strcspn(&s[i], reject) == 0)
				return ((len - i) - 1);	/* -1 for the first non matching char */
		}
		return len;
	}
	/* never reached */
	return 0;
}

char*
zt_cstr_basename(char *npath,
	  int len,
	  const char *path,
	  const char *suffix)
{
	char *base = NULL;
	char *endp = NULL;
	char *suffixp = NULL;
	int base_len = 0;
	int path_len = 0;
  
	zt_assert(npath);
	zt_assert(path);
  
	memset(npath, '\0', len);
	path_len = strlen(path);
  
	base = rindex(path, PATH_SEPERATOR);
	if(!base)
		base = (char *)path; /* point to the beginning of path */
	else
		base++;
  
	endp = (char *)path + path_len; /* point to the end of path */
  
	if(suffix){
		suffixp = strstr(base, suffix);
		base_len = suffixp - base;
	}else{
		base_len = endp - base;
	}
  
	memcpy(npath, base, MIN(base_len, (len - 1))); /* copy the basename */
	return(npath);
}


char *
zt_cstr_dirname (char *npath, int len, const char *path)
{
	char *dirname = NULL;
	int dir_len = 0;
	int path_len = 0;

	zt_assert(npath);
	zt_assert(path);
    
	memset(npath, '\0', len);

	path_len = strlen(path);
  
	dirname = rindex(path, PATH_SEPERATOR);

	if(!dirname)
		dirname = (char *)path + path_len;

	dir_len = dirname - path;
	memcpy(npath, path, MIN(dir_len, (len - 1)));
	return(npath);
}


char *
zt_cstr_path_append(const char *path1, const char *path2) 
{
	char	* rpath;
	int		  len1;
	int		  len2;
	
	len1 = strlen(path1);
	len2 = strlen(path2);
	
	rpath = XMALLOC(char, len1 + len2 + 2);

	memcpy(rpath, path1, len1);
	rpath[len1] = PATH_SEPERATOR;
	memcpy(rpath+len1+1, path2, len2);
	
	return rpath;
}

/* converts binary data to a hex string
 * it does not NULL terminate the string
 */
size_t
zt_binary_to_hex(void *data, size_t dlen, char *hex, size_t hlen) 
{
	size_t		  n;
	char		* dptr = hex;
	
	for(n=0; (n < dlen) && ((n * 2) < hlen); n++, dptr+=2) {
		uint8_t		  c = ((uint8_t *)data)[n];
		if(hex != NULL) {
			dptr[0] = HEX_DIGITS[((c >> 4) & 0xF)];
			dptr[1] = HEX_DIGITS[(c & 0xF)];
		}
	}	
	return n * 2;
}

static int8_t hex_to_char(char hex) 
{
	uint8_t		  c = hex;
	
	if (c >= '0' && c <= '9') {
		c = c - 48;
	} else if (c >= 'A' && c <= 'F') {
		c = c - 55;  /* 65 - 10 */
	} else if (c >= 'a' && c <= 'f') {
		c = c - 87;  /* 97 - 10 */
	} else {
		return -1;
	}
	return c;
}

/*
 * convert a hex string to binary
 * hex - points to a (possibly) null terminated hex string
 * hlen - amount of hex to process
 * data - points to the location to store the converted data which
 *        should be 2 times the size of the hex values in hex
 * dlen = holds the length of data
 *
 * returns - -1 on error and number of bytes in data on success
 * --
 * if called with a NULL data will return the number of bytes required
 * to convert all data in hex.
 */
size_t
zt_hex_to_binary(char *hex, size_t hlen, void *data, size_t dlen) 
{
	size_t		  n;
	size_t		  y;

	if (data == NULL) {
		dlen = -1;
	}
	
	for(n=0, y=0; n < hlen && *hex != '\0' && y < dlen; n++) {
		int8_t		  c = hex_to_char(*hex++);
		int8_t		  c2;
		int8_t		  cc = 0;

		if (c == -1) {
			continue;
		}
		
		if ((c2 = hex_to_char(*hex++)) == -1) {
			zt_log_printf(zt_log_err, "ivalid hex value %c%c", hex[-2], hex[-1]);
			return -1;
		} else {
			n++;
		}
		
		cc = (c << 4) | (c2 & 0xF);
		
		if (data != NULL) {
			((char *)data)[y] = (char)cc;
		}
		y++;
	}
	return y;
}
