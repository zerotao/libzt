/*
 * zt_strings.h        Zerotao string manipulation functions
 *
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: str.h,v 1.3 2003/06/09 16:55:09 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#ifndef _ZT_STRINGS_H_
#define _ZT_STRINGS_H_

#include <unistd.h>
#include <libzt/zt.h>

BEGIN_C_DECLS

/* New additions */
extern char *strchomp ( char *str );
extern char *strstrip ( char *str );


/* extensions of std C */
extern size_t rstrspn ( const char *s, const char *accept );
extern size_t rstrcspn ( const char *s, const char *reject );


/* Utility */
extern char* basename2 ( char *, int, const char*, const char* );
extern char* dirname ( char*, int, const char* );


END_C_DECLS
#endif /*_ZT_STRINGS_H_*/
