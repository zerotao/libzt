/*
 * Interface to ini style config files
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: ini.c,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

#include "cfg_private.h"

#include <stdio.h>
#include <string.h>

#include "../zt.h"
#include "../zt_log.h"
#include "../zt_assert.h"

#define BUFMAX 1024

static char* valid_variable_name = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

static int parse_cfg(cfg_ty* cfg);
static int parse_file(cfg_ty* cfg, FILE* file);
static int parse_line(FILE* file, struct bvv_ty* bvv);
static int parse_block(FILE* file, struct bvv_ty* bvv);


/* component data */
static cfg_vtbl_ty vtbl = {
  sizeof(cfg_ty),
  0,
  cfg_priv_destructor,
  cfg_priv_get,
  cfg_priv_set
};

cfg_ty *
cfg_ini( file, opts )
     char *file;
	 int opts;
{
	cfg_ty *cfg = cfg_new(&vtbl);
	cfg->filename = strdup(file);
	cfg->opts = opts;
	if(parse_cfg(cfg) < 0){
		free(cfg->filename);
		free(cfg);
		cfg = NULL;
	}
	
	return cfg;
}

int parse_cfg(cfg_ty* cfg)
{
	int ret = 0;
	FILE* file;

	if((file = fopen(cfg->filename, "r")) == NULL)
	{
		return -1;
	}	
	ret = parse_file(cfg, file);
	fclose(file);
	return(ret);
}


static int parse_block(FILE* file, struct bvv_ty* bvv)
{
	char buff[BUFMAX];
	int c;
	int i = 0;
	memset(buff, '\0', BUFMAX);

	assert(file);
	assert(bvv);
	
	while(((c = fgetc(file)) != EOF) && (i < BUFMAX)){
		if(c == ']') break;
		buff[i] = (char)c;
		i++;
	}

	if(i >= BUFMAX){
//		errno = EOVERFLOW;
		return -1;
	}
	
	/* Otherwize we have a block name in buff */
	bvv->block = strdup(buff);
	if(bvv->block == NULL){
		return -1;
	}
	return(0);	
}

static int parse_line(FILE* file, struct bvv_ty* bvv)
{
	char buff[BUFMAX];
	int end;
	char* var;
	char* val;
	char* tok;
	int len;
	int vallen;
	int i;
	int c;	

	memset(buff, '\0', BUFMAX);
	if(fgets(buff, BUFMAX, file) == NULL){
		return (0);
	}
	val = strstr(buff, "=");
	if(!val){
//		errno = EPROTO;
		return (0);
	}
	len = strlen(buff) -1;
	if(buff[len] == '\n'){
		buff[len] = '\0'; /* get rid of that pesky \n */
	} else {		/* No \n but maybe we are at the EOF */
		c = fgetc(file);
		ungetc(c, file);	
		if(c != EOF){
//			errno = EPROTO;	/* Nope no EOF so some sort of protocol error */
			return 0;
		}
	}
	
	val++;			/* val points to the beginning of value + any white space*/
	var = buff;		/* var points to the beginning of variable */
	end = strspn(buff, valid_variable_name);
	tok = buff + end;
	*tok = '\0';		/* chop any invalid characters from variable*/
	end = strspn(val, " \t");
	val = val + end;	/* Now val points to the beginning of value - any leading white space */
	vallen = strlen(val);
	
	for(i=0; i < vallen; i++){
		if((val[i] == ';') || (val[i] == '#')){
			val[i] = '\0';
			break;
		}
	}
	vallen = strlen(val);
	for(i=vallen-1; i > 0; i--){
		if((val[i] == ' ') || (val[i] == '\t')){
			val[i] = '\0';
		} else {
			break;
		}
	}
	
	if(val >= buff+len){
//		errno = EPROTO;
		return 0;
	}
	bvv->variable = strdup(var);
	bvv->value = strdup(val);	
	return 1;
}

static int parse_file(cfg_ty* cfg, FILE* file)
{
	int c;
	char block[BUFMAX];
	struct bvv_ty bvv;
	int numents = 0;
	int line = 1;

	assert(cfg);
	memset(&bvv, '\0', sizeof(struct bvv_ty));
	sprintf(block, "Global");									/* default blockname */
	bvv.block = strdup(block);
	
	while((c = fgetc(file)) != EOF){
		switch(c){
		case '\n':
			break;
		case '[':												/* begin a block */
			memset(block, '\0', BUFMAX);
			if(bvv.block){
				free(bvv.block);
				bvv.block = NULL;
			}
			parse_block(file, &bvv);
			discard_line(file);
			break;
		case ';':
			/* FALL-THRU */
		case '#':												/* comment */
			while(((c = fgetc(file)) != EOF) && (c != '\n'));
			break;
		case ' ':
			/* FALL-TRHU */
		case '\t':												/* whitespace */
			discard_whitespace(file);
			break;
		default:
			ungetc(c, file);
			if(!(parse_line(file, &bvv))){
				log_printf(log_err, "Syntax error in config file at line: %d\n", line);
				return -1;
			}
			insert_bvv(cfg, &bvv);
			free(bvv.variable);
			free(bvv.value);
			numents++;
			break;
		}
		line++;
	}
	free(bvv.block);
	cfg->numentries = numents;
	return(0);
}
