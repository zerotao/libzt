/****h* libZT/ProgName
 *
 * DESCRIPTION
 *   program name functions
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/

/*
 * Description: 
 */

#ifndef _ZT_PROGNAME_H_
#define _ZT_PROGNAME_H_

#include <libzt/zt.h>

/****d* ProgName/STRIP_DIR
 *  NAME
 *    STRIP_DIR
 *****/
#define STRIP_DIR 1

BEGIN_C_DECLS

extern char *zt_progname(char *, int);

END_C_DECLS
#endif /* _ZT_PROGNAME_H_ */
