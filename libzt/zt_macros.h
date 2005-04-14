/*
 * zt_macros.h        ZeroTao Macros
 *
 * Copyright (C) 2000-2002, 2004, 2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: macros.h,v 1.2 2003/06/09 13:42:13 jshiffer Exp $
 */

/*
 * Description: 
 */

#ifndef _ZT_MACROS_H_
#define _ZT_MACROS_H_

#ifdef __cplusplus
extern "C" {
#pragma }
#endif

/* Get the absolute value of a int */
#undef ABS
#define ABS(a)  (((a) < 0) ? -(a) : (a))

/* Force a value to within a min and max */
#undef CLAMP
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#undef CEILING
#define CEILING(x, ceil) (((x) > (ceil)) ? (ceil) : (x))

#undef FLOOR
#define FLOOR(x, floor) (((x) < (floor)) ? (floor) : (x))

/* Max of a and b */
#undef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Minimum of a and b */
#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/* Get the size of an array */
#define sizeof_array(a) (sizeof(a) / sizeof(a[0]))

/* Get the end of an array */
#define endof_array(a) ((a) + sizeof_array(a))

/* get the offset into a structure */
#ifndef offsetof
#define offsetof(type, member)  		\
	((size_t)(&((type *)0)->member))
#endif	/* offset_of*/

/* get the head of the structure this member is part of */
#ifndef containerof
#define containerof(ptr, type, member)		\
	((type *)((char *)(ptr) - offsetof(type, member)))
#endif	/* container_of */

#define BLANK "%*s"

#define INDENT(lvl) INDENT_PAD((lvl), 5, 0)
#define INDENT_TO(tgt, step, start) 			\
	INDENT_PAD((((tgt) - (start)) / (step)),	\
		   (step),				\
		   (((tgt) - (start)) % (step)))

#define INDENT_PAD(lvl,step,pad) (((lvl) * step) + (pad)), ""

#ifdef __cplusplus
}
#endif

#endif /*_ZT_MACROS_H_*/
