/*!
 * Filename: zt_hash.h
 * Description: this is an implementation of the FNV hashing algorithm as described at
 *      http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *   as this is a measly import of the work done at isthe.com this file and
 *   the assoctiated zt_hash.c are in the public domain per their request.
 *
 * Notes:
 *
 */
#include "zt_internal.h"
#include "zt_hash.h"

/*
 * Other sources for hashing, testing hashes, etc:
 * http://www.burtleburtle.net/bob/hash/hashfaq.html
 */

uint32_t
zt_hash32_buff(void *buf, size_t len, uint32_t hval)
{
    unsigned char *bp = (unsigned char *)buf;
    unsigned char *be = bp + len;

    if (hval == 0) {
        hval = ZT_HASH32_INIT;
    }

    while (bp < be) {
        /* xor the bottom with the current octet */
        hval ^= (uint32_t)*bp++;

        /* multiply by the 32 bit FNV magic prime mod 2^32 */
        hval *= ZT_HASH32_PRIME;
    }

    return hval;
}

uint32_t
zt_hash32_cstr(const uint8_t *str, uint32_t hval)
{
    unsigned char *s = (unsigned char *)str;    /* unsigned string */

    if (hval == 0) {
        hval = ZT_HASH32_INIT;
    }

    while (*s) {
        /* xor the bottom with the current octet */
        hval ^= (uint32_t)*s++;
        /* multiply by the 32 bit FNV magic prime mod 2^32 */
        hval *= ZT_HASH32_PRIME;
    }

    return hval;
}


#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L

uint64_t
zt_hash64_buff(void *buf, size_t len, uint64_t hval)
{
    unsigned char *bp = (unsigned char *)buf;
    unsigned char *be = bp + len;

    if (hval == 0) {
        hval = ZT_HASH64_INIT;
    }

    while (bp < be) {
        /* xor the bottom with the current octet */
        hval ^= (uint64_t)*bp++;

        /* multiply by the 64 bit FNV magic prime mod 2^64 */
        hval *= ZT_HASH64_PRIME;
    }

    return hval;
}

uint64_t
zt_hash64_cstr(const uint8_t *str, uint64_t hval)
{
    unsigned char *s = (unsigned char *)str;    /* unsigned string */

    if (hval == 0) {
        hval = ZT_HASH64_INIT;
    }

    while (*s) {
        /* xor the bottom with the current octet */
        hval ^= (uint64_t)*s++;

        /* multiply by the 64 bit FNV magic prime mod 2^64 */
        hval *= ZT_HASH64_PRIME;
    }

    return hval;
}

#endif  /* __STDC_VERSION__ */
