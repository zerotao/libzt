#ifndef _ZT_CFG_H_
#define _ZT_CFG_H_

#include <libzt/zt.h>
#include <libzt/zt_log.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

typedef enum cfg_type cfg_type;
enum cfg_type {
  cfg_bool = 0,
  cfg_int,
  cfg_float,
  cfg_string,
  cfg_ref
};

typedef struct cfg_ty cfg_ty;

/** f
 * closes an open config
 * @param: cfg_ty * : pointer to a valid cfg_ty
 * @return: void 
 * @usage: 
 * cfg_close(cfg);
 */
void cfg_close _(( cfg_ty * ));
#define cfg_close( cfg ) 	\
	LOG_DEBUG_INFO(NULL);	\
	cfg_close( cfg )

/** f
 * get a variable from a cfg_ty
 * @param: cfg_ty * : pointer to a valid cfg_ty
 * @param: char * : string with the name of a block
 * @param: char * : string with the name of a variable
 * @param: void * : address of a variable to fill with the value 
 * @param: cfg_type : type of the expected variable
 * @return: 0 on success 1 on failure 
 * @usage: 
 * cfg_get ( cfg, "main", "test1", &local_test, cfg_int );
 */
int cfg_get _(( cfg_ty *, char *, char *, void *, cfg_type ));
#define cfg_get( cfg, block, name, addr, type )	\
	LOG_DEBUG_INFO(NULL);			\
	cfg_get(cfg, block, name, addr, type)

/** f
 * set a variable in cfg
 * @param: cfg_ty * : pointer to a valid cfg_ty structure
 * @param: char * : string with the name of a block
 * @param: char * : string with the name of a variable
 * @param: void * : address of a variable to fill the value with
 * @param: cfg_type : type of the expected variable
 * @return: 0 on success 1 on failure 
 * @usage: 
 * 
 */
int cfg_set _(( cfg_ty *, char *, char *, void *, cfg_type ));
#define cfg_set( cfg, block, name, addr, type )	\
	LOG_DEBUG_INFO(NULL);			\
	cfg_set(cfg, block, name, addr, type)

#ifdef __cplusplus
}
#endif

#include <libzt/zt_cfg/cfg_interface.h>

#endif  /* _ZT_CFG_H_ */
