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

#include "../zt_internal.h"
#include "../zt_log.h"
#include "cfg_private.h"


struct zt_cfg_block_ty* get_block(struct zt_cfg_block_ty* head, char* name);
struct zt_cfg_block_ty* add_block(struct zt_cfg_ty* cfg, char* name);
struct zt_cfg_value_ty* get_variable(struct zt_cfg_value_ty* head, char*vname);
struct zt_cfg_value_ty* add_variable(struct zt_cfg_block_ty* block, char *name);
zt_cfg_type get_type(char* value, void* nvalue);

zt_cfg_ty*
zt_cfg_new(zt_cfg_vtbl_ty *vptr)
{
    zt_cfg_ty * result;

    result = zt_callocs(vptr->size, 1);
    result->vtbl = vptr;
    return result;
}

void cfg_discard_line(FILE* file)
{
    int c = 0;

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            break;
        }
    }
}

void cfg_discard_whitespace(FILE* file)
{
    int c = 0;

    while ((c = fgetc(file)) != EOF) {
        if ((c != ' ') && (c != '\t')) {
            ungetc(c, file);
            break;
        }
    }
}

struct zt_cfg_block_ty*
get_block(struct zt_cfg_block_ty* head, char* name)
{
    while (head) {
        if (!strcmp(name, head->name)) {
            break;
        }
        head = head->next;
    }
    return head;
}

struct zt_cfg_block_ty*
add_block(struct zt_cfg_ty* cfg, char* name)
{
    struct zt_cfg_block_ty* head = cfg->head;

    struct zt_cfg_block_ty* new = head;

    if (head) {
        new = get_block(head, name);
    }
    if (!new) {
        new = zt_calloc(struct zt_cfg_block_ty, 1);
        new->name = strdup(name);
        new->next = cfg->head;
        cfg->head = new;
    }
    return new;
}

struct zt_cfg_value_ty*
get_variable(struct zt_cfg_value_ty* head, char*vname)
{
    while (head) {
        if (!strcmp(head->name, vname)) {
            break;
        }
        head = head->next;
    }
    return head;
}

struct zt_cfg_value_ty*
add_variable(struct zt_cfg_block_ty* block, char *name)
{
    struct zt_cfg_value_ty* head = block->head;

    struct zt_cfg_value_ty* new = head;

    if (head) {
        new = get_variable(head, name);
    }
    if (!new) {
        new = zt_calloc(struct zt_cfg_value_ty, 1);
        new->name = strdup(name);
        new->next = block->head;
        block->head = new;
    }
    return new;
}

zt_cfg_type
get_type(char* value, void* nvalue)
{
    char * endptr = NULL;
    char * pvalue = value;

    /* check for int/long */
    *(long *)nvalue = strtol(pvalue, &endptr, 0);
    if (((*pvalue != '\0') && (*endptr == '\0'))) {
        return zt_cfg_int;
    }

    pvalue = value;
    *(double *)nvalue = strtod(pvalue, &endptr);
    if (((*pvalue != '\0') && (*endptr == '\0'))) {
        return zt_cfg_float;
    }
    pvalue = value;
    if ((!strcasecmp(pvalue, "yes")) ||
        (!strcasecmp(pvalue, "true"))) {
        *(int*)nvalue = 1;
        return zt_cfg_bool;
    } else if ((!strcasecmp(pvalue, "no")) ||
               (!strcasecmp(pvalue, "false"))) {
        *(int*)nvalue = 0;
        return zt_cfg_bool;
    }
    if (value[0] == '\'' || value[0] == '\"') {
        size_t    len = strlen(&value[1]);

        if (value[len] != '\'' || value[len] != '\"') {
            /* error  */
        }
        value[len] = '\0';
        *(char **)nvalue = strdup(&value[1]);
    } else {
        *(char **)nvalue = strdup(value);
    }

    return zt_cfg_string;
}

int
cfg_insert_bvv(struct zt_cfg_ty* cfg, struct cfg_bvv_ty* bvv)
{
    struct zt_cfg_block_ty * block_head = NULL;
    struct zt_cfg_value_ty * value = NULL;
    char                   * variable = NULL;

    block_head = add_block(cfg, bvv->block);
    value = add_variable(block_head, bvv->variable);

    if (value->v.s != NULL) {
        value->altered = 1;
    }

    value->type = get_type(bvv->value, &value->v);

    /* Check for refrences */
    if (value->type == zt_cfg_string) {
        variable = strchr(value->v.s, '.');
        if (((variable) && (*variable == '.'))) {
            struct zt_cfg_value_ty* v = NULL;
            struct zt_cfg_block_ty* b = NULL;
            char                   *block = zt_calloc(char, strlen(value->v.s) + 1);
            strncpy(block, value->v.s, (variable - value->v.s));
            variable++;    /* move past the '.' */
            if ((b = get_block(cfg->head, block))) {
                if ((v = get_variable(b->head, variable))) {
                    free(value->v.s);    /* we had to be a string to get here */
                    value->v.r = v;
                    value->type = zt_cfg_ref;
                }
            }
            free(block);
        }
    }
    return 0;
}

#define BUFMAX 1024
int
zt_cfg_priv_set(zt_cfg_ty *cfg, char *block_name,
                char *variable_name, void *var, zt_cfg_type type)
{
    struct cfg_bvv_ty bvv;

    bvv.block = strdup(block_name);
    bvv.variable = strdup(variable_name);
    bvv.value = zt_calloc(char, BUFMAX);

    switch (type) {
        case zt_cfg_int:
            snprintf(bvv.value, BUFMAX, "%ld", *(long *)var);
            break;
        case zt_cfg_float:
            snprintf(bvv.value, BUFMAX, "%f", *(double *)var);
            break;
        case zt_cfg_bool:
            if ((*(int *)var == 1) || (*(int *)var == 0)) {
                zt_log_printf(zt_log_err, "Invalid value for type zt_cfg_bool in zt_cfg_set!  Must be a string of (true|yes|no|false).");
                return -1;
            }
        /* FALLTHRU */
        case zt_cfg_string:
            snprintf(bvv.value, BUFMAX, "%s", *(char **)var);
            break;
        default:
            zt_log_printf(zt_log_err, "Unknown type in cfg variable list");
            return -1;
    }

    cfg_insert_bvv(cfg, &bvv);

    free(bvv.block);
    free(bvv.variable);
    free(bvv.value);
    return 1;
}

int
zt_cfg_priv_get(zt_cfg_ty *cfg, char *block_name,
                char *variable_name, void *var, zt_cfg_type type)
{
    struct zt_cfg_block_ty* block = NULL;
    struct zt_cfg_value_ty* value = NULL;

    block = get_block(cfg->head, block_name);
    if (!block) {
        return -1;
    }

    value = get_variable(block->head, variable_name);

    if (!value) {
        return -1;
    }


    while (value->type == zt_cfg_ref) {
        value = value->v.r;
    }

    if (value->type != type) {
        return -1;
    }

    switch (value->type) {
        case zt_cfg_bool:
            *(int *)var = value->v.b;
            break;
        case zt_cfg_int:
            *(long *)var = value->v.i;
            break;
        case zt_cfg_float:
            *(double *)var = value->v.f;
            break;
        case zt_cfg_string:
            *(char **)var = value->v.s;
            break;
        default:
            zt_log_printf(zt_log_err, "Unknown type in cfg variable list");
            return -1;
    }

    return value->type;
} /* zt_cfg_priv_get */

void
zt_cfg_priv_destructor( zt_cfg_ty *cfg)
{
    struct zt_cfg_block_ty * block = cfg->head;
    struct zt_cfg_block_ty * blk_next = block;
    struct zt_cfg_value_ty * value = block->head;
    struct zt_cfg_value_ty * val_next = block->head;


    while (block) {
        blk_next = block->next;
        value = block->head;
        while (value) {
            val_next = value->next;
            if (value->type == zt_cfg_string) {
                zt_free(value->v.s); value->v.s = NULL;
            }
            if (value->name) {
                zt_free(value->name); value->name = NULL;
            }
            zt_free(value); value = NULL;
            value = val_next;
        }
        if (block->name) {
            zt_free(block->name);  block->name = NULL;
        }
        zt_free(block);
        block = blk_next;
    }
    if (cfg->filename != NULL) {
        zt_free(cfg->filename);
    }

    zt_free(cfg);
    return;
}
