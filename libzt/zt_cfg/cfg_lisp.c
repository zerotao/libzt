/*
 * Interface to lisp config files
 * Copyright (C) 2003-2004 Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: lisp.c,v 1.1 2003/11/26 17:37:16 jshiffer Exp $
 *
 */

#include "cfg_private.h"

#include "../zt.h"
#include "../zt_log.h"
#include "../zt_assert.h"

typedef struct cfg_lisp_ty cfg_lisp_ty;
struct cfg_lisp_ty {
	cfg_ty	  inherited;
	/* private vars MUST go after the inherited struct*/
	zlInterp  zl;
};
	
static cfg_vtbl_ty vtbl = {
	sizeof(cfg_lisp_ty),
	0,
	cfg_priv_destructor,
	cfg_priv_get,
	cfg_priv_set
};

cfg_ty *
cfg_lisp( file, opts )
char	* file;
int	  opts;
{
	cfg_ty	* cfg = cfg_new(&vtbl);
	cfg->filename = strdup(file);
	cfg->opts = opts;

	/* parse the config return NULL on error*/

	return cfg;
}

cfg_ty *
cfg_lisp2( cfg, file, opts )
cfg_ty	* cfg;
char	* file;
int	  opts;
{
	cfg_ty	* cfg = cfg_new(&vtbl);
	cfg->filename = strdup(file);
	cfg->opts = opts;
	
	return cfg;
}
