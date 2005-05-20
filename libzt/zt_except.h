/****h* libZT/Exceptions
 * DESCRIPTION
 *   exception hander for c
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2005 Jason L. Shiffer <jshiffer@zerotao.com>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/

#ifndef _ZT_EXCEPT_H_
#define _ZT_EXCEPT_H_

#include <libzt/zt.h>
#include <setjmp.h>
#include <stdlib.h>

BEGIN_C_DECLS

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
extern void _except_unhandled_exception(char *etext, const char *efile, unsigned int eline, const char *efunc);
extern void _except_call_handlers(struct except_Frame *);

/****d* Exceptions/INSTALL_EXCEPT_HANDLER
 * NAME
 *   INSTALL_EXCEPT_HANDLER
 *
 * SOURCE
 */
extern void _except_install_handler(void*, except_handler h);
#define INSTALL_EXCEPT_HANDLER(EXCEPTION, HANDLER)			\
	_except_install_handler(&EXCEPTION, HANDLER)

/*** INSTALL_EXCEPT_HANDLER ***/


/****d* Exceptions/REMOVE_EXCEPT_HANDLER
 * NAME
 *   REMOVE_EXCEPT_HANDLER
 *
 * SOURCE
 */
extern void _except_remove_handler(void*, except_handler);
#define REMOVE_EXCEPT_HANDLER(EXCEPTION, HANDLER)			\
	_except_remove_handler(&EXCEPTION, HANDLER)

/*** REMOVE_EXCEPT_HANDLER ***/

/****d* Exceptions/RETHROW
 *  NAME
 *    RETHROW
 *
 *  SOURCE
 */
#define RETHROW()							\
		_except_Stack->caught = 0

/************ RETHROW */

/****d* Exceptions/THROW_TYPED
 *  NAME
 *    THROW_TYPED
 *
 *  SOURCE
 */
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

/************ THROW_TYPED */

/****d* Exceptions/TRY_THROW
 *  NAME
 *    TRY_THROW
 *
 *  SOURCE
 */
#define TRY_THROW(EXCEPTION)						\
	TRY(THROW(EXCEPTION))

/************ TRY_THROW */

/****d* Exceptions/THROW
 *  NAME
 *    THROW
 *
 *  SOURCE
 */
#define THROW(EXCEPTION)						\
	THROW_TYPED(EXCEPTION, EXCEPTION);

/************ THROW */

/****d* Exceptions/CATCH
 *  NAME
 *    CATCH
 *
 *  SOURCE
 */
#define CATCH(EXCEPTION, DATA)								\
	if(_except_Stack->phase == except_UnWindPhase){					\
		void *e = _except_Stack->exception;					\
		void *e2 = (void *)(&EXCEPTION);					\
		if(e >= e2 && e2 <= (void *)((int)e + sizeof(EXCEPTION))){ 		\
			_except_Stack->caught = 1;					\
			{ DATA };							\
		}									\
	}

/************ CATCH */

/****d* Exceptions/_PUSH_FRAME_DATA
 *  NAME
 *    _PUSH_FRAME_DATA
 *
 *  SOURCE
 */
#define _PUSH_FRAME_DATA(x)				\
	(x)->prev->exception = (x)->exception;		\
	(x)->prev->type = (x)->type;			\
	(x)->prev->etext = (x)->etext;			\
	(x)->prev->efile = (x)->efile;			\
	(x)->prev->efunc = (x)->efunc;			\
	(x)->prev->eline = (x)->eline
/************ _PUSH_FRAME_DATA */

/****d* Exceptions/DO_TRY
 *  NAME
 *    DO_TRY
 *
 *  SOURCE
 */
#define DO_TRY										\
	do{										\
		struct except_Frame Frame;						\
		Frame.prev = _except_Stack;						\
		Frame.phase = except_WindPhase;						\
		_except_Stack = &Frame;							\
		switch(setjmp(_except_Stack->env)){					\
			case 0:
/************ DO_TRY */

/****d* Exceptions/ELSE_TRY
 *  NAME
 *    ELSE_TRY
 *
 *  SOURCE
 */
#define ELSE_TRY									\
                        break;								\
	                case 1:								\
			_except_Stack->phase = except_UnWindPhase;

/************ ELSE_TRY */

/****d* Exceptions/END_TRY
 *  NAME
 *    END_TRY
 *
 *  SOURCE
 */
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


/************ END_TRY */

/****d* Exceptions/TRY
 *  NAME
 *    TRY
 *
 *  SOURCE
 */
#define TRY(WIND_DATA, UNWIND_DATA...)						\
	DO_TRY									\
	{ WIND_DATA }								\
	ELSE_TRY								\
	{ UNWIND_DATA }								\
	END_TRY

/************ TRY */

/****d* Exceptions/TRY_RETURN
 *  NAME
 *    TRY_RETURN
 *
 *  SOURCE
 */
#define TRY_RETURN								\
	_except_Stack = _except_Stack->prev;					\
	return

/************ TRY_RETURN */

/****d* Exceptions/UNWIND_PROTECT
 *  NAME
 *    UNWIND_PROTECT
 *
 *  SOURCE
 */
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
/*** UNWIND_PROTECT ***/

END_C_DECLS

#endif /* _ZT_EXCEPT_H_ */

