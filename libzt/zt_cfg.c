/*
 * zt_cfg.c        config file abstraction
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: cfg.c,v 1.2 2003/06/09 13:42:13 jshiffer Exp $
 *
 */

/*
 * Description: 
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "zt_cfg.h"
#include "zt_cfg/cfg_private.h"

void
cfg_close ( cfg )
	cfg_ty *cfg;
{
	if(cfg && cfg->vtbl->destructor)
		cfg->vtbl->destructor(cfg);
	return;
}

int
cfg_get ( cfg, block, name, value, type )
	cfg_ty *cfg;
	char *block;	 
	char *name;
	void *value;
	cfg_type type;
{
	if(cfg && cfg->vtbl->get) 
		return cfg->vtbl->get(cfg, block, name, value, type);
	return 0;
}

int
cfg_set ( cfg, block, name, value, type )
	cfg_ty *cfg;
	char *block;	 
	char *name;
	void *value;
	cfg_type type;
{
	if(cfg && cfg->vtbl->set) 		
		return cfg->vtbl->set(cfg, block, name, value, type);
	return 0;
}
