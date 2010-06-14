/*!
 * Filename: zt_cfg.h
 * Description: generic configuration file support
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_CFG_H_
#define _ZT_CFG_H_

#include <libzt/zt.h>
#include <libzt/zt_log.h>

BEGIN_C_DECLS

typedef struct zt_cfg_ty zt_cfg_ty;

enum zt_cfg_type {
    zt_cfg_bool = 0,
    zt_cfg_int,
    zt_cfg_float,
    zt_cfg_string,
    zt_cfg_ref
};
typedef enum zt_cfg_type zt_cfg_type;


/*!
 * Name: zt_cfg_close
 * Description: close a config descriptor
 */
void zt_cfg_close( zt_cfg_ty * );
#define zt_cfg_close( cfg )                     \
    ZT_LOG_DEBUG_INFO(NULL);                    \
    zt_cfg_close( cfg )

/*!
 * Name: zt_cfg_get
 * Description: fetch the value of a configuration variable
 * Result: returns 0 on failure and >0 on success
 */
int zt_cfg_get( zt_cfg_ty *cfg, char *block, char *name, void *value, zt_cfg_type type);
#define zt_cfg_get( cfg, block, name, addr, type )    \
    ZT_LOG_DEBUG_INFO(NULL), zt_cfg_get(cfg, block, name, addr, type)

#define zt_cfg_get_int( cfg, block, name, addr ) \
    zt_cfg_get( cfg, block, name, addr, zt_cfg_int )
#define zt_cfg_get_bool( cfg, block, name, addr ) \
    zt_cfg_get( cfg, block, name, addr, zt_cfg_bool )
#define zt_cfg_get_float( cfg, block, name, addr ) \
    zt_cfg_get( cfg, block, name, addr, zt_cfg_float )
#define zt_cfg_get_string( cfg, block, name, addr ) \
    zt_cfg_get( cfg, block, name, addr, zt_cfg_string )

/*!
 * Name: zt_cfg_set
 * Description: set the value of a configuration varaible
 * Result: returns 0 on failure and >0 on success
 */
int zt_cfg_set( zt_cfg_ty *, char *, char *, void *, zt_cfg_type );
#define zt_cfg_set( cfg, block, name, addr, type )    \
    ZT_LOG_DEBUG_INFO(NULL), zt_cfg_set(cfg, block, name, addr, type)

#define zt_cfg_set_int( cfg, block, name, addr ) \
    zt_cfg_set( cfg, block, name, addr, zt_cfg_int)
#define zt_cfg_set_bool( cfg, block, name, addr ) \
    zt_cfg_set( cfg, block, name, addr, zt_cfg_bool)
#define zt_cfg_set_float( cfg, block, name, addr ) \
    zt_cfg_set( cfg, block, name, addr, zt_cfg_float)
#define zt_cfg_set_string( cfg, block, name, addr ) \
    zt_cfg_set( cfg, block, name, addr, zt_cfg_string)

END_C_DECLS
#include <libzt/zt_cfg/cfg_interface.h>

#endif  /* _ZT_CFG_H_ */
