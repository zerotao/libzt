/*!
 * Filename: zt_cfg.h
 * Description: config file abstraction
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "zt.h"
#include "zt_cfg.h"
#include "zt_cfg/cfg_private.h"



#undef zt_cfg_close
void zt_cfg_close( zt_cfg_ty *cfg )
{
    if (cfg && cfg->vtbl->destructor) {
        cfg->vtbl->destructor(cfg);
    }
    return;
}

#undef zt_cfg_get
int
zt_cfg_get( zt_cfg_ty *cfg, char *block, char *name, void * value, zt_cfg_type type )
{
    if (cfg && cfg->vtbl->get) {
        return cfg->vtbl->get(cfg, block, name, value, type);
    }
    return 0;
}

#undef zt_cfg_set
int zt_cfg_set( zt_cfg_ty *cfg, char *block, char *name, void *value, zt_cfg_type type )
{
    if (cfg && cfg->vtbl->set) {
        return cfg->vtbl->set(cfg, block, name, value, type);
    }
    return 0;
}
