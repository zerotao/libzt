/****h* libZT/CommonMacros
 * DESCRIPTION
 *
 * COPYRIGHT
 *   Copyright (C) 2000-2002, 2004, 2005, 2006, Jason L. Shiffer <jshiffer@zerotao.com>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/

#ifndef _ZT_MACROS_H_
#define _ZT_MACROS_H_

#include <libzt/zt.h>

BEGIN_C_DECLS

/****d* CommonMacros/ABS
 * NAME
 *   ABS - Get the absolute value of a int
 *****/
#undef ABS
#define ABS(a)  (((a) < 0) ? -(a) : (a))

#undef CLAMP
/****d* CommonMacros/CLAMP
 * NAME
 *   CLAMP - Force a value to within a min and max
 *****/
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#undef CEILING
/****d* CommonMacros/CEILING
 * NAME
 *   CEILING
 *****/
#define CEILING(x, ceil) (((x) > (ceil)) ? (ceil) : (x))

#undef FLOOR
/****d* CommonMacros/FLOOR
 * NAME
 *   FLOOR
 *****/
#define FLOOR(x, floor) (((x) < (floor)) ? (floor) : (x))

#undef MAX
/****d* CommonMacros/MAX
 * NAME
 *   MAX - Max of a and b
 *****/
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#undef MIN
/****d* CommonMacros/MIN
 * NAME
 *   MIN - Minimum of a and b
 *****/
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/****d* CommonMacros/sizeof_array
 * NAME
 *   sizeof_array - Get the size of an array 
 *****/
#define sizeof_array(a) (sizeof(a) / sizeof(a[0]))

/****d* CommonMacros/endof_array
 * NAME
 *   endof_array - Get the end of an array
 *****/
#define endof_array(a) ((a) + sizeof_array(a))

#ifndef offsetof
/****d* CommonMacros/offsetof
 * NAME
 *   offsetof - get the offset into a structure
 *****/
#define offsetof(type, member)  		\
	((size_t)(&((type *)0)->member))
#endif	/* offset_of*/

#ifndef containerof
/****d* CommonMacros/containerof
 * NAME
 *   containerof - get the head of the structure this member is part of
 *****/
#define containerof(ptr, type, member)		\
	((type *)((char *)(ptr) - offsetof(type, member)))
#endif	/* container_of */

/****d* CommonMacros/BLANK
 * NAME
 *   BLANK
 *****/
#define BLANK "%*s"

#define NL "\n"

/****d* CommonMacros/INDENT
 * NAME
 *   INDENT
 *****/
#define INDENT(lvl) INDENT_PAD((lvl), 5, 0)

/****d* CommonMacros/INDENT_TO
 * NAME
 *   INDENT_TO
 *****/
#define INDENT_TO(tgt, step, start) 			\
	INDENT_PAD((((tgt) - (start)) / (step)),	\
		   (step),				\
		   (((tgt) - (start)) % (step)))

/****d* CommonMacros/INDENT_PAD
 * NAME
 *   INDENT_PAD
 *****/
#define INDENT_PAD(lvl,step,pad) ((int)(((lvl) * step) + (pad))), ""

/******
 * Apply macros
 *****/
#define MACRO_APPLY(FUNC, ARGS, NEXT) FUNC ARGS NEXT
#define MACRO_APPLY_STOP(FUNC, ARGS, NEXT) FUNC ARGS

#define _MACRO_EVAL_1A(OP, F1, F2, ARGS) OP(F1, ARGS, _MACRO_EVAL_1B)
#define _MACRO_EVAL_1B(OP, F1, F2, ARGS) OP(F1, ARGS, _MACRO_EVAL_1A)

#define _MACRO_EVAL_2A(OP, F1, F2, ARGS) OP(F2, ARGS, _MACRO_EVAL_2B)
#define _MACRO_EVAL_2B(OP, F1, F2, ARGS) OP(F2, ARGS, _MACRO_EVAL_2A)

#define MACRO_EVAL_1(FORMS) _MACRO_EVAL_1A FORMS
#define MACRO_EVAL_2(FORMS) _MACRO_EVAL_2A FORMS

/* Bit macros */
#define BIT_SET(target, bit) \
	((target) |= (bit))

#define BIT_UNSET(target, bit) \
	((target) &= ~(bit))

#define BIT_ISSET(target, bit) \
	(((target) & (bit)) ? TRUE : FALSE)

#define BIT_ISUNSET(target, bit) \
	(BIT_ISSET(target, bit) ? FALSE : TRUE)

END_C_DECLS
#endif /*_ZT_MACROS_H_*/
