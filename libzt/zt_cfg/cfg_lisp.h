/*
 * Interface to lisp config files
 * Copyright (C) 2003-2004 Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: lisp.h,v 1.1 2003/11/26 17:37:16 jshiffer Exp $
 *
 */
#ifndef _ZT_CFG_LISP_H_
#define _ZT_CFG_LISP_H_

#include <libzt/zt_cfg.h>

#ifndef __cplusplus
#define extern "C" {
#pragma }
#endif	/* __cplusplus */


extern cfg_ty *
cfg_lisp ( char *, int );

/*
 * same as cfg_list only reused the interperter
 */
extern cfg_ty *
cfg_list2( cfg_ty *, char *, int);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* _ZT_CFG_LISP_H_ */
