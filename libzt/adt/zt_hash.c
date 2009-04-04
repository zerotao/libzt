/****h* libZT/Hashing
 * DESCRIPTION
 *   this is an implementation of the FNV hashing algorithm as described at
 *   http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *
 * COPYRIGHT
 *   as this is a measly import of the work done at isthe.com this file and
 *   the assoctiated zt_hash.c are in the public domain per their request.
 *
 ****/

#include "zt_hash.h"

/*
 * Other sources for hashing, testing hashes, etc:
 * http://www.burtleburtle.net/bob/hash/hashfaq.html
 */

u_int32_t
zt_hash32_buff(void *buf, size_t len, u_int32_t hval)
{
    unsigned char *bp = (unsigned char *)buf;
    unsigned char *be = bp + len;
	
	if (hval == 0) {
		hval = ZT_HASH32_INIT;
	}
	
    while (bp < be) {
		/* xor the bottom with the current octet */
		hval ^= (u_int32_t)*bp++;
	    
		/* multiply by the 32 bit FNV magic prime mod 2^32 */
		hval *= ZT_HASH32_PRIME;
    }

    return hval;
}

u_int32_t
zt_hash32_cstr(const u_int8_t *str, u_int32_t hval)
{
    unsigned char *s = (unsigned char *)str;	/* unsigned string */

	if (hval == 0) {
		hval = ZT_HASH32_INIT;
	}
	
    while (*s) {
		/* xor the bottom with the current octet */
		hval ^= (u_int32_t)*s++;
		/* multiply by the 32 bit FNV magic prime mod 2^32 */
		hval *= ZT_HASH32_PRIME;
    }

    return hval;
}


#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L

u_int64_t
zt_hash64_buff(void *buf, size_t len, u_int64_t hval)
{
    unsigned char *bp = (unsigned char *)buf;
    unsigned char *be = bp + len;
	
	if (hval == 0) {
		hval = ZT_HASH64_INIT;
	}

    while (bp < be) {
		/* xor the bottom with the current octet */
		hval ^= (u_int64_t)*bp++;
	    
		/* multiply by the 64 bit FNV magic prime mod 2^64 */
		hval *= ZT_HASH64_PRIME;
    }

    return hval;
}

u_int64_t
zt_hash64_cstr(const u_int8_t *str, u_int64_t hval)
{
    unsigned char *s = (unsigned char *)str;	/* unsigned string */

	if (hval == 0) {
		hval = ZT_HASH64_INIT;
	}
	
    while (*s) {

		/* xor the bottom with the current octet */
		hval ^= (u_int64_t)*s++;

		/* multiply by the 64 bit FNV magic prime mod 2^64 */
		hval *= ZT_HASH64_PRIME;
    }

    return hval;
}

#endif  /* __STDC_VERSION__ */
