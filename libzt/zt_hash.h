/*
 * zt_hash.h  hashing functions
 */

/*
 * this is an implementation of the FNV hashing algorithm as described at
 * http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *
 * as this is a measly import of the work done there this file and
 * the assoctiated zt_hash.c are in the public domain per their request.
 */

#ifndef _ZT_HASH_H_
#define _ZT_HASH_H_

#include <libzt/zt.h>

#define ZT_HASH64_INIT ((u_int64_t)0xcbf29ce484222325ULL)
#define ZT_HASH32_INIT ((u_int32_t)0x811c9dc5)

#define ZT_HASH32_PRIME ((u_int32_t)0x01000193)
#define ZT_HASH64_PRIME ((u_int64_t)0x01000193)

u_int32_t zt_hash32_buff(void *buf, size_t len, u_int32_t init);
u_int32_t zt_hash32_cstr(const u_int8_t *buf, u_int32_t init);

#define ZT_HASH_MASK(x) (((u_int32_t)1<<(x))-1)

#define ZT_HASH24_MASK(hash) \
	(hash = (hash>>24) ^ (hash & ZT_HASH_MASK(24)))
#define ZT_HASH16_MASK(hash) \
	(hash = (hash>>16) ^ (hash & ZT_HASH_MASK(16)))

/* intentionally different */
#define ZT_HASH8_MASK(hash) \
	(hash = ((hash>>8) ^ hash) & ZT_HASH_MASK(8))
#define ZT_HASH4_MASK(hash) \
	(hash = ((hash>>4) ^ hash) & ZT_HASH_MASK(4))

#define ZT_HASH_SUB32_MASK(hash,x) \
	if ((x) < 32) {								\
		if ((x) < 16) {							\
			(hash = ((hash>>(x)) ^ hash) & ZT_HASH_MASK(x)); 	\
		} else {							\
			(hash = ((hash>>(x)) ^ (hash & ZT_HASH_MASK(x)))); 	\
		}								\
	}


u_int64_t zt_hash64_buff(void *buf, size_t len, u_int64_t init);
u_int64_t zt_hash64_cstr(const u_int8_t *buf, u_int64_t init);

#endif	/* _ZT_HASH_H_ */


