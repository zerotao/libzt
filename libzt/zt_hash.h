/*
 * zt_hash.h        ZeroTao New Hash
 *
 * Copyright (C) 2002-2004, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * Origional code developed by Bob Jenkins, December 1996, Public Domain
 *
 * $Id$
 */

/*
 * Description:
 * Described in the paper @ http://burtleburtle.net/bob/hash/evahash.html
 *
 * I include only the "portable" version that allows for machines of
 * any endian type.
 */

#ifndef _ZT_HASH_H_
#define _ZT_HASH_H_

#include <libzt/zt.h>

#define zt_hashsize(n) ((unsigned long)1<<(n))
#define zt_hashmask(n) (zt_hashsize(n)-1)

unsigned long
zt_hash(unsigned char *k, unsigned long length, unsigned long initval);


#define zt_hashsize64(n) ((unsigned long long)1<<(n))
#define zt_hashmask64(n) (zt_hashsize64(n)-1)

unsigned long long
zt_hash64(unsigned char *k,
	  unsigned long long length,
	  unsigned long long initval);

#endif /* _ZT_HASH_H_ */


