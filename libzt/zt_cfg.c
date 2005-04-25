/****ih* Cfg/IMPL
 * NAME
 *   zt_cfg.c
 * DESCRIPTION
 *   config file abstraction
 * COPYRIGHT
 *   Copyright (C) 2000-2002, 2004-2005, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 *   See file COPYING for details.
 ****/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "zt_cfg.h"
#include "zt_cfg/cfg_private.h"



#undef cfg_close
void cfg_close ( cfg_ty *cfg )
{
	if(cfg && cfg->vtbl->destructor)
		cfg->vtbl->destructor(cfg);
	return;
}

#undef cfg_get
int cfg_get ( cfg_ty	* cfg,
	      char 	* block,
	      char 	* name,
	      void 	* value,
	      cfg_type 	  type )
{
	if(cfg && cfg->vtbl->get) 
		return cfg->vtbl->get(cfg, block, name, value, type);
	return 0;
}

#undef cfg_set
int cfg_set ( cfg_ty	* cfg,
	      char 	* block,
	      char 	* name,
	      void 	* value,
	      cfg_type 	  type )
{
	if(cfg && cfg->vtbl->set) 		
		return cfg->vtbl->set(cfg, block, name, value, type);
	return 0;
}
