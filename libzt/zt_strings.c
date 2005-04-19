/*! \file zt_strings.c
 * \brief string manipulation functions
 *
 *
 */

/*
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 */


#include <config.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "zt_strings.h"
#include "zt_assert.h"
#include "zt_format.h"

/*! 
 * generate an index based on an index statement i and length len
 */
#define IDX(i, len) ((i) <= 0 ? (i) + (len) : (i) - 1)

/*!
 * update s, i and j 
 */
#define CONVERT(s, i, j) do {			\
		int	  len;			\
		assert(s);			\
		len = strlen(s);		\
		i = IDX(i, len);		\
		j = IDX(j, len);		\
		if (i > j) {			\
			int t = i;		\
			i = j;			\
			j = t;			\
		}				\
		assert(i >= 0 && j <= len);	\
	} while(0)

/*!
 * return a substring
 */
char *str_sub(const char *s, int i, int j)
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
 * return a string containing n cpoies of s[i:j] plus NUL
 */
char *str_dup(const char *s, int i, int j, int n)
{
	int	  k;
	char	* new, *p;

	assert(n >= 0);
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
 *
 */
char *str_cat(const char *s1, int i1, int j1,
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
 * 
 */
char *str_catv(const char *s, ...)
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
 * reverse s[i:j]
 */
char *str_reverse(const char *s, int i, int j)
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
 *
 */
char *str_map(const char *s, int i, int j,
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
		assert(*from == 0 && *to == 0);
	} else {
		assert(from == NULL && to == NULL && s);
		assert(map['a']);
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
 *
 */
int str_pos(const char *s, int i)
{
	int	  len;
	
	assert(s);
	len = strlen(s);
	i = IDX(i, len);
	assert(i >= 0 && i <= len);
	return i + 1;
}

/*!
 *
 */
int str_len(const char *s, int i, int j)
{
	CONVERT(s, i, j);
	return j - i;
}

/*!
 * return -1, 0, 1 if s1 is lexically less then, equal to or greater
 * then s2
 */
int str_cmp(const char *s1, int i1, int j1,
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
 * 
 */
int str_chr(const char *s, int i, int j, int c)
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
 *
 */
int str_rchr(const char *s, int i, int j, int c)
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
 *
 */
int str_upto(const char *s, int i, int j, const char *set)
{
	assert(set);
	CONVERT(s, i, j);
	for( ; i < j; i++) {
		if (strchr(set, s[i])) {
			return i + 1;
		}
	}
	
	return 0;
}

/*!
 *
 */
int str_rupto(const char *s, int i, int j, const char *set)
{
	assert(set);

	CONVERT(s, i, j);
	while (j > i) {
		if (strchr(set, s[--j])) {
			return j + 1;
		}
	}
	
	return 0;
}

/*!
 *
 */
int str_find(const char *s, int i, int j, const char *str)
{
	int	  len;

	CONVERT(s, i, j);
	assert(str);

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
 *
 */
int str_rfind(const char *s, int i, int j, const char *str)
{
	int	  len;

	CONVERT(s, i, j);
	assert(str);
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
 *
 */
int str_any(const char *s, int i, const char *set)
{
	int	  len;

	assert(s);
	assert(set);

	len = strlen(s);
	i = IDX(i, len);

	assert(i >= 0 && i <= len);
	if (i < len && strchr(set, s[i])) {
		return i + 2;
	}
	
	return 0;
}

/*!
 *
 */
int str_many(const char *s, int i, int j, const char *set)
{
	assert(set);

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
 *
 */
int str_rmany(const char *s, int i, int j, const char *set)
{
	assert(set);

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
int str_match(const char *s, int i, int j, const char *str)
{
	int	  len;

	CONVERT(s, i, j);
	assert(str);
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
int str_rmatch(const char *s, int i, int j, const char *str)
{
	int	  len;

	CONVERT(s, i, j);
	assert(str);
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
str_format(int code, va_list *app,
	   int put(int c, void *cl), void *cl,
	   unsigned char flags[], int width, int precision)
{
	char	* s;
	int	  i,
		  j;

	assert(app && flags);
	s = va_arg(*app, char *);
	i = va_arg(*app, int);
	j = va_arg(*app, int);

	CONVERT(s, i, j);

	fmt_puts(s + i, j - i, put, cl, flags, width, precision);
}

/* strip the \n from the end of a string */
char*
str_chomp (char *str)
{
	assert(str);
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
str_strip (char *str)
{
	assert(str);
	{
		/* strip whitespace from the beginning of the string */
		int len = 0;
		int nl = 0;
		len = strspn(str, WHITESPACE);
		memmove(str, &str[len], (strlen(str) - len) + 1); /* +1 captures \0 */

		/* strip whitespace from the end of the string */
		if(index(str, '\n') != NULL)
			nl = 1;
		len = str_rspn(str, WHITESPACE "\n");
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
str_rspn (const char *s,
	 const char *accept)
{
	assert(s);
	assert(accept);
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
str_rcspn (const char *s,
	  const char *reject)
{
	assert(s);
	assert(reject);
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
str_basename(char *npath,
	  int len,
	  const char *path,
	  const char *suffix)
{
	char *base = NULL;
	char *endp = NULL;
	char *suffixp = NULL;
	int base_len = 0;
	int path_len = 0;
  
	assert(npath);
	assert(path);
  
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
str_dirname (char *npath, int len, const char *path)
{
	char *dirname = NULL;
	int dir_len = 0;
	int path_len = 0;

	assert(npath);
	assert(path);
    
	memset(npath, '\0', len);

	path_len = strlen(path);
  
	dirname = rindex(path, PATH_SEPERATOR);

	if(!dirname)
		dirname = (char *)path + path_len;

	dir_len = dirname - path;
	memcpy(npath, path, MIN(dir_len, (len - 1)));
	return(npath);
}
