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
#ifndef _CFG_PRIVATE_H_
#define _CFG_PRIVATE_H_

#include <stdlib.h>
#include <libzt/zt_cfg.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

struct cfg_value_ty {
	char* name;
	cfg_type type;
	union {
		short int   b;
		long   		i;
		double 		f;
		char*  		s;
		struct cfg_value_ty* r;
	} v;
	int altered;
	struct cfg_value_ty* next;
};

struct cfg_block_ty {
	char *name;
	struct cfg_value_ty* head;
	struct cfg_block_ty* next;
};

struct cfg_ty {
  struct cfg_vtbl_ty *vtbl;
  /* rest of opts */
	char *filename;
	int numentries;
	int opts;	
	struct cfg_block_ty* head;
};
  
typedef struct cfg_vtbl_ty cfg_vtbl_ty;
struct cfg_vtbl_ty {
	size_t size;
	int opts;
	/* virtual function pointers */
	void (* destructor)(cfg_ty *);
	int  (* get)(cfg_ty *, char *, char *, void *, cfg_type);
	int  (* set)(cfg_ty *, char *, char *, void *, cfg_type);
};

struct bvv_ty {
	char* block;
	char* variable;
	char* value;
	int line;
};

void discard_whitespace(FILE* file);
void discard_line(FILE* file);

struct cfg_block_ty* get_block(struct cfg_block_ty* head, char* name);
struct cfg_block_ty* add_block(struct cfg_ty* cfg, char* name);
struct cfg_value_ty* get_variable(struct cfg_value_ty* head, char*vname);
struct cfg_value_ty* add_variable(struct cfg_block_ty* block, char *name);
cfg_type get_type(char* value, void* nvalue);
int insert_bvv(struct cfg_ty* cfg, struct bvv_ty* bvv);

int cfg_priv_set ( cfg_ty *cfg, char *block_name, char *variable_name, void *var, cfg_type type );
int cfg_priv_get ( cfg_ty *cfg, char *block_name, char *variable_name, void *var, cfg_type type );
void cfg_priv_destructor ( cfg_ty *cfg );

cfg_ty *cfg_new (cfg_vtbl_ty *);
#ifdef __cplusplus
}
#endif
#endif  /* _CFG_PRIVATE_H_ */
