/*
 * zt_except.c        excption handler for C
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id:
 */

#include <stdio.h>

#include "zt_except.h"

struct except_Handler {
	struct except_Handler	 *next;
	except_handler		  h;
};

struct except_Handlers {
	void			 *e;
	struct except_Handler	 *handlers;
	struct except_Handlers	 *next;
};
  
struct except_Frame *_except_Stack = NULL;
struct except_Handlers _except_Handlers_Stack = { NULL, NULL, NULL };

void _except_install_handler(void *e, except_handler h){
	struct except_Handlers *stack = &_except_Handlers_Stack;
	struct except_Handler *tmph = NULL;
	/* make space for a new handler */
	struct except_Handler *newh = calloc(1, sizeof(struct except_Handler));
	if(!newh){
		fprintf(stderr, "Could not calloc space for new handler\n");
		exit(255);
	}
	newh->next = NULL;
	newh->h = h;
  
	if((stack->e == NULL)){
		stack->e = e;
		stack->handlers = newh;
		return;
	}

	while((stack->next != NULL) && (stack->e != e)){
		stack = stack->next;
	}
  
	if((stack->next == NULL) && (stack->e != e)){
		stack->next = calloc(1, sizeof(struct except_Handlers));
		if(!stack->next){
			fprintf(stderr, "Could not calloc space for new handler\n");
			exit(255);
		}
		stack->next->e = e;
		stack->next->handlers = newh;
		return;
	}
	//  stack = stack->next;
	if(stack->handlers == NULL){
		stack->handlers = newh;
		return;
	}
	tmph = stack->handlers;
	while(tmph->next != NULL){
		tmph = tmph->next;
	}
	tmph->next = newh;
	return;
}

void _except_remove_handler(void *e, except_handler h){
	struct except_Handlers *stack = &_except_Handlers_Stack;
	while((stack->e != e) && stack->next != NULL)
	{
		stack = stack->next;
	}  
	if(stack->e == e){
		struct except_Handler *handler = stack->handlers;
		struct except_Handler *prev = NULL;

		while((handler->next != NULL) && (handler->h != h)){
			prev = handler;
			handler = handler->next;
		}
		if(prev == NULL){	/* First one */
			stack->handlers = handler->next;
			free(handler);
		}else if(handler->h == h){
			prev->next = handler->next;
			free(handler);
		}else{
			fprintf(stderr, "Attempt to remove a non installed exception Handler.\n");
		} 
		if(stack->handlers == NULL){
			stack->e = NULL;
		}
	}else{
		fprintf(stderr, "Attempt to remove a non installed exception Handler.\n");
	}
}

void _except_call_handlers(struct except_Frame *estack)
{
	struct except_Handlers *hstack = &_except_Handlers_Stack;
	struct except_Handler *handler;
	
	while((hstack->e != estack->exception) && hstack->next != NULL)
	{
		hstack = hstack->next;
	}  
	if(hstack->e == estack->exception){
		int ret;
		int last = 0;
		handler = hstack->handlers;
		while((handler != NULL) && (!last)){
			ret = handler->h(estack->exception,
					 estack->type,
					 estack->etext,
					 estack->efile,
					 estack->efunc,
					 estack->eline);
			switch(ret){
				case 1:
					handler = handler->next;
					break;
				case 0:
					last = 1;
					continue;
					break;
				default:
					fprintf( stderr,
						 "Exception handler for '%s' has "
						 "requested an exit for exception "
						 "'%s' @ %s[%d]:%s\n",
						 estack->etext,
						 estack->etext,
						 estack->efile,
						 estack->eline,
						 estack->efunc);
					exit(ret);
					break;
			}
		}
	}else{
		fprintf(stderr,
			"Uncaught/Unhandled Exception: '%s' @ %s[%d]:%s\n",
			estack->etext,
			estack->efile,
			estack->eline,
			estack->efunc);
		abort();
	}
}

