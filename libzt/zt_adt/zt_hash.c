#include "../zt_hash.h"

u_int32_t
zt_hash32_buff(void *buf, size_t len, u_int32_t hval)
{
    unsigned char *bp = (unsigned char *)buf;
    unsigned char *be = bp + len;

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

    while (*s) {

	/* xor the bottom with the current octet */
	hval ^= (u_int32_t)*s++;

	/* multiply by the 32 bit FNV magic prime mod 2^32 */
	hval *= ZT_HASH32_PRIME;
    }

    return hval;
}


u_int64_t
zt_hash64_buff(void *buf, size_t len, u_int64_t hval)
{
    unsigned char *bp = (unsigned char *)buf;
    unsigned char *be = bp + len;

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

    while (*s) {

	/* xor the bottom with the current octet */
	hval ^= (u_int64_t)*s++;

	/* multiply by the 64 bit FNV magic prime mod 2^64 */
	hval *= ZT_HASH64_PRIME;
    }

    return hval;
}
