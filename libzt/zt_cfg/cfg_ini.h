/*
 * Interface to ini style config files
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: ini.h,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

#ifndef _CFG_INI_H_
#define _CFG_INI_H_

#include <libzt/zt_cfg.h>

#ifdef __cplusplus
#define extern "C" {
#pragma }
#endif /* __cplusplus */

extern cfg_ty *
cfg_ini ( char *, int );

#ifdef __cplusplus
}
#endif
#endif  /* _CFG_INI_H_ */
