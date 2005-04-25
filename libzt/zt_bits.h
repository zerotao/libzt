/****h* libZT/Bits
 * DESCRIPTION
 *   Bit Operations
 *
 * COPYRIGHT
 *   Copyright (C) 2001-2005, Jason L. Shiffer <jshiffer@zerotao.org>.
 *   All Rights Reserved.
 *   See file COPYING for details.
 *
 ****/

#ifndef _ZT_BITS_H_
#define _ZT_BITS_H_

BEGIN_C_DECLS

extern void set_bit(int nr, volatile void * addr);
extern void clear_bit(int nr, volatile void * addr);
extern void change_bit(int nr, volatile void * addr);
extern int test_and_set_bit(int nr, volatile void * addr);
extern int test_and_clear_bit(int nr, volatile void * addr);
extern int test_and_change_bit(int nr, volatile void * addr);
extern int find_first_zero_bit(void * addr, unsigned size);
extern int find_next_zero_bit(void * addr, int size, int offset);
extern unsigned long ffz(unsigned long word);
extern int ffs(int x);

END_C_DECLS
#endif /* _ZT_BITS_H_ */
