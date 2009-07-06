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

/****t* Cfg/cfg_ty
 * NAME
 *   cfg_ty - opaque type associated with a config file/style
 * SOURCE
 */
typedef struct cfg_ty cfg_ty;
/*** Cfg/cfg_ty ***/


/****t* Cfg/cfg_type
 * NAME
 *   cfg_type - enumeration of supported variable types
 * SOURCE
 */
enum cfg_type {
  cfg_bool = 0,
  cfg_int,
  cfg_float,
  cfg_string,
  cfg_ref
};
typedef enum cfg_type cfg_type;
/*** Cfg/cfg_type ***/


/****f* Cfg/cfg_close [1.0]
 * NAME
 *   cfg_close - close the config associated with cfg
 * SYNOPSIS
 *   void = cfg_close(cfg)
 * INPUTS
 *  o cfg - pointer to a void cfg_ty
 * RESULT
 *   void
 * SEE ALSO
 *   cfg_get, cfg_set
 * NOTES
 *   the interface that users of this function get is initially a macro
 *   which expands to a call to the function of the same name after
 *   setting debug information for logging. 
 ****
 */
void cfg_close ( cfg_ty * );
#define cfg_close( cfg ) 	\
	ZT_LOG_DEBUG_INFO(NULL);	\
	cfg_close( cfg )

/****f* Cfg/cfg_get
 * NAME
 *   cfg_get - fetch the value of a config variable
 * SYNOPSIS
 *   error = cfg_get(cfg, space, name, value, type)
 *
 * DESCRIPTION
 *
 * INPUTS
 *   * cfg    - pointer to a valid cfg_ty
 *   * space  - string representation of a namespace to search for name
 *   * name   - string representation of a variable to get
 *   * value  - address of a variable to fill with the value
 *   * type   - type of the expected variable
 * RESULT
 *   error - return 0 on failure and >0 on success
 * SEE ALSO
 *   cfg_set, cfg_close
 * NOTES
 *   the interface that users of this function get is initially a macro
 *   which expands to a call to the function of the same name after
 *   setting debug information for logging.
 ****
 */
int cfg_get ( cfg_ty *cfg, char *block, char *name, void *value, cfg_type type);
#define cfg_get( cfg, block, name, addr, type )	\
	ZT_LOG_DEBUG_INFO(NULL);                       \
	cfg_get(cfg, block, name, addr, type)


/****f* Cfg/cfg_set
 * NAME
 *   cfg_set - set the value of a config variable
 * SYNOPSIS
 *   error = cfg_set(cfg, space, name, value, type)
 * INPUTS
 *   o cfg    - pointer to a valid cfg_ty 
 *   o space  - string representation of a namespace to use for name
 *   o name   - string representation of a variable to get
 *   o value  - address of a variable to to set from
 *   o type   - type of the expected variable
 * RESULT
 *   error - return 0 on failure and >0 on success
 * SEE ALSO
 *   cfg_get, cfg_close
 * NOTES
 *   the interface that users of this function get is initially a macro
 *   which expands to a call to the function of the same name after
 *   setting debug information for logging.
 ****
 */

int cfg_set ( cfg_ty *, char *, char *, void *, cfg_type );
#define cfg_set( cfg, block, name, addr, type )	\
	ZT_LOG_DEBUG_INFO(NULL);                       \
	cfg_set(cfg, block, name, addr, type)

END_C_DECLS
#include <libzt/zt_cfg/cfg_interface.h>

#endif  /* _ZT_CFG_H_ */
