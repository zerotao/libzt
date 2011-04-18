/*
 * Filename: zt_macros.h
 * Description: common macro definitions
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *      Copyright (C) 2000-2010, Jason L. Shiffer
 *      See file COPYING for details
 *
 * Notes:
 *
 */

#ifndef _ZT_MACROS_H_
#define _ZT_MACROS_H_

#include <zt.h>

BEGIN_C_DECLS

/*!
 * Name: ABS
 * Description: Get the absolute value of an int
 */
#undef ABS
#define ABS(a)  (((a) < 0) ? -(a) : (a))

/*!
 * Name: CLAMP
 * Description: Force a value to within min and max
 */
#undef CLAMP
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

/*!
 * Name: CEILING
 * Description: clamp an integer to a max value
 */
#undef CEILING
#define CEILING(x, ceil) (((x) > (ceil)) ? (ceil) : (x))

/*!
 * Name: FLOOR
 * Description: clamp an integer to a min value
 */
#undef FLOOR
#define FLOOR(x, floor) (((x) < (floor)) ? (floor) : (x))

/*!
 * Name: MAX
 * Description: return the maximum of a and b
 */
#undef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/*!
 * Name: MIN
 * Description: return the minimum of a and b
 */
#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/*!
 * Name: sizeof_array
 * Description: get the size of an array
 */
#define sizeof_array(a) (sizeof(a) / sizeof(a[0]))

/*!
 * Name: endof_array
 * Description: get the end of an array
 */
#define endof_array(a) ((a) + sizeof_array(a))

/*!
 * Name: zt_offsetof
 * Description: get the offset into a structure by name
 */
#ifndef offsetof
#define zt_offsetof(type, member) \
    ((size_t)(&((type *)0)->member))
#else
#define zt_offsetof(type, member) offsetof(type,member)
#endif    /* offset_of*/

/*!
 * Name: containerof
 * Description: get the head of a structure containing member X
 */
#ifndef containerof
#define containerof(ptr, type, member) \
    ((type *)((char *)(ptr) - zt_offsetof(type, member)))
#endif    /* container_of */

/*!
 * Name: BLANK
 * Description: works with INDENT to inject blank data
 * Example: printf(BLANK "-a, --alpha" NL, INDENT_TO(15, 5, 0))
 */
#define BLANK "%*s"

/*!
 * Name: NL
 * Description: supply the appropriate newline for the platform
 * Example: printf("Usage: %s" NL, argv[0])
 */
#if defined(_WIN32)
# define NL "\r\n"
#else
# define NL "\n"
#endif /* defined(_WIN32) */

/*!
 * Name: INDENT
 * Description: indent to column lvl*5
 * Example: printf(BLANK "-a, --alpha" NL, INDENT(3))
 */
#define INDENT(lvl) INDENT_PAD((lvl), 5, 0)

/*!
 * Name: INDENT_TO
 * Description: indent to column tgt*step starting at start
 * Example: printf(BLANK "-a, --alpha" NL, INDENT_TO(15, 5, 0))
 */
#define INDENT_TO(tgt, step, start)          \
    INDENT_PAD((((tgt) - (start)) / (step)), \
               (step),                       \
               (((tgt) - (start)) % (step)))

/*!
 * Name: INDENT_PAD
 * Description: injects padding into a print statement
 * Exmaple: printf(BLANK "-a, --alpha" NL, INDENT_PATH(3, 5, 0))
 */
#define INDENT_PAD(lvl, step, pad) ((int)(((lvl) * step) + (pad))), ""

/************ Apply Macros ************/

#define MACRO_APPLY(FUNC, ARGS, NEXT) FUNC ARGS NEXT
#define MACRO_APPLY_STOP(FUNC, ARGS, NEXT) FUNC ARGS

#define _MACRO_EVAL_1A(OP, F1, F2, ARGS) OP(F1, ARGS, _MACRO_EVAL_1B)
#define _MACRO_EVAL_1B(OP, F1, F2, ARGS) OP(F1, ARGS, _MACRO_EVAL_1A)

#define _MACRO_EVAL_2A(OP, F1, F2, ARGS) OP(F2, ARGS, _MACRO_EVAL_2B)
#define _MACRO_EVAL_2B(OP, F1, F2, ARGS) OP(F2, ARGS, _MACRO_EVAL_2A)

#define MACRO_EVAL_1(FORMS) _MACRO_EVAL_1A FORMS
#define MACRO_EVAL_2(FORMS) _MACRO_EVAL_2A FORMS

/************ Bit macros ************/
/*!
 * Name: ZT_BIT_SET
 * Description: set bit on target
 */
#define ZT_BIT_SET(target, bit) \
    ((target) |= (bit))

/*!
 * Name: ZT_BIT_UNSET
 * Description: unset bit on target
 */
#define ZT_BIT_UNSET(target, bit) \
    ((target) &= ~(bit))

/*!
 * Name: ZT_BIT_ISSET
 * Description: TRUE if bit is set on target
 */
#define ZT_BIT_ISSET(target, bit) \
    (((target) & (bit)) ? TRUE : FALSE)

/*!
 * Name: ZT_BIT_ISUNSET
 * Description: TRUE if bit is not set on target
 */
#define ZT_BIT_ISUNSET(target, bit) \
    (ZT_BIT_ISSET(target, bit) ? FALSE : TRUE)

END_C_DECLS
#endif /*_ZT_MACROS_H_*/
