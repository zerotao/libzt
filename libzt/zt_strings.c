/*
 * zt_strings.c        Zerotao string manipulation functions
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: str.c,v 1.3 2003/11/26 17:37:16 jshiffer Exp $
 *
 */

/*
 * Description: 
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

/* strip the \n from the end of a string */
char*
strchomp ( str )
     char *str;
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
strstrip ( str )
     char *str;
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
    len = rstrspn(str, WHITESPACE "\n");
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
rstrspn ( s, accept )
     const char *s;
     const char *accept;
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
rstrcspn ( s, reject )
     const char *s;
     const char *reject;
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
basename2(npath, len, path, suffix)
     char *npath;
     int len;
     const char *path;
     const char *suffix;
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
dirname ( npath, len, path )
     char *npath;
     int len;
     const char *path;
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
