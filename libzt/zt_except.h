/*
 * exception hander for c
 *
 * Copyright (C) 2000-2005 Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id:
 */

#ifndef _ZT_EXCEPT_H_
#define _ZT_EXCEPT_H_

#include <setjmp.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif


/* exported types */
struct except_Frame {
	struct except_Frame	 *prev;
	int 		  	  phase;
	int 		  	  caught;
	jmp_buf			  env;
	void			 *exception;
	void			 *type;
	char			 *etext;
	char			 *efunc;
	char			 *efile;
	int			  eline;
};

typedef int (*except_handler)(void *, void *, char *, char *, char *, int);

enum { except_WindPhase = 0, except_UnWindPhase };

/* exported variables */
extern struct except_Frame *_except_Stack;

/* exported functions */
extern void _except_install_handler(void*, except_handler h);
extern void _except_remove_handler(void*, except_handler);
extern void _except_call_handlers(struct except_Frame *);
extern void _except_unhandled_exception(char *etext, char *efile, unsigned int eline, char *efunc);


/* exported macros */
#define INSTALL_EXCEPT_HANDLER(EXCEPTION, HANDLER)			\
	_except_install_handler(&EXCEPTION, HANDLER)

#define REMOVE_EXCEPT_HANDLER(EXCEPTION, HANDLER)			\
	_except_remove_handler(&EXCEPTION, HANDLER)

#define RETHROW()							\
		_except_Stack->caught = 0

#define THROW_TYPED(EXCEPTION, TYPE)					\
	do {								\
		if(!_except_Stack) {					\
			_except_unhandled_exception(#EXCEPTION,		\
						    __FILE__,		\
						    __LINE__,		\
						    __FUNCTION__);	\
		}							\
		_except_Stack->etext = #EXCEPTION;			\
		_except_Stack->efile = __FILE__;			\
		_except_Stack->efunc = __FUNCTION__;			\
		_except_Stack->eline = __LINE__;			\
		_except_Stack->caught = 0;				\
		if(_except_Stack->phase == except_WindPhase){		\
			_except_Stack->exception = &(EXCEPTION);	\
			_except_Stack->type = &(TYPE);			\
			longjmp(_except_Stack->env, 1);			\
		}							\
	} while(0)

#define TRY_THROW(EXCEPTION)						\
	TRY(THROW(EXCEPTION))

#define THROW(EXCEPTION)						\
	THROW_TYPED(EXCEPTION, EXCEPTION);

#define CATCH(EXCEPTION, DATA)								\
	if(_except_Stack->phase == except_UnWindPhase){					\
		void *e = _except_Stack->exception;					\
		void *e2 = (void *)(&EXCEPTION);					\
		if(e >= e2 && e2 <= (void *)((int)e + sizeof(EXCEPTION))){ 		\
			_except_Stack->caught = 1;					\
			{ DATA };							\
		}									\
	}

#define _PUSH_FRAME_DATA(x)				\
	(x)->prev->exception = (x)->exception;		\
	(x)->prev->type = (x)->type;			\
	(x)->prev->etext = (x)->etext;			\
	(x)->prev->efile = (x)->efile;			\
	(x)->prev->efunc = (x)->efunc;			\
	(x)->prev->eline = (x)->eline

#define DO_TRY										\
	do{										\
		struct except_Frame Frame;						\
		Frame.prev = _except_Stack;						\
		Frame.phase = except_WindPhase;						\
		_except_Stack = &Frame;							\
		switch(setjmp(_except_Stack->env)){					\
			case 0:
#define ELSE_TRY									\
                        break;								\
	                case 1:								\
			_except_Stack->phase = except_UnWindPhase;

#define END_TRY										\
                        default:							\
			if((_except_Stack->exception) &&				\
			   (_except_Stack->caught == 0)){				\
				if(_except_Stack->prev != NULL){			\
					_PUSH_FRAME_DATA(_except_Stack); 		\
					_except_Stack = _except_Stack->prev; 		\
					_except_Stack->caught = 0;			\
					longjmp(_except_Stack->env, 1);			\
				}else{							\
					_except_call_handlers(_except_Stack); 		\
				}							\
			}								\
		}/*unwind end*/								\
		_except_Stack = _except_Stack->prev;					\
	}while(0);


#define TRY(WIND_DATA, UNWIND_DATA...)						\
	DO_TRY									\
	{ WIND_DATA }								\
	ELSE_TRY								\
	{ UNWIND_DATA }								\
	END_TRY

#define TRY_RETURN								\
	_except_Stack = _except_Stack->prev;					\
	return

#define UNWIND_PROTECT(WIND_DATA, UNWIND_DATA)					\
	 do {									\
		 static void *_unwind_protect;					\
		 TRY(								\
		     {								\
			     { WIND_DATA; }					\
			     _unwind_protect = _except_Stack->exception; 	\
			     THROW (_unwind_protect);				\
		     },								\
		     {								\
			     { UNWIND_DATA; }					\
			     CATCH (_unwind_protect, ; );			\
			     RETHROW();						\
		     });							\
	 }while(0)

#ifdef __cplusplus
#endif

#endif /* _ZT_EXCEPT_H_ */
