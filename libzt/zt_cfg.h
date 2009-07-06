/****h* libZT/Cfg
 * NAME
 *   zt_cfg.h
 * DESCRIPTION
 *   generic configuration file support
 * COPYRIGHT
 *   Copyright (C) 2000-2002, 2004-2005 Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 *   See file COPYING for details.
 ****/
#ifndef _ZT_CFG_H_
#define _ZT_CFG_H_

#include <libzt/zt.h>
#include <libzt/zt_log.h>

BEGIN_C_DECLS

/****t* Cfg/zt_cfg_ty
 * NAME
 *   zt_cfg_ty - opaque type associated with a config file/style
 * SOURCE
 */
typedef struct zt_cfg_ty zt_cfg_ty;
/*** Cfg/zt_cfg_ty ***/


/****t* Cfg/zt_cfg_type
 * NAME
 *   zt_cfg_type - enumeration of supported variable types
 * SOURCE
 */
enum zt_cfg_type {
    zt_cfg_bool = 0,
    zt_cfg_int,
    zt_cfg_float,
    zt_cfg_string,
    zt_cfg_ref
};
typedef enum zt_cfg_type zt_cfg_type;
/*** Cfg/zt_cfg_type ***/


/****f* Cfg/zt_cfg_close [1.0]
 * NAME
 *   zt_cfg_close - close the config associated with cfg
 * SYNOPSIS
 *   void = zt_cfg_close(cfg)
 * INPUTS
 *  o cfg - pointer to a void zt_cfg_ty
 * RESULT
 *   void
 * SEE ALSO
 *   zt_cfg_get, zt_cfg_set
 * NOTES
 *   the interface that users of this function get is initially a macro
 *   which expands to a call to the function of the same name after
 *   setting debug information for logging. 
 ****
 */
void zt_cfg_close ( zt_cfg_ty * );
#define zt_cfg_close( cfg )                     \
	ZT_LOG_DEBUG_INFO(NULL);                    \
	zt_cfg_close( cfg )

/****f* Cfg/zt_cfg_get
 * NAME
 *   zt_cfg_get - fetch the value of a config variable
 * SYNOPSIS
 *   error = zt_cfg_get(cfg, space, name, value, type)
 *
 * DESCRIPTION
 *
 * INPUTS
 *   * cfg    - pointer to a valid zt_cfg_ty
 *   * space  - string representation of a namespace to search for name
 *   * name   - string representation of a variable to get
 *   * value  - address of a variable to fill with the value
 *   * type   - type of the expected variable
 * RESULT
 *   error - return 0 on failure and >0 on success
 * SEE ALSO
 *   zt_cfg_set, zt_cfg_close
 * NOTES
 *   the interface that users of this function get is initially a macro
 *   which expands to a call to the function of the same name after
 *   setting debug information for logging.
 ****
 */
int zt_cfg_get ( zt_cfg_ty *cfg, char *block, char *name, void *value, zt_cfg_type type);
#define zt_cfg_get( cfg, block, name, addr, type )	\
	ZT_LOG_DEBUG_INFO(NULL);                        \
	zt_cfg_get(cfg, block, name, addr, type)


/****f* Cfg/zt_cfg_set
 * NAME
 *   zt_cfg_set - set the value of a config variable
 * SYNOPSIS
 *   error = zt_cfg_set(cfg, space, name, value, type)
 * INPUTS
 *   o cfg    - pointer to a valid zt_cfg_ty 
 *   o space  - string representation of a namespace to use for name
 *   o name   - string representation of a variable to get
 *   o value  - address of a variable to to set from
 *   o type   - type of the expected variable
 * RESULT
 *   error - return 0 on failure and >0 on success
 * SEE ALSO
 *   zt_cfg_get, zt_cfg_close
 * NOTES
 *   the interface that users of this function get is initially a macro
 *   which expands to a call to the function of the same name after
 *   setting debug information for logging.
 ****
 */

int zt_cfg_set ( zt_cfg_ty *, char *, char *, void *, zt_cfg_type );
#define zt_cfg_set( cfg, block, name, addr, type )	\
	ZT_LOG_DEBUG_INFO(NULL);                        \
	zt_cfg_set(cfg, block, name, addr, type)

END_C_DECLS
#include <libzt/zt_cfg/cfg_interface.h>

#endif  /* _ZT_CFG_H_ */
