/* private interface to cfg
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: cfg_private.h,v 1.3 2003/06/09 16:55:09 jshiffer Exp $
 *
 */

/* Private interface to cfg
 * You should NOT access this directly
 * unless you are extending the component
 */
#ifndef _ZT_CFG_PRIVATE_H_
#define _ZT_CFG_PRIVATE_H_

#include <stdlib.h>
#include <libzt/zt_cfg.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

struct zt_cfg_value_ty {
	char* name;
	zt_cfg_type type;
	union {
		short int   b;
		long   		i;
		double 		f;
		char*  		s;
		struct zt_cfg_value_ty* r;
	} v;
	int altered;
	struct zt_cfg_value_ty* next;
};

struct zt_cfg_block_ty {
	char *name;
	struct zt_cfg_value_ty* head;
	struct zt_cfg_block_ty* next;
};

struct zt_cfg_ty {
  struct zt_cfg_vtbl_ty *vtbl;
  /* rest of opts */
	char *filename;
	int numentries;
	int opts;	
	struct zt_cfg_block_ty* head;
};
  
typedef struct zt_cfg_vtbl_ty zt_cfg_vtbl_ty;
struct zt_cfg_vtbl_ty {
	size_t size;
	int opts;
	/* virtual function pointers */
	void (* destructor)(zt_cfg_ty *);
	int  (* get)(zt_cfg_ty *, char *, char *, void *, zt_cfg_type);
	int  (* set)(zt_cfg_ty *, char *, char *, void *, zt_cfg_type);
};

struct cfg_bvv_ty {
	char* block;
	char* variable;
	char* value;
	int line;
};

int zt_cfg_priv_set ( zt_cfg_ty *cfg, char *block_name, char *variable_name, void *var, zt_cfg_type type );
int zt_cfg_priv_get ( zt_cfg_ty *cfg, char *block_name, char *variable_name, void *var, zt_cfg_type type );
void zt_cfg_priv_destructor ( zt_cfg_ty *cfg );

void cfg_discard_whitespace(FILE* file);
void cfg_discard_line(FILE* file);
int cfg_insert_bvv(struct zt_cfg_ty* cfg, struct cfg_bvv_ty* bvv);

zt_cfg_ty *zt_cfg_new (zt_cfg_vtbl_ty *);
#ifdef __cplusplus
}
#endif
#endif  /* _ZT_CFG_PRIVATE_H_ */
