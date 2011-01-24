/*!
 * Filename: zt_hash.h
 * Description: this is an implementation of the FNV hashing algorithm as described at
 *   http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *   as this is a measly import of the work done at isthe.com this file and
 *   the assoctiated zt_hash.c are in the public domain per their request.
 *
 * Notes:
 *
 */

#ifndef _ZT_HASH_H_
#define _ZT_HASH_H_

#ifdef __cplusplus
    extern "C" {
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L

#define ZT_HASH64_INIT ((uint64_t)0xcbf29ce484222325ULL)
#define ZT_HASH64_PRIME ((uint64_t)0x100000001b3ULL)

uint64_t zt_hash64_buff(void *buf, size_t len, uint64_t init);
uint64_t zt_hash64_cstr(const uint8_t *buf, uint64_t init);

#endif /* __STDC_VERSION__ */

#define ZT_HASH32_INIT ((uint32_t)0x811c9dc5)
#define ZT_HASH32_PRIME ((uint32_t)0x01000193U)


uint32_t zt_hash32_buff(void *buf, size_t len, uint32_t init);
uint32_t zt_hash32_cstr(const uint8_t *buf, uint32_t init);

#define ZT_HASH_MASK(x) (((uint32_t)1 << (x)) - 1)

#define ZT_HASH24_MASK(hash) \
    (hash = (hash >> 24) ^ (hash & ZT_HASH_MASK(24)))
#define ZT_HASH16_MASK(hash) \
    (hash = (hash >> 16) ^ (hash & ZT_HASH_MASK(16)))

/* intentionally different */
#define ZT_HASH8_MASK(hash) \
    (hash = ((hash >> 8) ^ hash) & ZT_HASH_MASK(8))

#define ZT_HASH4_MASK(hash) \
    (hash = ((hash >> 4) ^ hash) & ZT_HASH_MASK(4))

/* *INDENT-OFF* */
#define ZT_HASH_SUB32_MASK(hash, x)                                   \
    if ((x) < 32) {                                                   \
        if ((x) < 16) {                                               \
            (hash = ((hash >> (x)) ^ hash) & ZT_HASH_MASK(x));        \
        } else {                                                      \
            (hash = ((hash >> (x)) ^ (hash & ZT_HASH_MASK(x))));      \
        }                                                             \
    }
/* *INDENT-ON* */

#ifdef __cplusplus
}
#endif
#endif    /* _ZT_HASH_H_ */
