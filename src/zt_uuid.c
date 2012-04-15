#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#include <stdio.h>

#include <math.h>

#include "zt.h"
#include "zt_internal.h"

zt_uuid_t  NAMESPACE_DNS = { { { 0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_URL = { { { 0x6b, 0xa7, 0xb8, 0x11, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_OID = { { { 0x6b, 0xa7, 0xb8, 0x12, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_X500 = { { { 0x6b, 0xa7, 0xb8, 0x14, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };

static int rand_initialized           = 0;

#define _UUID_BASE62_SEGMENT UUID_BASE62_STR_LEN / 2
static const char base62_alphabet[63] = \
    "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static int base62_encode_int64(uint64_t in, char * out, size_t olen);
static int base62_decode_int64(const char * in, size_t ilen, uint64_t * out);

void       srandomdev(void);
#ifndef HAVE_SRANDOMDEV
#include <fcntl.h>
#include <time.h>
void
srandomdev(void) {
    int      fd;
    uint32_t rand_data;

    fd = open("/dev/urandom", O_RDONLY);

    if (fd < 0 || read(fd, &rand_data,
                       sizeof(uint32_t)) != sizeof(uint32_t)) {
        /* this is a shitty situation.... */
        rand_data = (uint32_t)time(NULL);
    }

    if (fd >= 0) {
        close(fd);
    }

    srandom(rand_data);
}

#endif /* ifndef HAVE_SRANDOMDEV */

int
zt_uuid4(zt_uuid_t * uuid) {
    long v;

    zt_assert(uuid);
    if (rand_initialized == 0) {
        srandomdev();
        rand_initialized = 1;
    }

    v = random(), memcpy(&uuid->data.bytes, &v, 4);
    v = random(), memcpy(&uuid->data.bytes[4], &v, 4);
    v = random(), memcpy(&uuid->data.bytes[8], &v, 4);
    v = random(), memcpy(&uuid->data.bytes[12], &v, 4);

    /* set the version number */
    uuid->data.bytes[UUID_VERSION_OFFT]   = (uuid->data.bytes[UUID_VERSION_OFFT] & 0x0F) | (UUID_VER_PSEUDORANDOM << 4);
    /* clear the clock bits */
    uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] = (uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] & 0x3F) | 0x80;

    return 0;
}

int
zt_uuid5(char * value, size_t vlen, zt_uuid_ns type, zt_uuid_t * uuid) {
    zt_sha1_ctx ctx;
    uint8_t     digest[20];

    zt_sha1_init(&ctx);
    switch (type) {
        case UUID_NS_DNS:
            zt_sha1_update(&ctx, NAMESPACE_DNS.data.bytes, UUID_ALEN);
            break;
        case UUID_NS_URL:
            zt_sha1_update(&ctx, NAMESPACE_URL.data.bytes, UUID_ALEN);
            break;
        case UUID_NS_OID:
            zt_sha1_update(&ctx, NAMESPACE_OID.data.bytes, UUID_ALEN);
            break;
        case UUID_NS_X500:
            zt_sha1_update(&ctx, NAMESPACE_X500.data.bytes, UUID_ALEN);
            break;
        default:
            zt_log_printf(zt_log_err, "unknown namespace %d", type);
            return -1;
    }
    zt_sha1_update(&ctx, (uint8_t*)value, vlen);
    zt_sha1_finalize(&ctx, digest);
    memcpy(uuid->data.bytes, digest, UUID_ALEN);

    /* set the version number */
    uuid->data.bytes[UUID_VERSION_OFFT]   = (uuid->data.bytes[UUID_VERSION_OFFT] & 0x0F) | (UUID_VER_NAMESPACE_SHA1 << 4);
    /* clear the clock bits */
    uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] = (uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] & 0x3F) | 0x80;

    return 0;
}

/*
 *  _flagToSize
 *
 *  Internal function returning the minimun necessary buffersize for each of the valid formats.
 *  Returns zero on unknown format.
 */

size_t
_flagToSize(zt_uuid_flags_t flags) {
    size_t oSz = 0;

    switch (flags) {
        case zt_uuid_std_fmt:
            oSz = UUID_STR_LEN + 1;
            break;
        case zt_uuid_short_fmt:
            oSz = UUID_SHORT_STR_LEN + 1;
            break;
        case zt_uuid_base62_fmt:
            oSz = UUID_BASE62_STR_LEN + 1;
            break;
        default:
            zt_assert(false);
            break;
    }

    return oSz;
}

/*
 *  _zt_uuid_fillstr
 *
 *  Internal function that formats and copies into 'uuids' a represent of the 'uuid' as specified by 'flags'.
 *
 *  Input:
 *      uuid - [Required] A pointer to a valid zt_uuid_t structure.
 *      uuids - [Required] A pointer to a character buffer in which to copy the string.
 *      uuids_size - [Required]  Pointer to a size_t containing the number of bytes allocated
 *          in 'uuids'.
 *      flags - [Required] One of [zt_uuid_base62_fmt, zt_uuid_std_fmt, zt_uuid_short_fmt]
 *
 *  Output:
 *      - Upon successful return, the function will return the number of characters copied into
 *          the param 'uuids', not including the terminating NULL byte.
 *      - A return value < 0 indicates an error.
 *
 */



ssize_t
_zt_uuid_fillstr(zt_uuid_t* uuid, char* uuids, size_t uuids_size, zt_uuid_flags_t flags) {
    int oLen = 0;

    if (flags == zt_uuid_base62_fmt) {
		uint64_t v1  = 0;
        uint64_t v2  = 0;
		int      i   = 0;
		int      ok1 = 0;
		int      ok2 = 0;

        zt_assert(uuids_size <= UUID_BASE62_STR_LEN + 1);

        for (i = 0; i < 8; i++) {
            const int shift = (7 - i) * 8;
            v1 |= ((uint64_t)uuid->data.bytes[i] << shift);
            v2 |= ((uint64_t)uuid->data.bytes[i + 8] << shift);
        }

        ok1 = base62_encode_int64(v1, uuids, _UUID_BASE62_SEGMENT);
        ok2 = base62_encode_int64(v2, uuids + _UUID_BASE62_SEGMENT, _UUID_BASE62_SEGMENT);
        if (ok1 >= 0 && ok2 >= 0) {
            oLen        = UUID_BASE62_STR_LEN;
            uuids[oLen] = 0;
        } else {
            oLen = -1;
        }
    } else {
		char         uuids_hex[UUID_SHORT_STR_LEN];
		const char * fmt = NULL;

        zt_assert(flags == zt_uuid_std_fmt || flags == zt_uuid_short_fmt);

        fmt = (flags == zt_uuid_std_fmt) ? "%8.8s-%4.4s-%4.4s-%4.4s-%12.12s" : "%8.8s%4.4s%4.4s%4.4s%12.12s";

        zt_binary_to_hex(uuid->data.bytes, UUID_ALEN, uuids_hex, UUID_SHORT_STR_LEN);

        oLen = snprintf(uuids, uuids_size, fmt,
                        uuids_hex,
                        &uuids_hex[8],
                        &uuids_hex[12],
                        &uuids_hex[16],
                        &uuids_hex[20]);

        zt_assert(flags != zt_uuid_std_fmt || oLen == UUID_STR_LEN);
        zt_assert(flags != zt_uuid_short_fmt || oLen == UUID_SHORT_STR_LEN);
    }

    return oLen;
} /* _zt_uuid_fillstr */

/*
 *  zt_uuid_tostr
 *
 *  Allocates and returns a string in 'uuids' representing 'uuid' as specified by 'flags'.
 *
 *  Input:
 *      uuid -  [Required] A pointer to a valid zt_uuid_t structure.
 *      uuids - [Required] A pointer to a character buffer in which to copy the string.
 *      flags - [Required] One of [zt_uuid_base62_fmt, zt_uuid_std_fmt, zt_uuid_short_fmt]
 *
 *  Output:
 *
 *      - Upon successful return, '*uuids' will contain a pointer to a string containting a ASCII
 *          representation of 'uuid'.  The function will return the number of characters copied into
 *          '*uuids'.  The caller is responsible for deallocating the string with zt_free.
 *      - A return value < 0 indicates an error.
 *
 */

ssize_t
zt_uuid_tostr(zt_uuid_t * uuid, char ** uuids, zt_uuid_flags_t flags) {
    ssize_t oVal;
    size_t  sz = 0;

    if (!uuid || !uuids) {
        return -1;
    }

    *uuids = NULL;

    sz     = _flagToSize(flags);
    if (!sz) {
        return -1;
    }

    *uuids = zt_calloc(char, sz);
    if (!*uuids) {
        return -1;
    }

    oVal   = _zt_uuid_fillstr(uuid, *uuids, sz, flags);
    if (oVal < 0) {
        zt_free(*uuids);
        *uuids = NULL;
    }

    return oVal;
} /* zt_uuid_tostr */

/*
 *  zt_uuid_fillstr
 *
 *  Formats and copies into 'uuids' a represent of the 'uuid' as specified by 'flags'.
 *
 *  Input:
 *      uuid - [Optional] A pointer to a valid zt_uuid_t structure.
 *      uuids - [Optional] A pointer to a character buffer in which to copy the string.
 *      uuids_size - [Required]  Pointer to a size_t containing the number of bytes allocated
 *          in 'uuids'.
 *      flags - [Required] One of [zt_uuid_base62_fmt, zt_uuid_std_fmt, zt_uuid_short_fmt]
 *
 *  Output:
 *      - Upon successful return, the function will return the number of characters copied into
 *          the param 'uuids', not including the terminating NULL byte.
 *      - A return value < 0 indicates an error.
 *      - If either 'uuid' or 'uuids' is NULL, the function will set *uuids_size to the
 *          mininum of bytes required in 'uuids' given the param 'flags'.  Will return 0.
 *      - If the buffer size passed in '*uuids_size' is inadequate, the function will set
 *          '*uuids_size' to the mininum of bytes required in 'uuids' and return 0.
 */

ssize_t
zt_uuid_fillstr(zt_uuid_t* uuid, char* uuids, size_t* uuids_size, zt_uuid_flags_t flags) {
    size_t sizerequired = _flagToSize(flags);

    if (!sizerequired || !uuids_size) {
        return -1; /* Bad input */
    }
    /* User has asked how much data to allocate, or an inadequate string was passed in */
    if (!uuid || !uuids || *uuids_size < sizerequired) {
        *uuids_size = sizerequired;
        return 0;
    }

    return _zt_uuid_fillstr(uuid, uuids, *uuids_size, flags);
} /* zt_uuid_fillstr */

int
zt_uuid_fromstr(char * uuidstr, zt_uuid_t * uuid, zt_uuid_flags_t flags) {
    char uuid_hex[UUID_SHORT_STR_LEN + 1]; /* +1 for sscanf */

    zt_assert(uuidstr);
    zt_assert(uuid);


    if (flags == zt_uuid_std_fmt) {
        if (strlen(uuidstr) != UUID_STR_LEN) {
            return -1;
        }
        sscanf(uuidstr, "%8s-%4s-%4s-%4s-%12s",
               uuid_hex,
               &uuid_hex[8],
               &uuid_hex[12],
               &uuid_hex[16],
               &uuid_hex[20]);
        zt_hex_to_binary(uuid_hex, UUID_SHORT_STR_LEN, uuid->data.bytes, UUID_ALEN);
    } else if (flags == zt_uuid_short_fmt) {
        if (strlen(uuidstr) != UUID_SHORT_STR_LEN) {
            return -1;
        }
        memcpy(uuid_hex, uuidstr, UUID_SHORT_STR_LEN);
        zt_hex_to_binary(uuid_hex, UUID_SHORT_STR_LEN, uuid->data.bytes, UUID_ALEN);
    } else if (flags == zt_uuid_base62_fmt) {
        uint64_t v1 = 0;
        uint64_t v2 = 0;
        int       i;

        if (base62_decode_int64(uuidstr, _UUID_BASE62_SEGMENT, &v1) != 0) {
            return -1;
        }
        if (base62_decode_int64(uuidstr + _UUID_BASE62_SEGMENT, UUID_BASE62_STR_LEN, &v2) != 0) {
            return -1;
        }

        for (i = 0; i < 8; i++) {
            const int shift = (7 - i) * 8;
            uuid->data.bytes[i]     = (v1 >> shift) & 0xFF;
            uuid->data.bytes[i + 8] = (v2 >> shift) & 0xFF;
        }
    } else {
        zt_log_printf(zt_log_err, "unknown uuid format");
        return -1;
    }


    return 0;
} /* zt_uuid_fromstr */

int
zt_uuid_cmp(zt_uuid_t * uuid, zt_uuid_t * uuid2) {
    return memcmp(uuid->data.bytes, uuid2->data.bytes, UUID_ALEN);
}

#define VALID_CHARS "abcdefABCDEF0123456789"

int
zt_uuid_isvalid(char * uuid, zt_uuid_flags_t flags) {
    size_t len;
    size_t i;

    if (uuid == NULL) {
        return -1;
    }

    len = strlen(uuid);

    switch (flags) {
        case zt_uuid_std_fmt:
            if (len != UUID_STR_LEN) {
                return -1;
            }
            break;
        case zt_uuid_short_fmt:
            if (len != UUID_SHORT_STR_LEN) {
                return -1;
            }
            break;
        case zt_uuid_base62_fmt:
            if (len != UUID_BASE62_STR_LEN) {
                return -1;
            }
            break;
        default:
            return -1;
    }


    /* 550e8400-e29b-41d4-a716-446655440000
     * 550e8400 - e 2  9  b  -  4  1  d  4  -  a  7  1  6  -  446655440000
     * 01234567 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23              */

    for (i = 0; i < len; i++) {
        if (flags == zt_uuid_std_fmt) {
            switch (i) {
                case 8:
                case 13:
                case 18:
                case 23:
                    if (uuid[i] != '-') {
                        return -1;
                    }
                    break;
                default:
                    if (strchr(VALID_CHARS, uuid[i]) == NULL) {
                        return -1;
                    }
                    break;
            }
        } else if (flags == zt_uuid_base62_fmt) {
            if (strchr(base62_alphabet, uuid[i]) == NULL) {
                return -1;
            }
        } else {
            if (strchr(VALID_CHARS, uuid[i]) == NULL) {
                return -1;
            }
        }
    }

    return 0;
} /* zt_uuid_isvalid */

static int
base62_encode_int64(uint64_t in, char * out, size_t olen) {
    unsigned int i   = _UUID_BASE62_SEGMENT;
    int          ret = 0;

    if (!out || olen < _UUID_BASE62_SEGMENT) {
        return -1;
    }

    while (in) {
        const unsigned int v = (unsigned int)(in % 62);

        out[--i] = base62_alphabet[v];
        in      /= 62;
    }

    ret = i;
    while (i > 0) {
        out[--i] = base62_alphabet[0];
    }

    if (olen > _UUID_BASE62_SEGMENT) {
        out[_UUID_BASE62_SEGMENT] = 0;
    }

    return ret;
}

static int
base62_decode_int64(const char * in, size_t ilen, uint64_t * out) {
    uint64_t    v    = 0;
    const size_t tlen = (ilen > _UUID_BASE62_SEGMENT) ? _UUID_BASE62_SEGMENT : ilen;
    size_t       i;

    if (!in || !out) {
        return -1;
    }

    for (i = 0; i < tlen; i++) {
        const char c = in[i];
        int        n;

        /* shift back from our alphabet */
        if (c >= '0' && c <= '9') {
            n = c - '0';
        } else if (c >= 'a' && c <= 'z') {
            n = c - ('a' - 10); /* 10 = offset of 'a' in our alphabet */
        } else if (c >= 'A' && c <= 'Z') {
            n = c - ('A' - 36); /* 36 = offset of 'A' in our alphabet */
        } else {
            return -1; /* invalid character */
        }

        v += n * (uint64_t)(powl(62, tlen - i - 1));
    }

    *out = v;
    return 0;
}