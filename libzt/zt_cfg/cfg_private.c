/* private interface to cfg
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: cfg_private.c,v 1.5 2003/11/26 17:47:29 jshiffer Exp $
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "../zt.h"
#include "../zt_log.h"
#include "cfg_private.h"

cfg_ty*
cfg_new ( vptr )
  cfg_vtbl_ty *vptr;
{
  cfg_ty *result;
  result = calloc(1, vptr->size);
  result->vtbl = vptr;
  return(result);
}

void discard_line(FILE* file)
{
	int c = 0;
	while((c = fgetc(file)) !=  EOF){
		if(c == '\n') break;
	}
}

void discard_whitespace(FILE* file)
{
	int c = 0;
	while((c = fgetc(file)) != EOF){
		if((c != ' ') && (c != '\t')){
			ungetc(c, file);
			break;
		}
	}
}

struct cfg_block_ty*
get_block(struct cfg_block_ty* head, char* name)
{
	while(head){
		if(!strcmp(name, head->name)){
			break;
		}
		head = head->next;
	}
	return head;
}

struct cfg_block_ty*
add_block(struct cfg_ty* cfg, char* name)
{
	struct cfg_block_ty* head = cfg->head;	
	struct cfg_block_ty* new = head;
	
	if(head){
		new = get_block(head, name);
	}
	if(!new){
		new = XCALLOC(struct cfg_block_ty, 1);
		new->name = strdup(name);
		new->next = cfg->head;
		cfg->head = new;
	}
	return new;
}

struct cfg_value_ty*
get_variable(struct cfg_value_ty* head, char*vname)
{
	while(head){
		if(!strcmp(head->name, vname)){
			break;
		}
		head = head->next;
	}
	return head;
}

struct cfg_value_ty*
add_variable(struct cfg_block_ty* block, char *name)
{
	struct cfg_value_ty* head = block->head;
	struct cfg_value_ty* new = head;
	
	if(head){
		new = get_variable(head, name);
	}
	if(!new){
		new = XCALLOC(struct cfg_value_ty, 1);
		new->name = strdup(name);
		new->next = block->head;
		block->head = new;
	}
	return new;	
}

cfg_type
get_type(char* value, void* nvalue)
{
	char* endptr = NULL;
	char* pvalue = value;
	
	/* check for int/long */	
	*(long *)nvalue = strtol(pvalue, &endptr, 0);
	if(((*pvalue != '\0') && (*endptr == '\0'))){
		return cfg_int;
	}
	
	pvalue = value;
	*(double *)nvalue = strtod(pvalue, &endptr);
	if(((*pvalue != '\0') && (*endptr == '\0'))){
		return cfg_float;
	}
	pvalue = value;	
	if((!strcasecmp(pvalue, "yes")) ||
	   (!strcasecmp(pvalue, "true")))
	{
		*(short int*)nvalue = 1;
		return cfg_bool;		
	}else if((!strcasecmp(pvalue, "no")) ||
			 (!strcasecmp(pvalue, "false")))
	{
		*(short int*)nvalue = 0;		
		return cfg_bool;
	}

	if(value[0] == '\'' || value[0] == '\"')
	{
		int	  len = strlen(&value[1]);
		if(value[len] != '\'' || value[len] != '\"')
		{
			/* error  */
		}
		value[len] = '\0';
		*(char **)nvalue = strdup(&value[1]);
	} else {
		*(char **)nvalue = strdup(value);
	}
	
	return cfg_string;
}

int insert_bvv(struct cfg_ty* cfg, struct bvv_ty* bvv)
{
	struct cfg_block_ty* block_head = NULL;	
	struct cfg_value_ty* value = NULL;
	char* variable = NULL;

	block_head = add_block(cfg, bvv->block);
	value = add_variable(block_head, bvv->variable);
	
	if(value->v.s != NULL)
		value->altered = 1;

	value->type = get_type(bvv->value, &value->v);

	/* Check for refrences */
	if(value->type == cfg_string){
		variable = index(value->v.s, '.');
		if(((variable) && (*variable == '.'))){
			struct cfg_value_ty* v = NULL;
			struct cfg_block_ty* b = NULL;
			char  *block = XCALLOC(char, strlen(value->v.s) + 1);
			strncpy(block, value->v.s, (variable - value->v.s));
			variable++;											/* move past the '.' */
			if((b = get_block(cfg->head, block))){
				if((v = get_variable(b->head, variable))){
					free(value->v.s);							/* we had to be a string to get here */
					value->v.r = v;
					value->type = cfg_ref;
				}
			}
			free(block);
		}
	}
	return 0;
}

int cfg_priv_set ( cfg, block_name, variable_name, var, type )
     cfg_ty *cfg;
     char *block_name;
     char *variable_name;
     void *var;
     cfg_type type;
{
	struct bvv_ty bvv;
	bvv.block = strdup(block_name);
	bvv.variable = strdup(variable_name);
#define BUFMAX 1024
	bvv.value = XCALLOC(char, BUFMAX);
	
	switch(type){
	case cfg_int:
		snprintf(bvv.value, BUFMAX, "%ld", *(long *)var);
		break;
	case cfg_float:
		snprintf(bvv.value, BUFMAX, "%f", *(double *)var);
		break;
	case cfg_bool:
		if((*(short int *)var == 1) || (*(short int *)var == 0)){
			log_printf(log_err, "Invalid value for type cfg_bool in cfg_set!  Must be a string of (true|yes|no|false).");
			return -1;
		}
		/* FALLTHRU */
	case cfg_string:
		snprintf(bvv.value, BUFMAX, "%s", *(char **)var);
		break;
	default:
		log_printf(log_err, "Unknown type in cfg variable list");
		return -1;
	}

	insert_bvv(cfg, &bvv);	

	free(bvv.block);
	free(bvv.variable);
	free(bvv.value);
	return 1;
}

int cfg_priv_get ( cfg, block_name, variable_name, var, type )
     cfg_ty *cfg;
     char *block_name;
     char *variable_name;
     void *var;
     cfg_type type;
{
	struct cfg_block_ty* block = NULL;
	struct cfg_value_ty* value = NULL;
	
	block = get_block(cfg->head, block_name);
	if(!block)
		return -1;
	
	value = get_variable(block->head, variable_name);
	if(!value)
		return -1;

	switch(value->type){
	case cfg_bool:
		*(short int *)var = value->v.b;
		break;
	case cfg_int:
		*(long *)var = value->v.i;
		break;
	case cfg_float:
		*(double *)var = value->v.f;
		break;
	case cfg_string:
		*(char **)var = value->v.s;
		break;
	case cfg_ref:
	{
		struct cfg_value_ty* ref = value->v.r;
		while(ref->type == cfg_ref){
			ref = ref->v.r;
		}
		switch(ref->type){
		case cfg_bool:
			*(short int *)var = ref->v.b;
			break;
		case cfg_int:
			*(long *)var = ref->v.i;
			break;
		case cfg_float:
			*(double *)var = ref->v.f;
			break;
		case cfg_string:
			*(char **)var = ref->v.s;
			break;
		default:
			log_printf(log_err, "Unknown type in cfg variable list");
			return -1;
		}
		break;
	}
	default:
		log_printf(log_err, "Unknown type in cfg variable list");
		return -1;
	}
//	if(value->type != type)
//		return 1;
	return value->type;
}

void cfg_priv_destructor ( cfg )
    cfg_ty *cfg;
{
	struct cfg_block_ty* block = cfg->head;	
	struct cfg_block_ty* blk_next = block;
	struct cfg_value_ty* value = block->head;
	struct cfg_value_ty* val_next = block->head;	
	

	while(block){
		blk_next = block->next;
		value = block->head;
		while(value){
			val_next = value->next;
			if(value->type == cfg_string){
				XFREE(value->v.s); value->v.s = NULL; 
			}
			if(value->name){
				XFREE(value->name); value->name = NULL;
			}
			XFREE(value); value = NULL;
			value = val_next;
		}
		if(block->name){
			XFREE(block->name);  block->name = NULL;
		}
		XFREE(block);
		block = blk_next;
	}
	if(cfg->filename != NULL)
		XFREE(cfg->filename);
	
	XFREE(cfg);
    return;
}
