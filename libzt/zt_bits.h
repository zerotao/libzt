/* zt_bits.h        Bit Operations
 * 
 * Copyright (C) 2001-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * 
 * 
 * $Id: bits.h,v 1.2 2003/06/09 13:42:13 jshiffer Exp $
 */

#ifndef _ZT_BITS_H_
#define _ZT_BITS_H_

#ifdef __cplusplus
extern "C" {
#pragma }
#endif

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

#ifdef __cplusplus
}
#endif
#endif /* _ZT_BITS_H_ */
