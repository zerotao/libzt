/*
 * zt_strings.h        Zerotao string manipulation functions
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
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

#ifdef __cplusplus
extern "C" {
#pragma }
#endif

extern char *strchomp _(( char *str ));
extern char *strstrip _(( char *str ));
extern size_t rstrspn _(( const char *s, const char *accept ));
extern size_t rstrcspn _(( const char *s, const char *reject ));

extern char* basename2 _(( char *, int, const char*, const char* ));
extern char* dirname _(( char*, int, const char* ));


#ifdef __cplusplus
}
#endif

#endif /*_ZT_STRINGS_H_*/
